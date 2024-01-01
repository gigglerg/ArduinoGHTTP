/**
 * \file
 *
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Example of complete request and response using static 
 * request data which could have come from any hardware.  Session variables are used with a counter which is incremented per request and output in 
 * resulting response.  A few iterations of request/response are made to demonstrate session handling within your route classes
 *
 * \cond PRIVATE
 * Expected serial output (Arduino/Maple mini/Mega2560):
 * setup()
 * Request: 0
 * POST /counter HTTP/1.1
 * Content-Length: 26
 * Accept: text/html, *-*; q=0.01
 * Cookie: name=value
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * ignore=Some%20text%20input
 * Global.Clear(idx 0)
 * Global.Start(idx 0)
 * Response:
 * HTTP/1.1 200 OK
 * Cookie-Set: SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Length: 50
 * 
 * <html><body><p>Session counter 1</p></body></html>
 * Request: 1
 * POST /counter HTTP/1.1
 * Content-Length: 26
 * Accept: text/html, *-*; q=0.01
 * Cookie: name=value; SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * ignore=Some%20text%20input
 * Global.Continue(idx 0)
 * Response:
 * HTTP/1.1 200 OK
 * Cookie-Set: SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Length: 50
 * 
 * <html><body><p>Session counter 2</p></body></html>
 * Request: 2
 * POST /counter HTTP/1.1
 * Content-Length: 26
 * Accept: text/html, *-*; q=0.01
 * Cookie: name=value; SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * ignore=Some%20text%20input
 * Global.Continue(idx 0)
 * Response:
 * HTTP/1.1 200 OK
 * Cookie-Set: SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Length: 50
 * 
 * <html><body><p>Session counter 3</p></body></html>
 * Request: 3
 * POST /counter HTTP/1.1
 * Content-Length: 26
 * Accept: text/html, *-*; q=0.01
 * Cookie: name=value; SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * ignore=Some%20text%20input
 * Global.Continue(idx 0)
 * Response:
 * HTTP/1.1 200 OK
 * Cookie-Set: SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Length: 50
 * 
 * <html><body><p>Session counter 4</p></body></html>
 * Request: 4
 * POST /counter HTTP/1.1
 * Content-Length: 26
 * Accept: text/html, *-*; q=0.01
 * Cookie: name=value; SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * ignore=Some%20text%20input
 * Global.Continue(idx 0)
 * Response:
 * HTTP/1.1 200 OK
 * Cookie-Set: SESSION=VrwYsrSm,Ahz09Q5f2Z76xphTcEpc3WK
 * Content-Length: 50
 * 
 * <html><body><p>Session counter 5</p></body></html>
 * Done
 * \endcond
 * 
 * DG, 2020
 */


/* Adjust some parameters to suit our application.  Actually these are overly restrictive here as they would be setup to meet the requirements of all your routes, clients and meta data */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0 // Not using for this app
#define GHTTP_REQUEST_PROCESS_COOKIES 1
#define GHTTP_SERVER_USE_SESSIONS 1
#define GHTTP_REQUEST_MAX_HEADERS 10
#define GHTTP_REQUEST_MAX_VARS 6
#define GHTTP_REQUEST_MAX_COOKIE 4
#define GHTTP_SERVER_MAX_ROUTES    10


// Include
#include "ghttp.h"
#include "global.h"
#include "shared.h"
#include "routecounter.h"

using namespace examples::ghttp_static_request_response_session_simple;

/*
 * Request/response buffer.  The data would come from your chosen hardware
 */
char buffer[600];

// Our application's shared data, like PHP super globals
Shared sd;

// Our application's route manager
ghttp::RouteManager<GHTTP_SERVER_MAX_ROUTES, ghttp::Route<Shared>, Shared>  rm(&sd);    // Maximum of 10 routes in this example

// Create our routes handled by this application
RouteCounter route_counter;
// ...

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    /*
     * You might be wondering what all this code is for?  Well because the framework is data centric and network hardware device independent, it is your 
     * responsibility to plumb the framework into whatever network hardware you choose.   There are examples with Ethercard library which you can look at
     */

    // This would generally be setup once at POR
    sd.server.ip4 = "10.0.0.2";

    // This would have been setup before request parse using information from your chosen hardware i/f
    sd.client.ip4 = "10.0.0.10";
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    sd.client.mac = "123456789abc";
#endif

    // Register routes.  This should be done once at POR, one invoke for each application route
    if (-1 == rm.Register(&route_counter)) {
        Serial.println("ERROR: Route manager register of something failed\n");
        return;
    }

    // We process the request twice so you can see a session reused with the 2nd client request
    char session[GHTTP_SESSIONID_LENGTH];
    for(uint8_t i=0; i<5; i++) {
        Serial.print("Request: ");
        Serial.println(i, DEC);

        // Request, assumed received over TCP/IP by some hardware interface.  Clients echo session cookies so all this "if ... else ..." is simply emulating what would happen
        // with a compliant client
        strcpy_P(buffer, PSTR(
            "POST /counter HTTP/1.1\r\n" \
            "Content-Length: 26\r\n" \
            "Accept: text/html, */*; q=0.01\r\n" \
            "Cookie: name=value"
            ));
        if (i) {
            strcat_P(buffer, PSTR("; "));
#if defined(ARDUINO_ARCH_AVR)
            PGM_STRCAT(buffer, GHTTP_SERVER_STRING_SESSION_COOKIE);
#else // !defined(ARDUINO_ARCH_AVR)
            strcat(buffer, GHTTP_SERVER_STRING_SESSION_COOKIE);
#endif
            strcat(buffer, "=");
            strcat(buffer, session);
        }
        strcat_P(buffer, PSTR(
            "\r\nContent-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
            "Accept-Encoding: gzip, deflate\r\n" \
            "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
            "ignore=Some%20text%20input"));
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
        
        // This is a hack for a static content example where there is no client and the requests are made up - Ignore
        if (!i) {
            strcpy(session, sd.session_manager.GetSession(0).SessionId().String());
        }
    } // for(uint8_t i=0; ...)
    
    Serial.println("Done");
}

void loop() {
    Serial.available();
}
