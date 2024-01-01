/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - In this example a text content GET request is parsed
 * with the content base64 decoded.
 *
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * GET /page?show=1 HTTP/1.1
 * Content-Length: 116
 * Accept: application/json, text/javascript, *-*; q=0.01
 * Content-Type: text/html; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * VGhpcyBpcyByZXF1ZXN0IGRhdGEgdGhhdCBpcyBpbiBwbGFpbiB0ZXh0IGFuZCB0aGVuIGJhc2U2NCBhcHBsaWVkLCBjYW4geW91IHJlYWQgdGhpcz8=
 * Headers: 4
 *     Content-Length = 116
 *     Accept = application/json, text/javascript, *-*; q=0.01
 *     Accept-Encoding = gzip, deflate
 *     Accept-Language = en-GB,en-US;q=0.9,en;q=0.8
 * Raw Content(116):
 * VGhpcyBpcyByZXF1ZXN0IGRhdGEgdGhhdCBpcyBpbiBwbGFpbiB0ZXh0IGFuZCB0aGVuIGJhc2U2NCBhcHBsaWVkLCBjYW4geW91IHJlYWQgdGhpcz8=
 * Decoded:
 * This is request data that is in plain text and then base64 applied, can you read this?
 * url: '/page'
 * method: 'GET'
 * protocol: 'HTTP'
 * protocol_version: '1.1'
 * url_raw: '/page?show=1'
 * content type list: 2
 *     text/html = 
 *     charset = iso-8859-1
 * url variables: 1
 *     show = 1
 * Done
 * 
 * DG, 2020
 */


/* Adjust some parameters to suit our request since there is only 1 and it is small */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0    // Not using for this application
#define GHTTP_REQUEST_PROCESS_COOKIES 0            // Not using cookies for this application
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 12
#define GHTTP_REQUEST_MAX_VARS 6


// Include
#include "ghttp.h"


/*
 * Request/response buffer
 */
char buffer[464];

ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE> request;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // Request, assumed received over TCP/IP by some hardware interface
    strcpy_P(buffer, PSTR(
        "GET /page?show=1 HTTP/1.1\r\n" \
        "Content-Length: 116\r\n" \
        "Accept: application/json, text/javascript, */*; q=0.01\r\n" \
        "Content-Type: text/html; charset=iso-8859-1\r\n" \
        "Accept-Encoding: gzip, deflate\r\n" \
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
        "VGhpcyBpcyByZXF1ZXN0IGRhdGEgdGhhdCBpcyBpbiBwbGFpbiB0ZXh0IGFuZCB0aGVuIGJhc2U2NCBhcHBsaWVkLCBjYW4geW91IHJlYWQgdGhpcz8="));
    Serial.println(buffer);

    uint16_t ci = request.Parse(buffer, strlen(buffer), sizeof(buffer));
    if (request.ERR_NONE != request.GetLastError()) {
      Serial.print("Error: ");
      Serial.println(request.GetLastError(), DEC);
      return;
    }

    uint8_t i, c = request.header.count;
    Serial.print("Headers: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.header[i];

        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        Serial.println(h->value.Get());
    }
    
    if (NULL != &request.content) {
        Serial.print("Raw Content(");
        Serial.print(strlen(&request.content));
        Serial.print("):\n");
        Serial.print(&request.content);
      
        Serial.print("\nDecoded:\n");
        ghttp::MIMEBase64::Decode(&request.content, const_cast<char*>(&request.content), request.GetContentLength(), request.GetContentLength());
        Serial.println(&request.content);
    }else {
        Serial.println("No content");
        return;
    }
    Serial.print("url: '");
    Serial.print(&request.url);
    Serial.println("'");
    Serial.print("method: '");
    Serial.print(&request.method);
    Serial.println("'");
    Serial.print("protocol: '");
    Serial.print(&request.protocol);
    Serial.println("'");
    Serial.print("protocol_version: '");
    Serial.print(&request.protocol_version);
    Serial.println("'");
    Serial.print("url_raw: '");
    Serial.print(&request.url_raw);
    Serial.println("'");

    c = request.content_type.count;
    Serial.print("content type list: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.content_type[i];
	    
        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        Serial.println(h->value.Get());
        if (h->Get() == NULL) {
            Serial.println("Name NULL");
            return;
        }
	    
        if (h->value.Get() == NULL) {
            Serial.println("Value NULL");
            return;
        }
    }

    c = request.query_variable.count;
    Serial.print("url variables: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.query_variable[i];

        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        Serial.println(h->value.Get());
        if (h->Get() == NULL) {
            Serial.println("Name NULL");
            return;
        }

        if (h->value.Get() == NULL) {
            Serial.println("Value NULL");
            return;
        }
    }
    Serial.println("Done");
}

void loop() {
    Serial.available();
}