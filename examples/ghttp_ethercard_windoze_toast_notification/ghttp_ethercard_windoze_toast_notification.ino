/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using data processed 
 * by STM32 ethercard library port on maple mini clone hardware.  A few routes exist covering HTML and PNG image data served over port 80 with the root page 
 * allowing client side javascript control of notifications.
 *
 * NOTE: You must use Microsoft Edge.  Chrome below M62 will work however recent additions require SSL certificates for anything other than localhost
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
 * DG, 2020
 */


/* Adjust some parameters to suit our application */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0
#define GHTTP_REQUEST_PROCESS_COOKIES 1
#define GHTTP_REQUEST_MAX_COOKIE 5
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 15
#define GHTTP_REQUEST_MAX_VARS 8
#define GHTTP_SERVER_MAX_ROUTES    5
#define GHTTP_SERVER_ACCEPT_AUTHORIZATION 0

// Include
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

#include "net.h" // From Ethercard library TCP/IP constants (low level)
#include "ghttp.h"
#include "shared.h"
#include "route/routeroot.h"
#include "route/routetoastjpg.h"
#include "route/routenotificationjs.h"

using namespace examples::ghttp_ethercard_windoze_toast_notification;

// Request/response buffer for EtherCard library
byte Ethernet::buffer[4*1024];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);

// Create our routes handled by this application
RouteRoot            route_root;
RouteToastJpg        route_toastjpg;
RouteNotificationJs  route_notificationjs;

void setup() {
    Serial.begin(9600);
    // This while implies you should open Debug serial console before it will run - well it is a demo...
    while(!Serial) { Serial.available(); }

    Serial.println("setup()");

    // This would generally be setup once at POR
    sd.server.ip4           = "10.0.0.10";
    sd.server.mac           = "123456123456";
    sd.server.gateway_ip4   = "10.0.0.1";

    // Server TCP/IP port
    ether.hisport = 80;

    pinMode(SPI_SS, OUTPUT);    // CS
    
    if (ether.begin(sizeof(Ethernet::buffer), &sd.server.mac.u8[0], SPI_SS) == 0) {
        Serial.println( "Failed to access Ethernet controller");
        return;
    }

    // As per Ethercard examples, setup static IP addresses of server and local gateway
    ether.staticSetup(&sd.server.ip4.u8[0], &sd.server.gateway_ip4.u8[0]);

    // Register routes for this application
    if (-1 == rm.Register(&route_root)) {
        Serial.println("ERROR: Route manager register of root failed\n");
        return;
    }
    if (-1 == rm.Register(&route_toastjpg)) {
        Serial.println("ERROR: Route manager register of toastjpg failed\n");
        return;
    }
    if (-1 == rm.Register(&route_notificationjs)) {
        Serial.println("ERROR: Route manager register of notificationjs failed\n");
        return;
    }
}

#define PACKET_CHUNK_SIZE 512    // Bytes

/* <rant>
 * If I had known the issues and how simplistic the Ethercard API was - I'd of used another h/w i/f, personally this is the reason for writing new libraries as 
 * everything I touch in Arduino is either a hack, half finished or doesn't work with little to no help beyond obvious examples that don't use the i/f in the way
 * you'd expect.  What is really required is a POSIX socket interface and the end of this mishmash of piecemeal ethernet TCP/IP interface(s).  Users shouldn't 
 * need to know about protocol specific sequence numbers, acknowledges and anything else under the hood.
 * </rant>
 */
void SendLargeResponse() {
    uint16_t tl = sd.response.GetLength(), o, l = PACKET_CHUNK_SIZE;
    ether.httpServerReplyAck(); // Send ack to the request
    for(o=0; o<tl; o+=PACKET_CHUNK_SIZE) {
        if ((o+PACKET_CHUNK_SIZE) > tl) {
            l = tl - o; // Remainder, last chunk
        }
        // Response data after first chunk?
        if (o>0) {
            // Move each data chunk down in response buffer, just above TCP/IP header
            memcpy(sd.misc.response_buffer, sd.misc.response_buffer+o, l);
        }
        if ((o+l) == tl) {
            ether.httpServerReply_with_flags(l, TCP_FLAGS_ACK_V | TCP_FLAGS_FIN_V);
        }else {
            ether.httpServerReply_with_flags(l, TCP_FLAGS_ACK_V);
        }
    }
}

void loop() {
    uint16_t buffer_pos;
    char* rtx_data;

    Serial.available();

    // Wait for an incoming TCP packet, but ignore its contents
    buffer_pos = ether.packetLoop(ether.packetReceive());
    if (buffer_pos) {
        rtx_data = (char *) Ethernet::buffer + buffer_pos;

        //Serial.println("request");
        //Serial.println(rtx_data);

        // Update client information (ip/mac) from ethercard library.  This is really only required for sessions as far as GHTTP is concerned
#if GHTTP_SERVER_USE_SESSIONS == 1
        for(uint8_t i=0; i<4; i++) {
            sd.client.ip4.u8[i] = Ethernet::buffer[IP_SRC_P + i];
        }
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        for(uint8_t i=0; i<6; i++) {
            sd.client.mac.u8[i] = Ethernet::buffer[ETH_SRC_MAC + i];
        }
#endif // GHTTP_SESSIONMGR_INCLUDE_MAC == 1
#endif // GHTTP_SERVER_USE_SESSIONS == 1

        // Parse our request...
        uint16_t ci = sd.request.Parse(rtx_data, strlen(rtx_data), sizeof(Ethernet::buffer) - buffer_pos, /*parse_content*/true, /*segmentation*/false);
        if (sd.request.ERR_NONE != sd.request.GetLastError()) {
            Serial.print("Error: ");
            Serial.println(sd.request.GetLastError(), DEC);
        }else {
            // Not using sessions and responses don't need request variables so we can use entire request buffer for response.  We could set this per request but this demo is simple so here will do for all responses
            sd.response.SetMaximum(sd.request.GetMaximum());
            // Ethercard responses over TCP need to skip packet header hence the get TCP offset call.  I didn't want any hardware specific calls in my routes so decided to keep it in a variable
            sd.misc.response_buffer = reinterpret_cast<char*>(ether.tcpOffset());
            
            // Service routes
            if (-1 == rm.Service()) {
                Serial.print("No route taken for '");
                Serial.print(sd.request.url.Get());
                Serial.println("'\n");
                ether.httpServerReply(0);
            }else {
                //Serial.println(sd.response.GetBuffer());
                SendLargeResponse();
            }
        }
    }
}