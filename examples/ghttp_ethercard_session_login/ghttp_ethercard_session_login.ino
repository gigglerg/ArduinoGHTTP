/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using data processed 
 * by STM32 ethercard library port on maple mini clone hardware.  Session cookies are employed and redirects are made on root content when not logged in.
 * Use any username (not empty string) and password "donotask" (neither are case sensitive)
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


/* Adjust some parameters to suit our application */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0
#define GHTTP_REQUEST_PROCESS_COOKIES 1
#define GHTTP_REQUEST_MAX_COOKIE 16
#define GHTTP_SERVER_USE_SESSIONS 1
#define GHTTP_SESSIONMGR_MAX_SESSIONS 5
#define GHTTP_REQUEST_MAX_HEADERS 20
#define GHTTP_REQUEST_MAX_VARS 8
#define GHTTP_SERVER_MAX_ROUTES    5
#define GHTTP_SESSIONMGR_INCLUDE_MAC 1 // Optional

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

#include "net.h" // Ethercard constants
#include "ghttp.h"
#include "global.h"
#include "shared.h"
#include "routeroot.h"
#include "routelogoff.h"
#include "routelogin.h"
#include "routefavicon.h"

using namespace examples::ghttp_ethercard_session_login;

// Request/response buffer for EtherCard library
byte Ethernet::buffer[2048];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);

// Create our routes handled by this application
RouteRoot route_root;
RouteLogoff route_logoff;
RouteLogin route_login;
RouteFavIcon route_favicon;

void setup() {
    Serial.begin(9600);
    // This while implies you should open Debug serial console before it will run - well it is a demo...
    while(!Serial) { }

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
    if (-1 == rm.Register(&route_logoff)) {
        Serial.println("ERROR: Route manager register of logoff failed\n");
        return;
    }
    if (-1 == rm.Register(&route_login)) {
        Serial.println("ERROR: Route manager register of login failed\n");
        return;
    }
    if (-1 == rm.Register(&route_favicon)) {
        Serial.println("ERROR: Route manager register of favicon failed\n");
        return;
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
            // Ethercard responses over TCP need to skip packet header hence the get TCP offset call.  I didn't want any hardware specific calls in my routes so decided to keep it in a variable
            sd.misc.response_buffer = reinterpret_cast<char*>(ether.tcpOffset());
            
            // Service routes
            if (-1 == rm.Service()) {
                Serial.print("No route taken for '");
                Serial.print(sd.request.url.Get());
                Serial.println("'\n");
                ether.httpServerReply(0);
            }else {
                ether.httpServerReply(sd.response.GetLength());
            }
            //Serial.println(sd.response.GetBuffer());
        }
    }
}