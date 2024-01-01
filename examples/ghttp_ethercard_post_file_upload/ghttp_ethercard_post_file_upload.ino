/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using data processed 
 * by STM32 ethercard library port on maple mini clone hardware.  A route offers a form with file upload via your chosen browser (client) over port 4000.  Any 
 * other route gets redirected 403 forbidden.  Upload size is limited to request/response buffer and exceeding yields 413 payload to large.
 *
 * Each uploaded file is passed through a simple checksum which is output in response to client.  On the host you can validate your file checksums with python 
 * script checksum.py found in example folder.
 *
 * TCP/IP transfers within this example support request/responses carried by more than one packet and segmentation of request is performed by GHTTP library 
 * during parse.
 *
 * This file could be wrapped into a class to hide the ethercard specifics and remove anything related to physical i/f hardware.  All of the routes no nothing 
 * about this interface.
 * 
 * For STM32.  Requires: https://github.com/stm32duino 1.4.0 and library Serasidis_EtherCard_STM with enc28j60.cpp.patch applied
 * SPI Wiring:
 *     Enc28jxx   Maple Mini
 *     SI      => I/O 4
 *     SO      => I/O 5
 *     CK      => I/O 6
 *     CS      => I/O 7
 *
 * For AVR Mega.  Requires: Library njh-EtherCard
 * SPI Wiring:
 *     Enc28jxx   Mega2560
 *     SI      => I/O 51
 *     SO      => I/O 50
 *     CK      => I/O 52
 *     CS      => I/O 53
 *
 * Dave.C, 2020
 */


// Include
#include "ghttp_app.h"
#include "ecwrap.h"

using namespace examples::ghttp_ethercard_post_file_upload;

#if defined(ARDUINO_ARCH_AVR)
#include <EtherCard.h>
#define SPI_SS    SS

#elif defined(ARDUINO_ARCH_STM32)
#include <EtherCard_STM.h>
#define SPI_SS    D7

#else
// Hack around and see what you get working, you're in unknown territory
#error Unsupported architecture
#endif

// Our application's shared data, like PHP super globals
Shared sd;

// Create our routes handled by this application
RouteRoot route_root;


void setup() {
    Serial.begin(9600);
    // This while implies you should open Debug serial console before it will run - well it is a demo...
    while(!Serial) { Serial.available(); }

    Serial.println("setup()");

    // This would generally be setup once at POR
    sd.server.ip4           = "10.0.0.10";
    sd.server.mac           = "123456123456";
    sd.server.gateway_ip4   = "10.0.0.1";

    pinMode(SPI_SS, OUTPUT);    // CS
   
    if (ECWrap::Initialise(4000, &sd.server.ip4.u8[0], &sd.server.gateway_ip4.u8[0], &sd.server.mac.u8[0], SPI_SS)) {
        Serial.println( "Failed to access Ethernet controller");
        return;
    }

    // Register shared memory instance for route handler use
    sd.rm.RegisterShared(&sd);

    // Register routes for this application
    if (-1 == sd.rm.Register(&route_root)) {
        Serial.println("ERROR: Route manager register of root failed\n");
        return;
    }
    
    // Set response maximum to be size of buffer.  Since we have our own buffer it won't change.  It isnt this size because of headers but for this example I wont worry about that
    sd.response.SetMaximum(ECWrap::GetHTTPBufferLength());
}


/*
 * Not to much information on the low level workings of Ethercard and enc28j60.  Here the IP and TCP headers
 * broken down.  Example header I sniffed then decoded:
 *
 * offset 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e
 * 0000   12 34 56 12 34 56 B8 6B 23 D3 35 CB 08 00 45 
 * 000F   00 02 D1 12 E2 40 00 80 06 D1 36 0A 00 00 05 
 * 001E   0A 00 00 0A E2 A4 0F A0 A6 5E CE 2A 00 00 10 
 * 002D   01 50 18 FA F0 22 E6 00 00 
 * 
 * IP header (starting at offset 0x0e):
 * 4 Version
 * 5 Header length (IHL)
 * 00 TOS
 * 02 D1 Total Length
 * 12 E2 Identification?
 * 4 Flags.  4=Do not fragment
 * 0 00 fragment offset
 * 80 Time To Live
 * 06 protocol.  6=TCP
 * D1 36 header checksum
 * 0A 00 00 05 source ip
 * 0A 00 00 0A destination ip
 * 
 * TCP header (starting at offset 0x22):
 * E2 A4 source port 42210
 * 0F A0 destination port 4000
 * A6 5E CE 2A sequence number
 * 00 00 10 01 acknowledge number
 * 5 offset
 * 0 reserved
 * 18 TCP flags (ACK+PUSH)
 * FA F0 window
 * 22 E6 checksum
 * 00 00 urgent pointer 
 * 
 * So Ethernet::buffer[0x2f] = 0x18 => TCP flags Ack + Push
 * Field information source: https://nmap.org/book/tcpip-ref.html
 */


void loop() {
    uint8_t err;

    Serial.available();

    // Receive HTTP request
    if (ECWrap::Receive(&err)) {
        if (sd.request.ERR_NONE == err) {
            Serial.print("route ");
            Serial.println(sd.request.url.Get());

            // Service routes
            if (-1 == sd.rm.Service()) {
                sd.response.SetBuffer(sd.request.GetBuffer(), 0, 0);
                ghttp::HTTPHelper::Redirect<Shared>(sd, "/", ghttp::HTTPResponseStatus::STATUS_FORBIDDEN);
            }
            
            // Has response already been handled by route?
            if (sd.response.GetLength()) {
                // No so send data from response buffer now
                //Serial.print("main response ");
                //Serial.println(sd.response.GetLength(), DEC);
                //Serial.println(sd.response.GetBuffer());
                ECWrap::Send(sd.response.GetBuffer(), sd.response.GetLength());
            }
        }else if (sd.request.ERR_SEGMENTATION == err) {
            // Ignore

        }else if (sd.request.ERR_PARSE_UNEXPECTED_DATA == err) {
            sd.response.SetBuffer(sd.request.GetBuffer(), 0, 0);
            ghttp::HTTPHelper::Redirect<Shared>(sd, "/", ghttp::HTTPResponseStatus::STATUS_PAYLOAD_TOO_LARGE);
            Serial.print("response ");
            Serial.println(sd.response.GetLength(), DEC);
            //Serial.println(sd.response.GetBuffer());
            ECWrap::Send(sd.response.GetBuffer(), sd.response.GetLength());
            
        }else {
            // Anything is a bad request
            sd.response.SetBuffer(sd.request.GetBuffer(), 0, 0);
            ghttp::HTTPHelper::Redirect<Shared>(sd, "/", ghttp::HTTPResponseStatus::STATUS_BAD_REQUEST);
            //Serial.print("response ");
            //Serial.println(sd.response.GetLength(), DEC);
            //Serial.println(sd.response.GetBuffer());
            ECWrap::Send(sd.response.GetBuffer(), sd.response.GetLength());
        }
    }
}
