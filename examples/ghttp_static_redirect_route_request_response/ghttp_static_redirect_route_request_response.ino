/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using static 
 * request data which could have come from any hardware.  To keep it simple there are no cookies or session and the request is urlencoded.  Routes 
 * redirect to other routes where a HTTP client would request each and finally end up at some non-redirect route
 *
 * \cond PRIVATE
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * Request:
 * GET /one HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 301 Moved Permanently
 * Location: http://10.0.0.2/two
 * Content-Length: 0
 * 
 * 
 * 
 * Request:
 * GET /two HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 302 Found
 * Location: http://10.0.0.2/msg?text=testing
 * Content-Length: 0
 * 
 * 
 * 
 * Request:
 * GET /msg?text=testing HTTP/1.1
 * Content-Length: 0
 * Accept: text/html, *-*; q=0.01
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * 
 * Response:
 * HTTP/1.1 200 OK
 * Content-Length: 45
 * 
 * <html><body><p>Msg: testing</p></body></html>
 * 
 * Done
 * \endcond
 * 
 * Dave.C, 2020
 */


/* Adjust some parameters to suit our application.  Actually these are overly restrictive here as they would be setup to meet the requirements of all your routes, clients and meta data */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0 // Not using for this app
#define GHTTP_REQUEST_PROCESS_COOKIES 0
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 10
#define GHTTP_REQUEST_MAX_VARS 6
#define GHTTP_REQUEST_MAX_COOKIE 4
#define GHTTP_SERVER_MAX_ROUTES    5


// Include
#include "ghttp.h"
#include "shared.h"
#include "routeone.h"
#include "routetwo.h"
#include "routemsg.h"

using namespace examples::ghttp_static_redirect_route_request_response;

/*
 * Request/response buffer.  The data would come from your chosen hardware
 */
char buffer[448];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);    // Maximum of 5 routes in this example

// Create our routes handled by this application
RouteOne route_one;
RouteTwo route_two;
RouteMsg route_msg;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // This would generally be setup once at POR
    sd.server.ip4 = "10.0.0.2";

    // This would have been setup before request processing using information from your chosen hardware i/f
    sd.client.ip4 = "10.0.0.10";

    // Register routes.  This should be done once at POR, one invoke for each application route
    if (-1 == rm.Register(&route_one)) {
        printf("ERROR: Route manager register of one failed\n");
        return;
    }
    if (-1 == rm.Register(&route_two)) {
        printf("ERROR: Route manager register of two failed\n");
        return;
    }
    if (-1 == rm.Register(&route_msg)) {
        printf("ERROR: Route manager register of msg failed\n");
        return;
    }

    for(uint8_t i=0; i<3; i++) {
        // Request, assumed received over TCP/IP by some hardware interface.  Select route based upon i
        switch(i) {
            case 0 :
                strcpy_P(buffer, PSTR("GET /one HTTP/1.1\r\n"));
                break;
            case 1 :
                strcpy_P(buffer, PSTR("GET /two HTTP/1.1\r\n"));
                break;
            case 2 :
                strcpy_P(buffer, PSTR("GET /msg?text=testing HTTP/1.1\r\n"));
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
        sd.request.Clear(); // as there is support for segmentation even if not used
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
