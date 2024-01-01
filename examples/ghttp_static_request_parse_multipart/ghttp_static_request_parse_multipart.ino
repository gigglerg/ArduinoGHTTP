/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - In this example a multipart post request is parsed
 *
 * Expected serial output (Arduino/Genuino Uno/Mega2560):
 * setup()
 * POST mrx:logmein@/process?msg={%22level%22:%221%22}} HTTP/1.1
 * Host: 10.0.0.2:8080
 * Connection: keep-alive
 * Content-Length: 599
 * Accept: text/html,text/javascript, *-*; q=0.01
 * Cookie: some=13; thing=57
 * User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)
 * Cookie: name=value; SESSION=NwwWpR4uxUrk,OKDunTX1EVo1cpC99FL-
 * Content-Type: multipart/form-data; boundary=----WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Accept-Encoding: gzip, deflate
 * Accept-Language: en-GB;q=0.9,en
 * 
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Content-Disposition: form-data; name="fwblob"; filename="send.txt"
 * Content-Type: text/plain
 * 
 * The quick brown fox jumped over the lazy dog.
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Content-Disposition: form-data; name="user"
 * 
 * unknown
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Content-Disposition: form-data; name="role"
 * 
 * picker
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Content-Disposition: form-data; name="dataset00"; filename="sendtxt.bin"
 * Content-Type: application/octet-stream
 * 
 * !@ABDEFGHIJKLM0
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4--
 * 
 * Headers: 7
 *     Host = 10.0.0.2
 *     Connection = keep-alive
 *     Content-Length = 599
 *     Accept = text/html,text/javascript, *-*; q=0.01
 *     User-Agent = Mozilla/5.0 (Windows NT 10.0; Win64; x64)
 *     Accept-Encoding = gzip, deflate
 *     Accept-Language = en-GB;q=0.9,en
 * Cookies: 4
 *     some = 13
 *     thing = 57
 *     name = value
 *     SESSION = NwwWpR4uxUrk,OKDunTX1EVo1cpC99FL-
 * SESSION is NwwWpR4uxUrk,OKDunTX1EVo1cpC99FL-Content(61):
 * ------WebKitFormBoundarys1vTnWL2y8YA6kR4
 * Content-Dispositionsizeof(request_instance) is 205
 * username: 'mrx'
 * password: 'logmein'
 * url: '/process'
 * method: 'POST'
 * protocol: 'HTTP'
 * protocol_version: '1.1'
 * url_raw: '/process?msg={%22level%22:%221%22}}'
 * content type list: 2
 *     multipart/form-data = 
 *     boundary = ----WebKitFormBoundarys1vTnWL2y8YA6kR4
 * url variables: 1
 *     msg = {"level":"1"}}
 * post variables: 2
 *     user = unknown
 *     role = picker
 * post file variables: 2
 *     fwblob = The quick brown fox jumped over the lazy dog. (45) as send.txt, type text/plain
 *     dataset00 = !@ABDEFGHIJKLM0 (15) as sendtxt.bin, type application/octet-stream
 * Done
 * 
 * DG, 2020
 */


/* Adjust some parameters to suit our request since there is only 1 and it is small */
#define GHTTP_SERVER_USE_SESSIONS 1
#define GHTTP_REQUEST_MAX_HEADERS 10
#define GHTTP_REQUEST_MAX_VARS 4
#define GHTTP_REQUEST_MAX_COOKIE 6


// Include
#include "ghttp.h"


/*
 * Request/response buffer
 */
char buffer[1132];

ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> request;

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("setup()");

    // Request, assumed received over TCP/IP by some hardware interface
    strcpy_P(buffer, PSTR(
        "POST mrx:logmein@/process?msg={%22level%22:%221%22}} HTTP/1.1\r\n" \
        "Host: 10.0.0.2:8080\r\n" \
        "Connection: keep-alive\r\n" \
        "Content-Length: 599\r\n" \
        "Accept: text/html,text/javascript, */*; q=0.01\r\n" \
        "Cookie: some=13; thing=57\r\n" \
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n" \
        "Cookie: name=value; SESSION=NwwWpR4uxUrk,OKDunTX1EVo1cpC99FL-\r\n" \
        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n" \
        "Accept-Encoding: gzip, deflate\r\n" \
        "Accept-Language: en-GB;q=0.9,en\r\n" \
        "\r\n" \
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n" \
        "Content-Disposition: form-data; name=\"fwblob\"; filename=\"send.txt\"\r\n" \
        "Content-Type: text/plain\r\n\r\n" \
        "The quick brown fox jumped over the lazy dog.\r\n" \
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n" \
        "Content-Disposition: form-data; name=\"user\"\r\n" \
        "\r\n" \
        "unknown\r\n" \
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n" \
        "Content-Disposition: form-data; name=\"role\"\r\n" \
        "\r\n" \
        "picker\r\n" \
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n" \
        "Content-Disposition: form-data; name=\"dataset00\"; filename=\"sendtxt.bin\"\r\n" \
        "Content-Type: application/octet-stream\r\n" \
        "\r\n" \
        "!@ABDEFGHIJKLM0\r\n" \
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4--\r\n"));
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
        auto h = request.header.At(i);

        Serial.print("\t");
        Serial.print(h->Get());
        Serial.print(" = ");
        Serial.println(h->value.Get());
    }
    c = request.cookie.count;
    Serial.print("Cookies: ");
    Serial.println(c, DEC);
    for(i=0; i<c; i++) {
        auto h = request.cookie.At(i);

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
        auto h = request.content_type.At(i);

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
        auto h = request.query_variable.At(i);

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
        auto h = request.post_variable.At(i);

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
        auto h = request.post_file_variable.At(i);
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