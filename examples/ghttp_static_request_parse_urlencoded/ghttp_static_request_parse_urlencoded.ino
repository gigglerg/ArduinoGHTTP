/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - In this example a x-www-form-urlencoded post request is parsed
 *
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * POST foo:bar@/something?callback=jQuery341015096386859381816_1573505324769&fly&stick&offset=&stop={%22request%22:{%22type%22:%22information%22}}&_=1573505324771 HTTP/1.1
 * Content-Length: 47
 * Accept: application/json, text/javascript, *-*; q=0.01
 * Origin: null
 * Cookie: name=value; HW_SESSION=1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk
 * Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
 * 
 * user=%22dave%22&wow&pass=%7Bhellodave%7D%5Cn%26
 * Headers: 5
 *     Content-Length = 47
 *     Accept = application/json, text/javascript, *-*; q=0.01
 *     Origin = null
 *     Accept-Encoding = gzip, deflate
 *     Accept-Language = en-GB,en-US;q=0.9,en;q=0.8
 * Cookies: 2
 *     name = value
 *     HW_SESSION = 1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk
 * Content(4):
 * usersizeof(request_instance) is 404
 * username: 'foo'
 * password: 'bar'
 * url: '/something'
 * method: 'POST'
 * protocol: 'HTTP'
 * protocol_version: '1.1'
 * url_raw: '/something?callback=jQuery341015096386859381816_1573505324769&fly&stick&offset=&stop={%22request%22:{%22type%22:%22information%22}}&_=1573505324771'
 * content type list: 2
 *     application/x-www-form-urlencoded = 
 *     charset = iso-8859-1
 * url variables: 6
 *     callback = jQuery341015096386859381816_1573505324769
 *     fly = 
 *     stick = 
 *     offset = 
 *     stop = {"request":{"type":"information"}}
 *     _ = 1573505324771
 * post variables: 3
 *     user = "dave"
 *     wow = 
 *     pass = {hellodave}\n&
 * Done
 * 
 * Dave.C, 2020
 */


/* Adjust some parameters to suit our request since there is only 1 and it is small */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 0 // Not using for this app
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 12
#define GHTTP_REQUEST_MAX_VARS 6
#define GHTTP_REQUEST_MAX_COOKIE 7


// Include
#include "ghttp.h"


/*
 * Request/response buffer
 */
char buffer[748];

ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_COOKIE> request;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // Request, assumed received over TCP/IP by some hardware interface
    strcpy_P(buffer, PSTR(
        "POST foo:bar@/something?callback=jQuery341015096386859381816_1573505324769&fly&stick&offset=&stop={%22request%22:{%22type%22:%22information%22}}&_=1573505324771 HTTP/1.1\r\n" \
        "Content-Length: 47\r\n" \
        "Accept: application/json, text/javascript, */*; q=0.01\r\n" \
        "Origin: null\r\n" \
        "Cookie: name=value; HW_SESSION=1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk\r\n" \
        "Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
        "Accept-Encoding: gzip, deflate\r\n" \
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
        "user=%22dave%22&wow&pass=%7Bhellodave%7D%5Cn%26"));

    Serial.println(buffer);

    uint16_t ci = request.Parse(buffer, strlen(buffer), sizeof(buffer), /*parse_content*/true, /*segmentation*/false);
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
    c = request.cookie.count;
    Serial.print("Cookies: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.cookie[i];

        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        Serial.println(h->value.Get());
    }
#if GHTTP_SERVER_USE_SESSIONS == 1
    if (NULL != &request.session) {
        Serial.print("SESSION is ");
        Serial.print(&request.session);
    }
#endif // GHTTP_SERVER_USE_SESSIONS == 1
    if (NULL != &request.content) {
        Serial.print("Content(");
        Serial.print(strlen(&request.content));
        Serial.print("):\n");
        Serial.print(&request.content);
    }else {
        Serial.println("No content");
        return;
    }
    Serial.print("sizeof(request_instance) is ");
    Serial.println(sizeof(request));
    Serial.print("username: '");
    Serial.print(&request.username);
    Serial.println("'");
    Serial.print("password: '");
    Serial.print(&request.password);
    Serial.println("'");
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

    c = request.post_variable.count;
    Serial.print("post variables: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.post_variable[i];

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

#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
    c = request.post_file_variable.count;
    Serial.print("post file variables: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.post_file_variable[i];
        const char* p = h->value.Get();

        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        for(uint16_t a = 0; a<h->filesize; a++) {
            if (p[a]<32) {
                if (p[a]<16) {
                    Serial.print('0');
                }
                Serial.print(p[a], HEX);
            }else {
                Serial.print(p[a]);
            }
        }

      Serial.print(" (");
      Serial.print(h->filesize, DEC);
      Serial.print(") as ");
      Serial.print(&h->filename);
      Serial.print(", type ");
      Serial.println(&h->mimetype);
      
      if (NULL == h->filename.Get()) {
          Serial.println("Filename NULL");
          return;
      }
      if (NULL == h->mimetype.Get()) {
          Serial.println("Mimetype NULL");
          return;
      }
    }
#endif // GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
    Serial.println("Done");
}

void loop() {
    Serial.available();
}