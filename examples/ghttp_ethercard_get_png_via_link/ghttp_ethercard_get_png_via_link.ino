/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using data processed 
 * by STM32 ethercard library port on maple mini clone hardware.  Root route HTML includes an image link to a logo resource on device and is requested by 
 * clients but is to large for SRAM buffering so taken directly from MCU flash, for details see route/routelogojpg.h.  An internet connection is also 
 * required as references are made to google hosted libraries for JQuery Javascript.
 * 
 * For STM32.  Requires: https://github.com/stm32duino 1.4.0 and library Serasidis_EtherCard_STM with enc28j60.cpp.patch applied
 * SPI Wiring:
 *     Enc28jxx   Maple Mini
 *     SI      => I/O 4
 *     SO      => I/O 5
 *     CK      => I/O 6
 *     CS      => I/O 7
 *
 * For AVR Mega2560.  This example does not work as the image data spans banks requiring 32bit PROGMEM pointers, feel free to correct
 *
 * DG, 2020
 */

// Include
#include "ghttp_app.h"
#include "ecwrap.h"
#include "route/routeroot.h"
#include "route/routelogojpg.h"

using namespace examples::ghttp_ethercard_get_png_via_link;

#if defined(ARDUINO_ARCH_STM32)
#define SPI_SS    D7

#else
// Hack around and see what you get working, you're in unknown territory
#error Unsupported architecture
#endif

// Our application's shared data, like PHP super globals.  This pretty clean where the main application is simply our shared instance + routes
Shared sd;

// Create our routes handled by this application
RouteRoot    route_root;
RouteLogoJpg route_logojpg;


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
    if (-1 == sd.rm.Register(&route_logojpg)) {
        Serial.println("ERROR: Route manager register of logojpg failed\n");
        return;
    }
    
    // Set response maximum to be size of buffer.  Since we have our own buffer and none of the routes access 
    // request after response building starts, it won't change
    sd.response.SetMaximum(ECWRAP_REQRSP_BUFFER_LENGTH);
}


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
                ECWrap::Send(sd.request.GetBuffer(), sd.response.GetLength());
            }
        }else if (sd.request.ERR_SEGMENTATION == err) {
            // Ignore

        }else {
            // Anything is a bad request
            sd.response.SetBuffer(sd.request.GetBuffer(), 0, 0);
            ghttp::HTTPHelper::Redirect<Shared>(sd, "/", ghttp::HTTPResponseStatus::STATUS_BAD_REQUEST);
            //Serial.print("response ");
            //Serial.println(sd.response.GetLength(), DEC);
            //Serial.println(sd.response.GetBuffer());
            ECWrap::Send(sd.request.GetBuffer(), sd.response.GetLength());
        }
    }
}
