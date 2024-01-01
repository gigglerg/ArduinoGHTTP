/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_post_form_processing
 * 
 * Ethercard TCP/IP packet i/f wrapper to simplify application and remove h/w detail.  You don't have to use this API
 * 
 * Requires: https://github.com/stm32duino 1.4.0 and library Serasidis_EtherCard_STM with enc28j60.cpp.patch applied
 *
 * Dave.C, 2020
 */

#include "ghttp_app.h"
#if defined(ARDUINO_ARCH_AVR)
#include <EtherCard.h>

#elif defined(ARDUINO_ARCH_STM32)
#include <EtherCard_STM.h>
#endif
#include "net.h" // From Ethercard library TCP/IP constants (low level)
#include "ecwrap.h"

using namespace examples::ghttp_ethercard_get_post_form_processing;

/**
 * Our applications shared data, need access to request/response instances
 */
extern Shared sd;


/**
 * Single packet buffer for EtherCard library.  The Ethercard library is pretty poor, if the host sends a 
 * TCP/IP packet larger than this buffer it gets truncated by ether.packetReceive() to buffer size - it's 
 * not obvious how to get the remaining packet data
 */
byte Ethernet::buffer[ECWRAP_RTX_PACKET_LENGTH];


/**
 * Request/response for GHTTP.  Ideally this would be shared with the h/w i/f packet buffer but segmenting 
 * requests with Ethercard, dealing with low level protocol stack headers mixed with data causes problems 
 * for incrementally receiving a contiguous data stream
 */
char rqrsbuffer[ECWRAP_REQRSP_BUFFER_LENGTH];


uint8_t ECWrap::Initialise(const uint16_t server_port, const uint8_t* ip4, const uint8_t* gwip4, const uint8_t* mac, uint8_t pin) {
    uint8_t result = 0;
    
    // Server TCP/IP port
    ether.hisport = server_port;
    
    if (ether.begin(sizeof(Ethernet::buffer), mac, pin) == 0) {
        result = 1;
    }else {
        // As per Ethercard examples, setup static IP addresses of server and local gateway
        ether.staticSetup(ip4, gwip4);
    }
    
    return result;
}


void ECWrap::UpdateClientInformation() {
#if GHTTP_SERVER_USE_SESSIONS == 1
    for(uint8_t i=0; i<4; i++) {
        sd.client.ip4.u8[i] = Ethernet::buffer[IP_SRC_P + i];
    }
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    for(uint8_t i=0; i<6; i++) {
        sd.client.mac.u8[i] = Ethernet::buffer[ETH_SRC_MAC + i];
    }
#endif // GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    return;

#else // GHTTP_SERVER_USE_SESSIONS != 1

#if ECWRAP_UPDATE_CLIENT_IP == 1
    for(uint8_t i=0; i<4; i++) {
        sd.client.ip4.u8[i] = Ethernet::buffer[IP_SRC_P + i];
    }
#endif // ECWRAP_UPDATE_CLIENT_IP == 1

#if ECWRAP_UPDATE_CLIENT_MAC == 1
    for(uint8_t i=0; i<6; i++) {
        sd.client.mac.u8[i] = Ethernet::buffer[ETH_SRC_MAC + i];
    }
#endif // ECWRAP_UPDATE_CLIENT_MAC == 1

#endif // GHTTP_SERVER_USE_SESSIONS != 1
}


void ECWrap::Send(const char *response, uint32_t response_length, const bool first, const bool more) {
    uint32_t tl = response_length, o, l = ECWRAP_TX_SEGMENT_SIZE;
    char *packet_buffer = GetPacketPayloadBuffer();

    if (first) {
        ether.httpServerReplyAck(); // Send ack to the request
    }
    for(o=0; o<tl; o+=ECWRAP_TX_SEGMENT_SIZE) {
        if ((o+ECWRAP_TX_SEGMENT_SIZE) > tl) {
            l = tl - o; // Remainder, last segment
        }

        // Copy each data segment into response buffer, just above TCP/IP packet headers
        memcpy(packet_buffer, response+o, l);
        //Serial.print("bs ");
        //Serial.println(l, DEC);

        if ((o+l) == tl) {
            if (more) {
                ether.httpServerReply_with_flags(l, TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V);
                //Serial.println("Send: More");
            }else {
                ether.httpServerReply_with_flags(l, TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V);
                ether.httpServerReply_with_flags(0, TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V | TCP_FLAGS_FIN_V);
                //Serial.println("Send: Finish");
            }
        }else {
            ether.httpServerReply_with_flags(l, TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V);
            //Serial.println("Send: +");
        }
    }
}


bool ECWrap::Receive(uint8_t *err) {
    static uint32_t timeout = 0;
    uint16_t e, cl;
    bool received = false;
    
    // Wait for an incoming TCP packet, but ignore its contents
    uint16_t packet_length = ether.packetReceive(); // NOTE: This length appears to include +20Bytes TCP header
    uint16_t buffer_pos = ether.packetLoop(packet_length);
    if (buffer_pos) {
        packet_length -= 0x36; // Remove IP+TCP header length, both are present in packet buffer - thanks Ethercard see ENC28J60::tcpOffset
        char *packet_buffer = (char *) Ethernet::buffer + buffer_pos;
        
        cl = sd.request.GetLength();
        if (cl && millis()>timeout) {
            //Serial.println("Request Clear");
            sd.request.Clear();
            cl = 0;
            timeout = millis() + ECWRAP_RX_SEGMENT_TIMEOUT;
        }

        // Update client information in shared instance
        ECWrap::UpdateClientInformation();

        //Serial.print("request ");
        //Serial.println(packet_length, DEC);
        //Serial.println(packet_buffer);

        // Will this packet overflow our buffer?
        if ((cl + packet_length) > sizeof(rqrsbuffer)) {
            sd.request.Clear();
            timeout = millis() + ECWRAP_RX_SEGMENT_TIMEOUT;
            e = sd.request.ERR_PARSE_UNEXPECTED_DATA;
        }else {
            // Buffer packet as part of request + reset timeout
            memcpy(&rqrsbuffer[cl], packet_buffer, packet_length);
            cl += packet_length;

            // Parse our request (ignore returned index)
            sd.request.Parse(rqrsbuffer, cl, sizeof(rqrsbuffer));
            timeout = millis() -1; // Ensure next entry clears but not here so caller can query
            e = sd.request.GetLastError();
            if (sd.request.ERR_SEGMENTATION == e) {
                // Ignore.  Send ack to client indicating packet accept as part of request
                ether.httpServerReplyAck();
                timeout = millis() + ECWRAP_RX_SEGMENT_TIMEOUT;
            
            }else if (sd.request.ERR_NONE != e) {
                //Serial.print("Error: ");
                //Serial.println(e, DEC);
            }
        }
    
        *err = e;
        received = true;
    } // if (buffer_pos)

    return received;
} // ECWrap::Receive(...)


char* ECWrap::GetPacketPayloadBuffer() {
    return reinterpret_cast<char*>(ENC28J60::tcpOffset());
}


uint16_t ECWrap::GetPacketPayloadBufferLength() {
    uint16_t tcpip_header_length = (uint16_t)(ENC28J60::tcpOffset() - Ethernet::buffer);

    return ECWRAP_RTX_PACKET_LENGTH - tcpip_header_length;
}


char* ECWrap::GetHTTPBuffer() {
    return rqrsbuffer;
}


uint16_t ECWrap::GetHTTPBufferLength() {
    return ECWRAP_REQRSP_BUFFER_LENGTH;
}
