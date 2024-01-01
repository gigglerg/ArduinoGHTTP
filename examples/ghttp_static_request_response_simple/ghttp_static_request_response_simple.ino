/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using static 
 * request data which could have come from any hardware.  To keep it simple there are no cookies or session and the request is urlencoded
 *
 * \cond PRIVATE
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * POST /something?page=123 HTTP/1.1
 * Content-Length: 28
 * Accept: application/json, text/javascript, *-*; q=0.01
 * Origin: null
 * Cookie: name=value; SESSION=1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * text=Dave%20waz%20ere%202020
 * Response:
 * HTTP/1.1 200 OK
 * Content-Length: 77
 * 
 * <html><body><p>Anything to say? Dave waz ere 2020, page 123</p></body></html>
 * Done
 * \endcond
 * 
 * DG, 2020
 */


/* Adjust some parameters to suit our application.  Actually these are overly restrictive here as they would be setup to meet the requirements of all your routes, clients and meta data */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0 // Not using for this app
#define GHTTP_REQUEST_PROCESS_COOKIES 1
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 10
#define GHTTP_REQUEST_MAX_VARS 6
#define GHTTP_REQUEST_MAX_COOKIE 4
#define GHTTP_SERVER_MAX_ROUTES 10


// Include
#include "ghttp.h"
#include "global.h"
#include "shared.h"
#include "routesomething.h"

using namespace examples::ghttp_static_request_response_simple;

/*
 * Request/response buffer.  The data would come from your chosen hardware
 */
char buffer[448];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);    // Maximum of 10 routes in this example

// Create our routes handled by this application
RouteSomething route_something;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // This would generally be setup once at POR
    sd.server.ip4 = "10.0.0.2";

    // This would have been setup before request processing using information from your chosen hardware i/f
    sd.client.ip4 = "10.0.0.10";

    // Register routes.  This should be done once at POR, one invoke for each application route
    if (-1 == rm.Register(&route_something)) {
        printf("ERROR: Route manager register of something failed\n");
        return;
    }

    // Tell our response instance to use the same buffer as request to reduce memory footprint.  You could do this when handling the request, changing offset as required
    sd.response.SetMaximum(sizeof(buffer));
    sd.response.SetBuffer(buffer, 0, 100); // Offset 100characters into request allowing access to some of the request while building response

    // Request, assumed received over TCP/IP by some hardware interface
    strcpy_P(buffer, PSTR(
        "POST /something?page=123 HTTP/1.1\r\n" \
        "Content-Length: 28\r\n" \
        "Accept: application/json, text/javascript, */*; q=0.01\r\n" \
        "Origin: null\r\n" \
        "Cookie: name=value; SESSION=1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk\r\n" \
        "Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
        "Accept-Encoding: gzip, deflate\r\n" \
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
        "text=Dave%20waz%20ere%202020"));
    Serial.println(buffer);

    // Parse our request...
    uint16_t ci = sd.request.Parse(buffer, strlen(buffer), sizeof(buffer), /*parse_content*/true, /*segmentation*/false);
    if (sd.request.ERR_NONE != sd.request.GetLastError()) {
        Serial.print("Error: ");
        Serial.println(sd.request.GetLastError(), DEC);
        return;
    }

    // Service routes
    if (-1 == rm.Service()) {
        Serial.print("Error: Our route wasnt taken");
        return;
    }

#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
    // In this example it is optional, included here because you would service session manager periodically.  Internally it will react to a 1S granularity
    sd.session_manager.Service();
#endif

    // Our response to be sent over your chosen hardware is accessible via response instance GetBuffer() and GetLength().  Here it is simply output over debug serial as a string
    Serial.println("Response:");
    Serial.println(sd.response.GetBuffer());

    Serial.println("Done");
}

void loop() {
    Serial.available();
}
