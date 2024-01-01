/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using static 
 * request data which could have come from any hardware.  To keep it simple there are no cookies or session and the request is urlencoded.  An unhandled 
 * route handler is registered which is taken last after all others for some default behaviour
 *
 * \cond PRIVATE
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * Request:
 * GET /foo HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 200 OK
 * Content-Length: 36
 * 
 * <html><body><p>Foo</p></body></html>
 * 
 * Request:
 * POST /bar HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 200 OK
 * Content-Length: 36
 * 
 * <html><body><p>Bar</p></body></html>
 * 
 * Request:
 * GET /doesntexist HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 200 OK
 * Content-Length: 42
 * 
 * <html><body><p>Unhandled</p></body></html>
 * 
 * Done
 * \endcond
 * 
 * DG, 2020
 */


/* Adjust some parameters to suit our application.  Actually these are overly restrictive here as they would be setup to meet the requirements of all your routes, clients and meta data */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0 // Not using for this app
#define GHTTP_REQUEST_PROCESS_COOKIES 0
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 10
#define GHTTP_REQUEST_MAX_VARS 6
#define GHTTP_REQUEST_MAX_COOKIE 4
#define GHTTP_SERVER_MAX_ROUTES 5


// Include
#include "ghttp.h"
#include "shared.h"
#include "routefoo.h"
#include "routebar.h"
#include "routeunhandled.h"

using namespace examples::ghttp_static_unhandled_route_request_response;

/*
 * Request/response buffer.  The data would come from your chosen hardware
 */
char buffer[448];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);    // Maximum of 5 routes in this example

// Create our routes handled by this application
RouteFoo       route_foo;
RouteBar       route_bar;
RouteUnhandled route_unhandled;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // This would generally be setup once at POR
    sd.server.ip4 = "10.0.0.2";

    // This would have been setup before request processing using information from your chosen hardware i/f
    sd.client.ip4 = "10.0.0.10";

    // Register routes.  This should be done once at POR, one invoke for each application route
    if (-1 == rm.Register(&route_foo)) {
        printf("ERROR: Route manager register of foo failed\n");
        return;
    }
    if (-1 == rm.Register(&route_bar)) {
        printf("ERROR: Route manager register of bar failed\n");
        return;
    }
    /*
     * Register our unhandled route, handler.  It is very important that this route handler be registered last as we want all 
     * other possible routes tested first before falling back to this.  As requests come in and are parsed, passed to route manager 
     * and processed, when a request has not route they fall through until they get to this unhandled route.  Default action can 
     * then be applied to our response in these situations
     */
    if (-1 == rm.Register(&route_unhandled)) {
        printf("ERROR: Route manager register of pre failed\n");
        return;
    }

    for(uint8_t i=0; i<3; i++) {
        // Tell our response instance to use the same buffer as request to reduce memory footprint.  You could do this when handling the request, changing offset as required via sd.response.SetBuffer
        sd.response.SetMaximum(sizeof(buffer));
        sd.response.SetBuffer(buffer, 0);

        // Request, assumed received over TCP/IP by some hardware interface.  Select route based upon i
        switch(i) {
            case 0 :
                strcpy_P(buffer, PSTR("GET /foo HTTP/1.1\r\n"));
                break;
            case 1 :
                strcpy_P(buffer, PSTR("POST /bar HTTP/1.1\r\n"));
                break;
            case 2 :
                strcpy_P(buffer, PSTR("GET /doesntexist HTTP/1.1\r\n"));
                break;
        }
        strcat_P(buffer, PSTR(
            "Content-Length: 0\r\n" \
            "Accept: text/html, */*; q=0.01\r\n" \
            "Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
            "Accept-Encoding: gzip, deflate\r\n" \
            "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n"));

        Serial.println("Request:");
        Serial.println(buffer);

        // Parse our request...
        sd.request.Clear(); // Support for segmentation.  While not using here we must ensure request is empty and new at start of each parse attempt OR clear segmentation flag on parse method
        uint16_t ci = sd.request.Parse(buffer, strlen(buffer), sizeof(buffer));
        if (sd.request.ERR_NONE != sd.request.GetLastError()) {
            Serial.print("Error: ");
            Serial.println(sd.request.GetLastError(), DEC);
            return;
        }

        // Service routes
        if (-1 == rm.Service()) {
            Serial.println("ERROR: No route taken\n");
            return;
        }else {
            // Our response to be sent over your chosen hardware is accessible via response instance GetBuffer() and GetLength().  Here it is simply output over debug serial as a string
            Serial.println("Response:");
            Serial.println(sd.response.GetBuffer());
            Serial.println();
        }

    } // for(uint8_t i=0;...)

    Serial.println("Done");
}

void loop() {
    Serial.available();
}
