// libtest.cpp : Defines the entry point for the console application.
//

// tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define GHTTP_SESSIONMGR_INCLUDE_MAC 1
#define GHTTP_SERVER_ACCEPT_AUTHORIZATION 1
#define GHTTP_REQUEST_MAX_POST_FILE 2
#include "ghttp.h"

using namespace ghttp;

#define STOPLINE()        { printf("STOP at line %u\n\n", __LINE__); }

#pragma comment( lib, "winmm.lib") 

char test_session_cookie[] = "nXG-0EHYz0WIeNwkGL4emqRCVYYUEiiO";

namespace Ethernet {

char buffer[5*1024];

//#define FORM_URLENCODED
//#define TEST_REQUEST_FRAGMENTED_DECODE_V09
//#define TEST_REQUEST_FRAGMENTED_GET

void Restore(bool request=true) {
    memset(Ethernet::buffer, 0, sizeof(Ethernet::buffer));

    // WARNING: Don't buffer content unless you absolutely know you won't break a test.  If you do, make sure all tests are run before committing changes
    if (request) {
#if defined(FORM_URLENCODED)
#if 0
        sprintf(buffer, 
        "POST mrwow:testies@/something?callback=jQuery341015096386859381816_1573505324769&change&stick&offset=&stop={%%22request%%22:{%%22type%%22:%%22information%%22}}&_=1573505324771 HTTP/1.1\r\n"
        "Host: 10.0.0.2:8080\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 47\r\n"
        "Accept: application/json, text/javascript, */*; q=0.01\r\n"
        "Origin: null\r\n"
        "Cookie: xxx=4321; gggg=1234\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36\r\n"
        "Cookie: name=value; SESSION=%s; GIOSESSID=value3\r\n"
        "Content-Type: application/x-www-form-urlencoded; charset=UTF-8; boundary=----WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n"
        "user=%%22coxd%%22&wow&pass=%%7Bsmeggthis%%7D%%5Cn%%26", ::test_session_cookie);
#else
//        strcpy(buffer,
//        "POST /something?control=123 HTTP/1.1\r\n" \
//        "Content-Length: 28\r\n" \
//        "Accept: text/html, */*; q=0.01\r\n" \
//        "Origin: null\r\n" \
 //       "Cookie: name=value; SESSION=1KcJwq20VwFkmtUStgYw9-GerXwi-cLAk\r\n" \
//        "Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
//        "Accept-Encoding: gzip, deflate\r\n" \
//        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
//        "text=dave%20waz%20ere%202020" \
//        );
        strcpy(buffer,
#if 0
            "POST /something HTTP/1.1\r\n" \
            "Content-Length: 26\r\n" \
            "Accept: text/html, */*; q=0.01\r\n" \
            "Cookie: name=value\r\n" \
            "Content-Type: application/x-www-form-urlencoded; charset=iso-8859-1\r\n" \
            "Accept-Encoding: gzip, deflate\r\n" \
            "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n" \
            "ignore=Some%20text%20input");
#endif
"POST mrwow:testies@/login HTTP/1.1\r\n" \
"Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n" \
"Referer: http://10.0.0.10/login\r\n" \
"Accept-Language: en-GB,en;q=0.5\r\n" \
"User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n" \
"Content-Type: application/x-www-form-urlencoded\r\n" \
"Accept-Encoding: gzip, deflate\r\n" \
"Authorization: Basic Y294ZDpoZWxsb3dvcmxk\r\n" \
"Host: 10.0.0.10\r\n" \
"Content-Length: 39\r\n" \
"Connection: Keep-Alive\r\n" \
"Cache-Control: no-cache\r\n\r\n" \
"user=xxxx&pass=yyyy&submit=Submit+Query");
        printf("length = %d\n", strlen(buffer));
#endif
#else
        sprintf(buffer, 
#if defined(TEST_REQUEST_FRAGMENTED_DECODE_V09)
        "GET mrwow:testies@/something?callback=jQuery341015096386859381816_1573505324769&offset=&stop={%%22request%%22:{%%22type%%22:%%22information%%22}}&_=1573505324771 HTTP/0.9\r\n"
#else // !defined(TEST_REQUEST_FRAGMENTED_DECODE_V09)
#if !defined(TEST_REQUEST_FRAGMENTED_GET)
"POST / HTTP/1.1\r\n"
"Host: 10.0.0.10:4000\r\n"
"Connection: keep-alive\r\n"
"Content-Length: 1341\r\n"
"Cache-Control: max-age=0\r\n"
"Origin: http://10.0.0.10:4000\r\n"
"Upgrade-Insecure-Requests: 1\r\n"
"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryWpEblEFNoNHp6D6f\r\n"
"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
"Referer: http://10.0.0.10:4000/\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n"
"------WebKitFormBoundaryWpEblEFNoNHp6D6f\r\n"
"Content-Disposition: form-data; name=\"my_file\"; filename=\"test_chksum_21504.txt\"\r\n"
"Content-Type: text/plain\r\n\r\n"
"Set in Birmingham, England and produced by Ragdoll Productions, Brum was first broadcast in 1991 and the last live action series was broadcast in 2002. It was initially directed, written and produced by Anne Wood and initially narrated by Toyah Willcox. The show was first aired on BBC One, as well as airing on satellite and pay television where it was screened on TCC and Living, and also aired in the United States on Discovery Family, in Australia on ABC, Max, Nickelodeon, ABC Kids and ABC2, in Brunei on RTB, in Indonesia on antv, in Singapore on Kids Central, in South Africa on SABC2 (with both an English dub and an Afrikaans dub) and CBeebies, in Kenya on KBC, in New Zealand on TV3, in Namibia on NBC, in Zimbabwe on ZTV, in Ireland on RTE, in Germany on BFBS and SSVC Television, in The Netherlands on Nederland 3, in Bangladesh on BTV and in the Emirates on Dubai 33. It also aired on the now defunct children's channel, KPC 'Kids Pick Cartoons'. A new CGI-animated series, aimed at young pre-school children, was produced in 2016.\r\n"
"------WebKitFormBoundaryWpEblEFNoNHp6D6f\r\n"
"Content-Disposition: form-data; name=\"submit\"\r\n\r\n"
"Submit\r\n"
"------WebKitFormBoundaryWpEblEFNoNHp6D6f--\r\n"
#if 0
        "POST mrwow:testies@/something?callback=jQuery341015096386859381816_1573505324769&offset=&stop={%%22request%%22:{%%22type%%22:%%22information%%22}}&_=1573505324771 HTTP/1.1\r\n"
        "Host: 10.0.0.2:8080\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 704\r\n"
        "Accept: application/json, text/javascript, */*; q=0.01\r\n"
        "Origin: null\r\n"
        "Cookie: xxx=4321; gggg=1234\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36\r\n"
        "Cookie: name=value; SESSION=%s; GIOSESSID=value3\r\n"
        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n"
        "\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Content-Disposition: form-data; name=\"thefile2\"; filename=\"send.txt\"\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "The quick brown fox jumped over the lazy dog.\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Content-Disposition: form-data; name=\"user\"\r\n"
        "\r\n"
        "coxd\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Content-Disposition: form-data; name=\"pass\"\r\n"
        "\r\n"
        "helloworld\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Content-Disposition: form-data; name=\"testies\"\r\n"
        "\r\n"
        "fixedvalue\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4\r\n"
        "Content-Disposition: form-data; name=\"thefile\"; filename=\"sendtxt.bin\"\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n"
        "!@ABDEFGHIJKLM0\r\n"
        "------WebKitFormBoundarys1vTnWL2y8YA6kR4--\r\n"
#endif//0
#else // defined(TEST_REQUEST_FRAGMENTED_GET)
        "GET / HTTP/1.1\r\n"
        "Host: 10.0.0.10:4000\r\n"
        "Connection: keep-alive\r\n"
        "Cache-Control: max-age=0\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\n"
#endif // defined(TEST_REQUEST_FRAGMENTED_GET)
#endif // !defined(TEST_REQUEST_FRAGMENTED_DECODE_V09)
        , ::test_session_cookie);
#endif
    }else {
        // Response...
        sprintf(buffer, 
        "HTTP/1.0 404 Not Found\r\n"
        "Set-Cookie: SESSION=%s\r\n"
/*        "\r\n"*/
        , ::test_session_cookie);
    }
}

};


/**
 * Tester class for HTTPBuffer exposing more of the API for direct testing by making it public
 */
class HTTPBufferTester : public HTTPBuffer {
public:
    HTTPBufferTester(char* buffer, uint16_t max_length, uint16_t length=0, uint16_t offset=0) : HTTPBuffer(buffer, max_length, length, offset) { }

    using HTTPBuffer::FindEOL;
};


/**
 * An example session user data class and in this case a class representing application specific globals
 */
class Global {
public:
    /**
     * Logged in flag
     */
    bool    logged_in;


    /**
     * Some counter
     */
    uint8_t    count;

public:
    /**
     * Default constructor, make stable instance
     */
    Global() : logged_in(false), count(0) { }


    /**
     * Invoked by SessionManager to clear session data before \ref Start
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Clear(const uint8_t session_idx) {
        logged_in = false;
        count = 0;
        printf("global[%u].Clear()\n", session_idx);
    }


    /**
     * Invoked by SessionManager at start of new session.  \ref Clear would have been invoked first as objects are reused the constructor is only invoked once so you can 
     * consider this as a dynamic constructor invoke where you can alter globals before handling the initial request
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Start(const uint8_t session_idx) {
        printf("global[%u].Start()\n", session_idx);
    }


    /**
     * Invoked when related session is stale and no longer used when handling new requests or if it is about to be re-used by SessionManager with a \ref Clear and \ref Start
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Stop(const uint8_t session_idx) {
        printf("global[%u].Stop()\n", session_idx);
    }


    /**
     * Invoked by SessionManager upon next request using related session
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Continue(const uint8_t session_idx) {
        printf("global[%u].Continue()\n", session_idx);
    }
}; // class Global


/**
 * Application specific class holding everything required to process request/reponses.  PHP like super globals, here collected into a single class which can be passed 
 * around the system to keep encapsulation.   Add and remove variables that your application would want to use
 *
 * \note There is no set layout of this object however it would seem pretty obvious that a request and response instance would be required
 * \note Even if not using sessions you can put items in here that you need shared between your request/response implementation
 */
class Shared {
public:
#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
    SessionManager<10, Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> session_manager;
#endif
    HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> request;
    HTTPResponse<HTTPResponseStatus> response;
    struct {
        IP4_t    ip4;
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        MAC_t    mac;
#endif
    }client;
    struct {
        IP4_t    ip4;
    }server;
}; // class Shared


/**
 * An example implemented route for protocol HTTP covering "/" and GET + POST requests
 */
class RouteRoot : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url=="/" && s.request.protocol=="http") {
            if (s.request.method=="post") {
                return true;
            }else if (s.request.method=="get") {
                return true;
            }
        }
        
        return false;
    }
}; // class RouteRoot


/**
 * An example implemented route for protocol HTTP covering "/something" and POST requests
 */
class RouteSomething : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!="/something" || s.request.method!="post" || s.request.protocol!="http") {
            return false;
        }

        // Start session and get session variables
        printf("request session: '%s'\n", s.request.session.Get());
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 0
        auto &session = s.session_manager.StartSession(s.client.ip4, s.request.session.Get());
#else
        auto &session = s.session_manager.StartSession(s.client.ip4, s.client.mac, s.request.session.Get());
#endif
        auto &session_vars = session.user_data;
        printf("actual session: '%s'\n", session.SessionId().String());

        if (!session_vars.logged_in) {
            printf("User not logged in - doing now...\n");
            session_vars.logged_in = true;    // Pretend they have so 2nd time entry
        }else {
            printf("User already logged in!\n");
        }

        return true;
    }
}; // class RouteSomething


/**
 * An example implemented route for protocol HTTP covering "/something" and POST requests
 */
class RouteTest : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url=="/test" && s.request.method=="get" && s.request.protocol=="http") {
            return true;
        }

        return false;
    }
};


/**
 * An example implemented route to handle unhandled requests.  Here you might redirect the client or issue some other notice like Forbidden etc.
 */
class RouteUnhandled : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        // ...
        return true;
    }
};


/**
 * An example implemented route for protocol HTTP covering "/something" and POST requests
 */
class RouteSomethingEx : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!="/something" || s.request.method!="post" || s.request.protocol!="http") {
            return false;
        }

#if 0
        // Setup response
        s.response.SetMaximum(s.request.GetMaximum() - 200);
        s.response.SetBuffer(s.request.GetBuffer(), 0, 200);

        // Start session and get session variables
        printf("request session: '%s'\n", s.request.session.Get());
        auto &session = s.session_manager.StartSession(s.client.ip4, s.request.session.Get());
        auto &session_vars = session.user_data;
        printf("actual session: '%s'\n", session.SessionId().String());

        // Set response status
        s.response.status.Set(HTTPResponseStatus::STATUS_OK);
        s.response.Header("HTTP/1.1");

        // set session cookie
        // HTTPHelper::SessionCookieSet(...)

//        s.response.HeaderComplete(session.SessionId().String(), "HttpOnly")
#endif // 0
//        auto& session_vars = HTTPHelper::StartSession<Shared, Global>(s, 200, "HttpOnly");
        s.response.SetMaximum(s.request.GetMaximum());
        s.response.SetBuffer(s.request.GetBuffer(), 0, 100);
//        s.response.Header("Content-Ignore: yes");

        s.response.Header(T("HTTP/1.1"));
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+="<html><body><p>Anything to say? ";
        if (s.request.post_variable["text"]) {
//            s.response.Append(s.request.post_variable["text"]->value.Get());
            s.response+=s.request.post_variable["text"]->value;
        }else {
            s.response+="No";
        }
        s.response+=", page ";
        if (s.request.query_variable["page"]) {
//            s.response.Append(s.request.query_variable["page"]->value.Get());
            s.response+=s.request.query_variable["page"];
        }else {
            s.response+="0";
        }
        s.response+="</p></body></html>";
        s.response.Complete();

        // Do we need a helper for redirects?
//        HTTPHelper::Redirect(s, "/error");
//        HTTPHelper::Redirect(s, "/error", HTTPResponseStatus::STATUS_TEMPORARY_REDIRECT);
//        HTTPHelper::Redirect(s, "/error/msg", HTTPResponseStatus::STATUS_PERMANENT_REDIRECT, true, "HTTP/1.0");
//        HTTPHelper::Redirect(s, "http://www.example.com", HTTPResponseStatus::STATUS_TEMPORARY_REDIRECT);

        printf("response:\n%s\n", s.response.GetBuffer());
        
        return true;
    }
}; // class RouteSomethingEx


// Tests...
#define TEST_IP_MAC
#define TEST_STRING_HELPER_URI
#define TEST_STRING_HELPER_BASE64
#define TEST_RESPONSE_HEADER
#define TEST_RESPONSE_BUILD
#define TEST_STRINGHELPER
#define TEST_REQUEST_DECODE_SUCCESS
#define TEST_SESSION_ID_SET_AND_MATCH
#define TEST_SESSIONMANAGER_OVER_POPULATE
#define TEST_SESSION_INSTANCE_AND_USER_VAR
#define TEST_ROUTE_MANAGER
#define TEST_RND_SESSION_ID_GENERATION
#define TEST_COOKIEJAR_POPULATION_AND_LOOKUP
#define TEST_REQUEST_FIRST_LINE_DECODE
#define TEST_COMPLETE_ROUTE_MANAGER_REQUEST_RESPONSE
#define TEST_MIMEBASE64
#define TEST_BUFFER
#define TEST_REQUEST_FRAGMENTED_DECODE_SUCCESS

int _tmain(int argc, _TCHAR* argv[])
{
    uint32_t test = 0;

#if 0
    {
    /* Create a place to store our results */
    int result;

    /* Create two arrays to hold our data */
    char example1[50] = "C strncmp at TechOnTheNet.com";
    char example2[50] = "C strncmp is a string function";

    /* Compare the two strings provided up to 11 characters */
    result = strncmp(example2, example1, 11);

    /* If the two strings are the same say so */
    if (result == 0) printf("Strings are the same\n");

    printf("result %d\n", result);

    /* If the first string is less than the second say so
     (This is because the 'a' in the word 'at' is less than
     the 'i' in the word 'is' */
    if (result < 0)
        printf("Second string is less than the first\n");

    int16_t r2 = StringHelper::StrNCmp(example1, example2, 11);
    printf("hlp result %d\n", r2);
    }
#endif //0

#if defined(TEST_MIMEBASE64)
    {
        char raw[128] = "VGhpcyBpcyByZXF1ZXN0IGRhdGEgdGhhdCBpcyBpbiBwbGFpbiB0ZXh0IGFuZCB0aGVuIGJhc2U2NCBhcHBsaWVkLCBjYW4geW91IHJlYWQgdGhpcz8=";
        char raw_copy[128];
        char enc[128];

        printf("\nTesting[%u] - MIMEBase64::Decode(...)\n", test++);
        strcpy(raw_copy, raw);
        printf("Base64 encoded(%d): '%s'\n", strlen(raw), raw);
        MIMEBase64::Decode(raw, const_cast<char*>(raw), sizeof(raw), strlen(raw));
        printf("Base64 decoded(%d): '%s'\n", strlen(raw), raw);
        if (86 != strlen(raw)) {
            printf("Test failed.  Expected string length 86 != %d\n", strlen(raw));
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");

        printf("\nTesting[%u] - MIMEBase64::Encode(...)\n", test++);
        MIMEBase64::Encode(raw, const_cast<char*>(enc), sizeof(enc), strlen(raw));
        printf("Base64 encoded(%d): '%s'\n", strlen(enc), enc);
        if (strlen(raw_copy) != strlen(enc)) {
            printf("Test failed.  Expected string length %d != %d\n", strlen(raw), strlen(enc));
            STOPLINE();
            return 1;
        }
        if (0 != StringHelper::StrNCmp(raw_copy, enc)) {
            printf("Test failed.  String compare failed??\n", strlen(raw), strlen(enc));
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");
    }
#endif // defined(TEST_MIMEBASE64)

#if defined(TEST_STRINGHELPER)
    {
        printf("\nTesting[%u] - FindSubString(...) 1st substring, success\n", test++);

        char src[] = "This is a test can you find somethIng within this string";
        char sub1[] = "This";
        printf("Source:\n'%s'\nFind:\n'%s'\n", src, sub1);
        uint16_t i = StringHelper::FindSubString(src, sub1);
        if (i >= strlen(src)) {
            printf("Test failed.  Expected to find substring '%s'\n", sub1);
            STOPLINE();
            return 1;
        }
        printf("Found '%s' @ %u\n", sub1, i);
        uint16_t at = 0;
        if (i != at) { 
            printf("Test failed.  Find substring '%s' at %u, expected %u\n", sub1, i, at);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) 2nd substring, case insensitive, success\n", test++);
        i = StringHelper::FindSubString(src, sub1, 1, false);
        if (i >= strlen(src)) {
            printf("Test failed.  Expected to find substring '%s'\n", sub1);
            STOPLINE();
            return 1;
        }
        printf("Found '%s' @ %u\n", sub1, i);
        at = 45;
        if (i != at) {
            printf("Test failed.  Find substring '%s' at %u, expected %u\n", sub1, i, at);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) end of string, success\n", test++);
        char sub2[] = "ring";
        i = StringHelper::FindSubString(src, sub2);
        if (i >= strlen(src)) {
            printf("Test failed.  Expected to find substring '%s'\n", sub2);
            STOPLINE();
            return 1;
        }
        printf("Found '%s' @ %u\n", sub2, i);
        at = 52;
        if (i != at) {
            printf("Test failed.  Find substring '%s' at %u, expected %u\n", sub2, i, at);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        char sub3[] = "in";
        printf("\nTesting[%u] - FindSubString(...) iterate to end of string looking for '%s' (case insensitive), success\n", test++, sub3);
        i = 0;
        uint16_t c = 0;
        uint16_t fi1[] = {24, 34, 42, 53};
        while(i < strlen(src)) {
            i = StringHelper::FindSubString(src, sub3, i, false);
            if (i < strlen(src)) {
                printf("Found '%s' @ %u\n", sub3, i);
                if (c>=(sizeof(fi1) / sizeof(uint16_t))) {
                    printf("Test failed.  To many substrings found, max %u\n", i, sizeof(fi1) / sizeof(uint16_t));
                    STOPLINE();
                    return 1;
                }
                if (i != fi1[c]) {
                    printf("Test failed.  Find substring '%s' at %u, expected %u\n", sub3, i, fi1[c]);
                    STOPLINE();
                    return 1;
                }
                c++;
                i++;
            }
        }
        printf("Found %u substrings\n", c);
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) iterate to end of string looking for '%s', success\n", test++, sub3);
        i = 0;
        c = 0;
        uint16_t fi2[] = {24, 42, 53};
        while(i < strlen(src)) {
            i = StringHelper::FindSubString(src, sub3, i);
            if (i < strlen(src)) {
                printf("Found '%s' @ %u\n", sub3, i);
                if (c>=(sizeof(fi2) / sizeof(uint16_t))) {
                    printf("Test failed.  To many substrings found, max %u\n", i, sizeof(fi2) / sizeof(uint16_t));
                    STOPLINE();
                    return 1;
                }
                if (i != fi2[c]) {
                    printf("Test failed.  Find substring '%s' at %u, expected %u\n", sub3, i, fi2[c]);
                    STOPLINE();
                    return 1;
                }
                c++;
                i++;
            }
        }
        printf("Found %u substrings\n", c);
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) 2nd substring, fail\n", test++);
        i = StringHelper::FindSubString(src, sub1, 1);
        if (i >= strlen(src)) {
            // Not found - good
            printf("Not found '%s'\n", sub1);
        }else {
            printf("Test failed.  Expected to NOT find substring '%s' at %u\n", sub1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) substring null, fail\n", test++);
        i = StringHelper::FindSubString(src, NULL);
        if (i >= strlen(src)) {
            // Not found - good
            printf("Not found NULL substring\n");
        }else {
            printf("Test failed.  Expected to NOT find NULL substring at %u\n", i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) source string null, fail\n", test++);
        i = StringHelper::FindSubString(NULL, sub1, 1);
        if (i >= strlen(src)) {
            // Not found - good
            printf("Not found '%s'\n", sub1);
        }else {
            printf("Test failed.  Expected to NOT find substring '%s' at %u\n", sub1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindSubString(...) source string and substring null, fail\n", test++);
        i = StringHelper::FindSubString(NULL, NULL, 1);
        if (i >= strlen(src)) {
            // Not found - good
            printf("Not found NULL substring\n");
        }else {
            printf("Test failed.  Expected to NOT anything but have at %u\n", i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - StrNCmp(...) no length specified, success\n", test++);
        char s1[] = "Hello World";
        char s2[] = "Hello World";
        printf("s1 = '%s', s2 = '%s'\n", s1, s2);
        i = StringHelper::StrNCmp(s1, s2);
        if (0 == i) {
            printf("Strings equal\n");
        }else {
            printf("Test failed.  Expected strings '%s' equal, they are not (%u)\n", s1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        uint16_t l = strlen(s1);
        printf("\nTesting[%u] - StrNCmp(...) length=1 ... %u, success\n", test++, l);
        printf("s1 = '%s', s2 = '%s'\n", s1, s2);
        for(uint16_t ll = 1; ll<=l; ll++) {
            i = StringHelper::StrNCmp(s1, s2, ll);
            if (0 == i) {
                printf("Strings equal with length %u\n", ll);
            }else {
                printf("Test failed.  Expected strings '%s' equal, they are not (i = %u, length = %u)\n", s1, i, ll);
                STOPLINE();
                return 1;
            }
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - StrNCmp(...) no length specified, case insensitive, success\n", test++);
        char s3[] = "HELLO WorlD";
        printf("s1 = '%s', s3 = '%s'\n", s1, s3);
        i = StringHelper::StrNCmp(s1, s3, 0, false);
        if (0 == i) {
            printf("Strings equal\n");
        }else {
            printf("Test failed.  Expected strings '%s' equal, they are not (%u)\n", s1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - StrNCmp(...) no length specified, s1 != s4 with s1 diff > s4, fail\n", test++);
        char s4[] = "Hello 0orld";
        printf("s1 = '%s', s4 = '%s'\n", s1, s4);
        i = StringHelper::StrNCmp(s1, s4);
        if (1 == i) {
            printf("Strings NOT equal with s1 diff > s4\n");
        }else {
            printf("Test failed.  Expected strings '%s' equal, they are not (%u)\n", s1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - StrNCmp(...) no length specified, s1 != s5 with s1 diff > s5, fail\n", test++);
        char s5[] = "Hello Zorld";
        printf("s1 = '%s', s5 = '%s'\n", s1, s5);
        i = StringHelper::StrNCmp(s1, s5);
        if (65535 == i) {    // -1
            printf("Strings NOT equal with s1 diff < s5\n");
        }else {
            printf("Test failed.  Expected strings '%s' equal, they are not (%u)\n", s1, i);
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindTerminator(...) split, success\n", test++);
        char src_term1[] = "SOMETHING: value1=result1; value2=result2; value3=result3";
        printf("src:\n%s\n", src_term1);
        l = strlen(src_term1);
        i = 0;
        uint16_t ie1[] = { 9, 25, 41, 0xffff }, ie_c = 0;
        while(i < l) {
            uint16_t e = StringHelper::FindTerminator(i, src_term1, ":;", false, l);

            if (e < l) {
                if ((e-i)>0) {
                    printf("Term['%c' @ %u] = '", src_term1[e], e);
                    for(uint16_t u=i; u<e; u++) {
                        printf("%c", src_term1[u]);
                    }
                    printf("'\n");
                    if (0xffff == ie1[ie_c]) {
                        printf("Test failed.  Run out of terminators at %u.  Last i was %u\n", e, i);
                        STOPLINE();
                        return 1;
                    }
                    if (e != ie1[ie_c]) {
                        printf("Test failed.  Expected terminator at %u not %u\n", ie1[ie_c], e);
                        STOPLINE();
                        return 1;
                    }
                    ie_c++;
                }
            }
            i = e + 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindTerminator(...) split, success\n", test++);
        printf("src:\n%s\n", src_term1);
        l = strlen(src_term1);
        i = 0;
        ie_c = 0;
        uint16_t ie2[] = { 9, 17, 25, 33, 41, 49, 0xffff };
        while(i < l) {
            uint16_t e = StringHelper::FindTerminator(i, src_term1, ":;= ", false, l);

            if (e < l) {
                if ((e-i)>0) {
                    printf("Term['%c' @ %u] = '", src_term1[e], e);
                    for(uint16_t u=i; u<e; u++) {
                        printf("%c", src_term1[u]);
                    }
                    printf("'\n");
                    if (0xffff == ie2[ie_c]) {
                        printf("Test failed.  Run out of terminators at %u.  Last i was %u\n", e, i);
                        STOPLINE();
                        return 1;
                    }
                    if (e != ie2[ie_c]) {
                        printf("Test failed.  Expected terminator at %u not %u\n", ie2[ie_c], e);
                        STOPLINE();
                        return 1;
                    }
                    ie_c++;
                }
            }
            i = e + 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindTerminator(...) split, A...B... success\n", test++);
        char src_term2[] = "AAAAAAAAAAAAABBBBBBBBBBBBBBCCCCCCCCCCC";
        printf("src:\n%s\n", src_term2);
        l = strlen(src_term2);
        i = 0;
        uint16_t e = StringHelper::FindTerminator(i, src_term2, "AB", true);
        ie_c = 27;
        if (e < l && (e-i)>0) {
            printf("Term['%c' @ %u] = '", src_term2[e], e);
            for(uint16_t u=i; u<e; u++) {
                printf("%c", src_term2[u]);
            }
            printf("'\n");
            if (e != ie_c) {
                printf("Test failed.  Expected terminator at %u not %u\n", ie_c, e);
                STOPLINE();
                return 1;
            }
            i = e + 1;
        }else {
            printf("Test failed.  Expected terminator\n");
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindTerminator(...) split, C... success\n", test++);
        printf("src:\n%s\n", src_term2);
        e = StringHelper::FindTerminator(i, src_term2, "C", true);
        ie_c = 38;
        if (e == l && (e-i)>0) {
            printf("Term['0x%02x' @ %u] = '", (uint32_t)src_term2[e], e);
            for(uint16_t u=i; u<e; u++) {
                printf("%c", src_term2[u]);
            }
            printf("'\n");
            if (e != ie_c) {
                printf("Test failed.  Expected terminator at %u not %u\n", ie_c, e);
                STOPLINE();
                return 1;
            }
            i = e + 1;
        }else {
            printf("Test failed.  Expected terminator\n");
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FindTerminator(...) split, no terminator\n", test++);
        printf("src:\n%s\n", src_term2);
        i = 0;
        e = StringHelper::FindTerminator(i, src_term2, " .", true);    // Terminator doesn't exist
        if (e < l && (e-i)>0) {
            printf("Term['%c' @ %u] = '", src_term2[e], e);
            for(uint16_t u=i; u<e; u++) {
                printf("%c", src_term2[u]);
            }
            printf("'\n");
            printf("Test failed.  Expected NO terminator\n", ie_c, e);
            STOPLINE();
            return 1;
        }else {
            printf("No terminator\n");
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - Reverse(...)\n", test++);
        char r_src[] = "Does this string read OK in reverse?";
        char c_cmp[] = "Does this string read OK in reverse?";
        uint16_t c_l = strlen(c_cmp);
        printf("src:\n%s\n", r_src);
        StringHelper::Reverse(r_src);
        printf("rev:\n%s\n", r_src);
        for(uint16_t i=0; i<c_l; i++) {
            if (r_src[i] == c_cmp[c_l-(1+i)]) {
            }else {
                printf("Test failed.  String not reversed??\n");
                STOPLINE();
                return 1;
            }
        }
        printf("rev:\n%s\n", StringHelper::Reverse(r_src));
        if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
            printf("Test failed.  Double reversed string doesn't match source\n");
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FromInt(...), base 10, int32_t\n", test++);
        int32_t pset[] = {-1, -2, -3, -100, -1000, -65536, 0, 1, 2, 3, 100, 1000, 65536 };
        uint16_t pset_l = sizeof(pset) / sizeof(int32_t);
        for(uint16_t i=0; i< pset_l; i++) {
            _itoa(pset[i], c_cmp, 10);
            uint8_t ll = StringHelper::FromInt<int32_t>(pset[i], r_src, 10);
            if (ll != strlen(r_src)) {
                printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                STOPLINE();
                return 1;
            }
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FromInt(...), base 10, int8_t n\n", test++);
        for(int8_t i=-127; ; i++) { // NOTE: not -128 due to known limitation of sign change internally
            _itoa(i, c_cmp, 10);
            uint8_t ll = StringHelper::FromInt<int8_t>(i, r_src, 10);
            if (ll != strlen(r_src)) {
                printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                STOPLINE();
                return 1;
            }
            if (i == 127) {
                break;
            }
        }
        printf("Test passed\n\n");


        printf("\nTesting[%u] - FromInt(...), base 10, int16_t n\n", test++);
        for(int16_t i=-32767; ; i++) { // NOTE: not -32768 due to known limitation of sign change internally
            _itoa(i, c_cmp, 10);
            uint8_t ll = StringHelper::FromInt<int16_t>(i, r_src, 10);
            if (ll != strlen(r_src)) {
                printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                STOPLINE();
                return 1;
            }
            if (i == 32767) {
                break;
            }
        }
        printf("Test passed\n\n");


        for(uint8_t b=10; b<=16; b+=6) {
            printf("\nTesting[%u] - FromUInt(...), base %u, uint32_t\n", test++, b);
            for(uint16_t i=(b==16?0:(pset_l>>1)); i< pset_l; i++) {  // +n only for base10 due to fact _itoa(...) takes a 32bit value so doesn't work as well as the StringHelper methods
                _itoa((uint32_t)pset[i], c_cmp, b);
                uint8_t ll = StringHelper::FromUInt<uint32_t>(pset[i], r_src, b);
                if (ll != strlen(r_src)) {
                    printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                    STOPLINE();
                    return 1;
                }
                if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                    printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                    STOPLINE();
                    return 1;
                }
            }
            printf("Test passed\n\n");


            printf("\nTesting[%u] - FromUInt(...), base %u, uint8_t\n", test++, b);
            for(uint8_t i=0; ; i++) {
                _itoa(i, c_cmp, b);
                uint8_t ll = StringHelper::FromUInt<uint8_t>(i, r_src, b);
                if (ll != strlen(r_src)) {
                    printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                    STOPLINE();
                    return 1;
                }
                if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                    printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                    STOPLINE();
                    return 1;
                }
                if (i == 255) {
                    break;
                }
            }
            printf("Test passed\n\n");


            printf("\nTesting[%u] - FromUInt(...), base %u, uint16_t n\n", test++, b);
            for(uint16_t i=0; ; i++) {
                _itoa(i, c_cmp, b);
                uint8_t ll = StringHelper::FromUInt<uint16_t>(i, r_src, b);
                if (ll != strlen(r_src)) {
                    printf("Test failed.  Length of Value '%s' (%u) doesn't match '%s' (%u) (i=%d)\n", r_src, ll, c_cmp, strlen(c_cmp), i);
                    STOPLINE();
                    return 1;
                }
                if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                    printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                    STOPLINE();
                    return 1;
                }
                if (i == 65535) {
                    break;
                }
            }
            printf("Test passed\n\n");
        } // for(uint8_t b=10...)


        printf("\nTesting[%u] - FromDecimal(...), float n\n", test++);
        float f_set[] = {-1003.4f, -102.3f, -10.1f, 0.0f, 10.1f, 102.3f, 1003.4f };
        for(uint8_t i=0; i<(sizeof(f_set) / sizeof(float)); i++) {
            sprintf(c_cmp, "%.3f", f_set[i]);
            uint8_t ll = StringHelper::FromDecimal<float>(f_set[i], r_src, 3);
            if (ll != strlen(r_src)) {
                printf("Test failed.  Length of Value '%s' (%u!=%u) doesn't match '%s' (i=%d)\n", r_src, ll, strlen(r_src), c_cmp, i);
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                STOPLINE();
                return 1;
            }
        }
        printf("Test passed\n\n");

        printf("\nTesting[%u] - FromDecimal(...), double n\n", test++);
        for(uint8_t i=0; i<(sizeof(f_set) / sizeof(float)); i++) {
            sprintf(c_cmp, "%.4f", (double)f_set[i]);    // %g??
            uint8_t ll = StringHelper::FromDecimal<double>(static_cast<double>(f_set[i]), r_src, 4);
            if (ll != strlen(r_src)) {
                printf("Test failed.  Length of Value '%s' (%u!=%u) doesn't match '%s' (i=%d)\n", r_src, ll, strlen(r_src), c_cmp, i);
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(r_src, c_cmp)) {
                printf("Test failed.  Value '%s' doesn't match '%s' (i=%d)\n", r_src, c_cmp, i);
                STOPLINE();
                return 1;
            }
        }
        printf("Test passed\n\n");

    }
#endif // defined(TEST_STRINGHELPER)


#if defined(TEST_IP_MAC)
    {
        MAC_t m;

        printf("\nTesting[%u] - MAC ADT set from strings and uints\n", test++);
        {
            m = "0123456789ab";
            printf("MAC: %02x%02x%02x%02x%02x%02x\n", m.u8[0], m.u8[1], m.u8[2], m.u8[3], m.u8[4], m.u8[5]);
            uint8_t vm[6] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab };
            for(uint8_t n=0; n<sizeof(vm); n++) {
                if (vm[n] != m.u8[n]) {
                    printf("Test failed\n");
                    STOPLINE();
                    return 1;
                }
            }
        }

        {
            m = "ab015689fecd";
            char s1[16], s2[16] = "\0";
            sprintf(s1, "%02x%02x%02x%02x%02x%02x", m.u8[0], m.u8[1], m.u8[2], m.u8[3], m.u8[4], m.u8[5]);
            printf("MAC: %s\n", s1);
            if (0x01ab != m.u16[0] || 0x8956 != m.u16[1] || 0xcdfe != m.u16[2]) {
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            uint8_t ll = m.String(s2);
            printf("s2 : %s\n", s2);
            if (ll != 12) {
                printf("Test failed.  Length != 12\n");
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(s1, s2)) {
                printf("Test failed, string compare %s != %s\n", s1, s2);
                STOPLINE();
                return 1;
            }
        }


        {
            m = "000000000000";
            char s1[16], s2[16] = "\0";
            sprintf(s1, "%02x%02x%02x%02x%02x%02x", m.u8[0], m.u8[1], m.u8[2], m.u8[3], m.u8[4], m.u8[5]);
            printf("MAC: %s\n", s1);
            if (0x0 != m.u16[0] || 0x0 != m.u16[1] || 0x0 != m.u16[2]) {
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            uint8_t ll = m.String(s2);
            printf("s2 : %s\n", s2);
            if (ll != 12) {
                printf("Test failed.  Length != 12\n");
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(s1, s2)) {
                printf("Test failed, string compare %s != %s\n", s1, s2);
                STOPLINE();
                return 1;
            }
        }

        {
            m = "ffffffffffff";
            char s1[16], s2[16] = "\0";
            sprintf(s1, "%02x%02x%02x%02x%02x%02x", m.u8[0], m.u8[1], m.u8[2], m.u8[3], m.u8[4], m.u8[5]);
            printf("MAC: %s\n", s1);
            if (0xffff != m.u16[0] || 0xffff != m.u16[1] || 0xffff != m.u16[2]) {
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            uint8_t ll = m.String(s2);
            printf("s2 : %s\n", s2);
            if (ll != 12) {
                printf("Test failed.  Length != 12\n");
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(s1, s2)) {
                printf("Test failed, string compare %s != %s\n", s1, s2);
                STOPLINE();
                return 1;
            }
        }

        {
            m.u8[0] = 0x01;
            m.u8[1] = 0xff;
            m.u8[2] = 0x02;
            m.u8[3] = 0xef;
            m.u8[4] = 0x03;
            m.u8[5] = 0xfd;
            char s1[16], s2[16] = "\0";
            sprintf(s1, "%02x%02x%02x%02x%02x%02x", m.u8[0], m.u8[1], m.u8[2], m.u8[3], m.u8[4], m.u8[5]);
            printf("MAC: %s\n", s1);
            if (0xff01 != m.u16[0] || 0xef02 != m.u16[1] || 0xfd03 != m.u16[2]) {
                printf("%04x %04x %04x\n", m.u16[0], m.u16[1], m.u16[2]);
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            uint8_t ll = m.String(s2);
            printf("s2 : %s\n", s2);
            if (ll != 12) {
                printf("Test failed.  Length != 12\n");
                STOPLINE();
                return 1;
            }
            if (0 != StringHelper::StrNCmp(s1, s2)) {
                printf("Test failed, string compare %s != %s\n", s1, s2);
                STOPLINE();
                return 1;
            }
        }

        IP4_t i;

        printf("\nTesting[%u] - IP ADT set from strings and uints\n", test++);

        i = "1.2.3.4";
        printf("IP: %u.%u.%u.%u\n", i.u8[0], i.u8[1], i.u8[2], i.u8[3]);
        if (67305985 != i.u32) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        i = "192.168.0.1";
        printf("IP: %u.%u.%u.%u\n", i.u8[0], i.u8[1], i.u8[2], i.u8[3]);
        if (16820416 != i.u32) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        i = "10.0.0.1";
        printf("IP: %u.%u.%u.%u\n", i.u8[0], i.u8[1], i.u8[2], i.u8[3]);
        if (16777226 != i.u32) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        char ip4_str[20];
        memset(ip4_str, 65, 20);
        uint8_t l = i.String(ip4_str);
        printf("str: %s (length %u)\n", ip4_str, l);
        if (l != 8) {
            printf("Test failed.  Length != 8\n");
            STOPLINE();
            return 1;
        }
        if (StringHelper::StrNCmp(ip4_str, "10.0.0.1") != 0) {
            printf("Test failed.  IP4 string not as expected \"10.0.0.1\"\n");
            STOPLINE();
            return 1;
        }
        for(uint8_t x=17; x<20; x++) {
            if (ip4_str[x] != 65) {
                printf("Test failed.  IP4 string convert overflow at index %u\n", x);
                STOPLINE();
                return 1;
            }
        }

        i.u8[0] = 100;
        i.u8[1] = 101;
        i.u8[2] = 255;
        i.u8[3] = 202;
        l = i.String(ip4_str);
        printf("str: %s (length %u)\n", ip4_str, l);
        if (l != 15) {
            printf("Test failed.  Length != 15\n");
            STOPLINE();
            return 1;
        }
        if (StringHelper::StrNCmp(ip4_str, "100.101.255.202") != 0) {
            printf("Test failed.  IP4 string not as expected \"100.101.255.202\"\n");
            STOPLINE();
            return 1;
        }
        for(uint8_t x=17; x<20; x++) {
            if (ip4_str[x] != 65) {
                printf("Test failed.  IP4 string convert overflow at index %u\n", x);
                STOPLINE();
                return 1;
            }
        }

        printf("Test passed\n\n");
    }
#endif // defined(TEST_IP_MAC)


#if defined(TEST_RND_SESSION_ID_GENERATION)
    {
        uint32_t dupcount = 0;
        char csid[1024][GHTTP_SESSIONID_LENGTH];
        printf("\nTesting[%u] - Session ID random generation\n", test++);

        for(uint32_t h=0; h<1024; h++)
        {
            const char* s = GHTTP_SESSIONRND_GENERATE(GHTTP_SESSIONID_LENGTH);

            printf("Session[%u] = '%s' ",h, s);
            strncpy(&csid[h][0], s, GHTTP_SESSIONID_LENGTH);
            csid[h][GHTTP_SESSIONID_LENGTH-1] = '\0';

            bool d=false;
            for(uint32_t u=0; u<h; u++) {
                if (0==strcmp(&csid[h][0], &csid[u][0])) {
                    if (!d) {
                        printf("duplicate ");
                        d=true;
                    }else {
                        printf(", ");
                    }
                    printf("%u", u);
                    dupcount++;
                }
            }
            printf("\n");
        }
        printf("Duplicate count = %u\n", dupcount);
        printf("Test passed\n\n");
    }
#endif // defined(TEST_RND_SESSION_ID_GENERATION)


#if defined(TEST_SESSION_ID_SET_AND_MATCH)
    {
        SessionId</*SESSIONID_LENGTH*/6> id[2]; /* 6 because that is my test strings */

        printf("\nTesting[%u] - Session ID set and match\n", test++);

        // Set same
        id[0] = "hello.";
        id[1] = "hello.";

        if (id[0] == id[1]) {
            printf("id[0] and id[1] are the same\n");
        }else {
            printf("id[0] and id[1] are the different\n");
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        // Set different, including length and case
        id[0] = "IGNORE";
        if (id[0] == id[1]) {
            printf("id[0] and id[1] are the same\n");
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }else {
            printf("id[0] and id[1] are the different\n");
        }

        // Set different, case only
        id[1] = "ignore";
        if (id[0] == id[1]) {
            printf("id[0] and id[1] are the same\n");
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }else {
            printf("id[0] and id[1] are the different\n");
        }

        printf("Test passed\n\n");
    }
#endif // defined(TEST_SESSION_ID_SET_AND_MATCH)


#if defined(TEST_SESSION_INSTANCE_AND_USER_VAR)
    {
        printf("\nTesting[%u] - Create a session instance and generate a session id for it.  Then change a user session variable\n", test++);

        Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME> ss;

        // Generate SID for session ss
        auto &sid = ss.SessionId();
        sid.Generate();

        printf("sid = '%s' (%p)\n", sid.String(), &sid);
        printf("logged in? %u\n", ss.user_data.logged_in);

        ss.user_data.logged_in = true;

        printf("logged in? %u\n", ss.user_data.logged_in);

        printf("Test passed\n\n");
    }
#endif // defined(TEST_SESSION_INSTANCE_AND_USER_VAR)


#if defined(TEST_SESSIONMANAGER_OVER_POPULATE)
    {
        printf("\nTesting[%u] - Session manager<4> with 5 sessions (over populate)\n", test++);
        SessionManager<4, Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> sm;

        // temporary store for SIDs for replay as if came from repeated web requests
        char csid[5][GHTTP_SESSIONID_LENGTH];

        for(uint8_t i=0; i<5; i++) {
            IP4_t ip4;
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
            MAC_t mac;
            auto &mys = sm.StartSession(ip4, mac);
#else // GHTTP_SESSIONMGR_INCLUDE_MAC != 1
            auto &mys = sm.StartSession(ip4);
#endif

            printf("New session id = '%s'\n", mys.SessionId().String());
            strncpy(&csid[i][0], mys.SessionId().String(), GHTTP_SESSIONID_LENGTH);

            printf("Userdata %p\n", &mys.user_data.count);
            mys.user_data.count = (i + 1) * 10;
        }

        printf("Reuse...\n");
        // Start with youngest
        for(int8_t i=4; i>=0; i--) {
            IP4_t ip4;
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
            MAC_t mac;
            auto &mys = sm.StartSession(ip4, mac, &csid[i][0]);
#else // GHTTP_SESSIONMGR_INCLUDE_MAC != 1
            auto &mys = sm.StartSession(ip4, &csid[i][0]);
#endif

            printf("Userdata %p\n", &mys.user_data.count);
            printf("Session id = '%s', user value = %u\n", mys.SessionId().String(), mys.user_data.count);

            while(!sm.Service());
        }
        printf("Test passed\n\n");
    }
#endif // defined(TEST_SESSIONMANAGER_OVER_POPULATE)


#if defined(TEST_COOKIEJAR_POPULATION_AND_LOOKUP)
    {
        Ethernet::Restore();

        printf("\nTesting[%u] - Cookie Jar population and lookup\n", test++);
        HTTPRequestCookieJar<8> cj;
        {
            // Set some hardcoded cookies from known positions within test request receive buffer Ethernet::buffer[]
            // This information would come from HTTPRequest.Parse(...) but we arn't checking that here
            struct {
                uint16_t i;
                uint16_t l;
            }hardcoded_cookie_positions[] = {
#if defined(FORM_URLENCODED)
                { 321, 3 },        // set cookie name i=321, len 3
                { 325, 4 },        // set cookie value i=325, len 4
                { 331, 4 },        // set cookie name i=331, len 4
                { 336, 4 },        // set cookie value i=336, len 4
                { 478, 4 },        // set cookie name i=478, len 4
                { 483, 5 },        // set cookie value i=483, len 5
                { 490, 7 },        // set cookie name i=490, len 7
                { 498, 32 },    // set cookie value i=498, len 32
                { 532, 9 },        // set cookie name i=532, len 9
                { 542, 6 },        // set cookie value i=542, len 6
#else // !defined(FORM_URLENCODED)
                { 311, 3 },        // set cookie name i=311, len 3
                { 315, 4 },        // set cookie value i=315, len 4
                { 321, 4 },        // set cookie name i=321, len 4
                { 326, 4 },        // set cookie value i=326, len 4
                { 468, 4 },        // set cookie name i=468, len 4
                { 473, 5 },        // set cookie value i=473, len 5
                { 480, 7 },        // set cookie name i=480, len 7
                { 488, 32 },    // set cookie value i=488, len 32
                { 522, 9 },        // set cookie name i=522, len 9
                { 532, 6 },        // set cookie value i=532, len 6
#endif // !defined(FORM_URLENCODED)
            };
            uint16_t cookie_set_lines = sizeof(hardcoded_cookie_positions) / (sizeof(uint16_t)*2);

            // Build cookie set
            for(uint32_t i=0; i<cookie_set_lines; i++) {
                HTTPAttrValueString *hh = cj.At(cj.count);
                if (!(i % 2)) {
                    // name
                    hh->Set(Ethernet::buffer, hardcoded_cookie_positions[i].i, hardcoded_cookie_positions[i].l);
                }else {
                    // value
                    hh->value.Set(Ethernet::buffer, hardcoded_cookie_positions[i].i, hardcoded_cookie_positions[i].l);
                    cj.count++;
                }
            }

            // Expected amount of cookies?
            if ((cookie_set_lines>>1) != cj.count) {
                printf("Test failed.  Cookie lines %u/2 != cookie %u\n", cookie_set_lines, cj.count);
                STOPLINE();
                return 1;
            }
        }

        // Output cookies with values
        for(uint8_t i=0;i<5;i++) {
            HTTPAttrValueString *h = cj.At(i);
            printf("%p cookie[%u]: '%s'='%s'\n", h, i, h->Get(), h->value.Get());
        }

        // Search cookies for strings, some values others cookie names
        {
            int16_t ix = cj.Find("1234");

            printf("Find 1234 (attr) = %d\n", (int)ix);
            if (ix>0) {
                printf("Found: '%s'\n", cj.At((uint8_t)ix)->Get());
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }else {
                printf("Not found\n");
            }

            ix = cj.Find("1234", true);
            printf("Find 1234 (value) = %d\n", (int)ix);
            if (ix>0) {
                printf("Found: '%s'\n", cj.At((uint8_t)ix)->Get());
            }else {
                printf("Not found\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            ix = cj.Find("name");
            printf("Find name (attr) = %d\n", (int)ix);
            if (ix>0) {
                printf("Found: '%s'\n", cj.At((uint8_t)ix)->value.Get());
            }else {
                printf("Not found\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            ix = cj.Find("name", true);
            printf("Find name (value) = %d\n", (int)ix);
            if (ix>0) {
                printf("Found: '%s'\n", cj.At((uint8_t)ix)->Get());
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }else {
                printf("Not found\n");
            }
        }
        printf("Test passed\n\n");
    }
#endif // defined(TEST_COOKIEJAR_POPULATION_AND_LOOKUP)

#if defined(TEST_REQUEST_FIRST_LINE_DECODE)
    {
        printf("\nTesting[%u] - HTTPRequest substring decode for first line (so substring testing)\n", test++);
        SessionManager<2, Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> sm;
        HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> r;

        sprintf(Ethernet::buffer, "POST / HTTP/1.1");

        r.method.Set(Ethernet::buffer, 0, 4);
        r.url.Set(Ethernet::buffer, 5, 1);
        r.protocol.Set(Ethernet::buffer, 7, 4);
        r.protocol_version.Set(Ethernet::buffer, 12, 3);

        printf("method  : '%s'\n", &r.method);
        // not case sensitive
        if (r.method != "POST") {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        if (r.method != "post") {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        printf("url     : '%s'\n", &r.url);
        if (r.url != "/") {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        printf("protocol: '%s'\n", &r.protocol);
        if (r.protocol != "HTTP") {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        printf("version : '%s'\n", &r.protocol_version);
        if (r.protocol_version != "1.1") {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        printf("Test passed\n\n");
    }
#endif // defined(TEST_REQUEST_FIRST_LINE_DECODE)


#if defined(TEST_REQUEST_DECODE_SUCCESS)
    {
        Ethernet::Restore();

        printf("%s\n", Ethernet::buffer);

        printf("\nTesting[%u] - HTTP request decode(success)\n", test++);
        SessionManager<10, Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> sm;
        HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> r;

        uint16_t ci = r.Parse(Ethernet::buffer, strlen(Ethernet::buffer), sizeof(Ethernet::buffer), true, false);
        if (r.ERR_NONE != r.GetLastError()) {
            printf("Error: %u\n", r.GetLastError());
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

/*
        if (!r.header["Authorization"]) {
            printf("Authorization doesnt exist??\n");
            STOPLINE();
            return 1;
        }
*/

        uint8_t i, c = r.header.count;
        printf("Headers: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.header.At(i);

            printf("\t%s: %s\n", h->Get(), h->value.Get());
        }
/*        if (8!=c) {
            printf("Expected 8 headers\n");
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }*/
        c = r.cookie.count;
        printf("Cookies: %u\n", c);
/*        if (5!=c) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }*/
        for(i=0; i<c; i++) {
            auto h = r.cookie.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
        }
        if (NULL != &r.session) {
            printf("SESSION is %s\n", &r.session);
        }
        if (NULL != &r.content) {
            printf("Content(%u):\n%s\n", strlen(&r.content), &r.content);
        }else {
            printf("No content\n");
            //printf("Test failed\n");
            //STOPLINE();
            //return 1;
        }
        printf("sizeof(sessionmgr_instance) is %u\n", sizeof(sm));
        printf("sizeof(request_instance) is %u\n", sizeof(r));
        printf("username: '%s'\n", r.username);
        printf("password: '%s'\n", r.password);
        printf("authorization username: '%s'\n", r.authorization_username);
        printf("authorization password: '%s'\n", r.authorization_password);
        printf("url: '%s'\n", r.url);
        if (r.url == "/login") {
            printf("route valid\n");
        }else {
            printf("route NOT valid\n");
        }
        printf("method: '%s'\n", r.method);
        printf("protocol: '%s'\n", r.protocol);
        printf("protocol_version: '%s'\n", r.protocol_version);
        printf("url_raw: '%s'\n", r.url_raw);

        c = r.content_type.count;
        printf("content type list: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.content_type.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

        c = r.query_variable.count;
        printf("url variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.query_variable.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
        c = r.post_variable.count;
        printf("post variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.post_variable.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

        c = r.post_file_variable.count;
        printf("post file variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.post_file_variable.At(i);
            const char* p = h->value.Get();

            printf("\t%s = ", h->Get());
            for(uint16_t a = 0; a<h->filesize; a++) {
                if (p[a]<32) {
                    printf("%02x", p[a]);
                }else {
                    printf("%c", p[a]);
                }
            }

            printf(" (%u) as %s, type %s\n", h->filesize, h->filename, h->mimetype);
            if (NULL == h->filename.Get()) {
                printf("Filename NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            if (NULL == h->mimetype.Get()) {
                printf("Mimetype NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1
        printf("Test passed\n\n");
    }
#endif // defined(TEST_REQUEST_DECODE_SUCCESS)


#if defined(TEST_ROUTE_MANAGER)
    {
        printf("\nTesting[%u] - Route Manager, HTTP request with optional sessions\n", test++);
        Shared sd;

        sd.client.ip4 = "10.0.0.15";
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        sd.client.mac = "123456789abc";
#endif

        RouteManager<10, Route<Shared>, Shared>  rm(&sd);    // Maximum of 10 routes in this example

        // Create our routes handled by this app (these would be globals in practice)
        RouteRoot route_root;
        RouteSomething route_something;
        RouteTest route_test;
        RouteUnhandled route_unhandled;

        // Register them
        int16_t r = rm.Register(&route_root);
        if (-1 == r) {
            printf("Test failed.  Route manager register of root failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route root at index %d\n", r);
        int16_t r_something = rm.Register(&route_something);
        if (-1 == r_something) {
            printf("Test failed.  Route manager register of something failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route something at index %d\n", r_something);
        r = rm.Register(&route_test);
        if (-1 == r) {
            printf("Test failed.  Route manager register of test failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route test at index %d\n", r);

#if 0 // We can't have default if we want to capture the fact no route might have been taken
        // Default is last route
        r = rm.Register(&route_unhandled);
        if (-1 == r) {
            printf("Test failed.  Route manager register of default failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route default at index %d\n", r);
#endif // 0

        // This would be our request receive and parse main loop
        int loop = 0;
        do {
            printf("Loop: %d\n", loop);

            Ethernet::Restore();
            // Parse our request...
            uint16_t ci = sd.request.Parse(Ethernet::buffer, strlen(Ethernet::buffer), sizeof(Ethernet::buffer));
            if (sd.request.ERR_NONE != sd.request.GetLastError()) {
                printf("Error: %u\n", sd.request.GetLastError());
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            int16_t route_ix = rm.Service();
            if (r_something != route_ix) {
                printf("Test failed.  No route or wrong route handled request (%d != %d)\n", r_something, route_ix);
                STOPLINE();
                return 1;
            }

            sd.session_manager.Service();

            if (loop++>0) {
                // We should be done after the 2nd iteration gets here...
                break;
            }

            // Assumed session 0, take session id string and update request so next time around we re-use the session
            strcpy(test_session_cookie, sd.session_manager.GetSession(0).SessionId().String());
        }while(1);

        printf("Test passed\n\n");
    }
#endif // defined(TEST_ROUTE_MANAGER)


#if defined(TEST_STRING_HELPER_URI)
    {
        char strA[] = "To%20infinity%20and%20beyond%21";
        char strB[] = "To%20infinity%20and%20beyond%21";    // For print purposes only

        printf("I: '%s', O: '%s'\n", strB, URI::Decode(strA));

        char strC[] = "http://www.foo.com/test?a=45&b=46#something";
        char strD[] = "http%3A%2F%2Fwww.foo.com%2Ftest%3Fa%3D45%26b%3D46%23something";
        char strE[sizeof(strC)*2] = {0};
        URI::Encode(strE, sizeof(strE), strC);
        printf("I: '%s' (%d), O: '%s' (%d)\n", strC, strlen(strC), strE, strlen(strE));
        if (0!=strcmp(strD, strE)) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        printf("I: '%s' (%d)", strE, strlen(strE));
        uint16_t stre_len = strlen(strE);
        URI::Decode(strE);
        printf(", O: '%s' (%d)\n", strE, strlen(strE));
        if (0!=strcmp(strC, strE)) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }
        if (URI::GetEncodeLength(strC) != (stre_len+1)) {
            printf("String length %d != %d\n", URI::GetEncodeLength(strC), (stre_len+1));
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        printf("Test passed\n\n");
    }
#endif // defined(TEST_STRING_HELPER_URI)


#if defined(TEST_STRING_HELPER_BASE64)

    {
        // Hell0 Wor1d => SGVsbDAgV29yMWQ=
        char s[][512] = {
                    { "H" },
                    { "Hell0 Wor1d" },
                    { "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure." }
            };
        char e[512] = { '\0' };
        char d[512] = { '\0' };

        printf("\nTesting[%u] - Base64 encode/decode\n", test++);

        for(uint16_t i=0; i<3; i++) {
            MIMEBase64::Encode(s[i], e, sizeof(e));
            printf("i: %s (%u)\no: %s (%u, %u)\n", s[i], strlen(s[i]), e, strlen(e), MIMEBase64::GetEncodeLength(s[i]));
            if (strlen(e) != MIMEBase64::GetEncodeLength(s[i])) {
                printf("Test failed.  Encode length and actual data length after encode diff (%u != %u)\n", MIMEBase64::GetEncodeLength(s[i]), strlen(e));
                STOPLINE();
                return 1;
            }

            MIMEBase64::Decode(e, d, sizeof(d));
            printf("i: %s (%u)\no: %s (%u, %u)\n", s[i], strlen(s[i]), d, strlen(d), MIMEBase64::GetDecodeLength(e));
            if (0 != strncmp(d, s[i], strlen(s[1]))) {
                printf("Test failed.  Decode string compare diffs to source (i=%u)\n", i);
                STOPLINE();
                return 1;
            }
            if (strlen(d) != MIMEBase64::GetDecodeLength(e)) {
                printf("Test failed.  Decode length and actual data length after decode diff (%u != %u)\n", MIMEBase64::GetDecodeLength(e), strlen(d));
                STOPLINE();
                return 1;
            }
        }
        printf("Test passed\n\n");
    }

#endif // defined(TEST_STRING_HELPER_BASE64)


#if defined(TEST_BUFFER)
    {
        strcpy(test_session_cookie, "nXG-0EHYz0WIeNwkGL4emqRCVYYUEiiO");
        Ethernet::Restore(false);
        HTTPBufferTester bt(Ethernet::buffer, sizeof(Ethernet::buffer), strlen(Ethernet::buffer));

        // Remove n from start
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (Remove %u start)\n", test++, n);
            uint16_t l = bt.GetLength();
            printf("H(%u): \n%s\n", l, Ethernet::buffer);
            char c = bt.At(n);
            bt.Remove(0, n-1);
            printf("H(%u): \n%s\n", bt.GetLength(), Ethernet::buffer);

            if (c != bt.At(0)) {
                printf("Test failed.  Char '%c' should be Char[%u] '%c' - after remove??\n", bt.At(0), n, c);
                STOPLINE();
                return 1;
            }
            if ((l-n) != bt.GetLength()) {
                printf("Test failed.  Length '%u', should be '%u'??\n", bt.GetLength(), l-n);
                STOPLINE();
                return 1;
            }
        }


        // Remove n from end
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (Remove %u end)\n", test++, n);
            uint16_t l = bt.GetLength();
            printf("H(%u): \n%s\n", l, Ethernet::buffer);
            uint16_t os = l - (n+1);
            char c = bt.At(os);
            bt.Remove(l - n, l - 1);
            printf("H(%u): \n%s\n", bt.GetLength(), Ethernet::buffer);

            if (c != bt.At(os)) {
                printf("Test failed.  Char '%c' should be Char[%u] '%c' - after remove??\n", bt.At(os), n, c);
                STOPLINE();
                return 1;
            }
            if ((l-n) != bt.GetLength()) {
                printf("Test failed.  Length '%u', should be '%u'??\n", bt.GetLength(), l-n);
                STOPLINE();
                return 1;
            }
        }

        // Remove from middle
        uint16_t s = bt.GetLength() / 2;
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (Remove %u middle)\n", test++, n);
            uint16_t l = bt.GetLength();
            printf("H(%u): \n%s\n", l, Ethernet::buffer);
            uint16_t os = s + n;
            char c = bt.At(os);
            bt.Remove(s, s + n-1);
            printf("H(%u): \n%s\n", bt.GetLength(), Ethernet::buffer);

            os = s;
            if (c != bt.At(os)) {
                printf("Test failed.  Char '%c' should be Char[%u] '%c' - after remove??\n", bt.At(os), os, c);
                STOPLINE();
                return 1;
            }
            if ((l-n) != bt.GetLength()) {
                printf("Test failed.  Length '%u', should be '%u'??\n", bt.GetLength(), l-n);
                STOPLINE();
                return 1;
            }
        }


        printf("\nTesting[%u] - HTTP Buffer (offset & PrepareZeroOffsetData)\n", test++);
        // Offset test
        Ethernet::Restore(false);

        // Take our buffer string, set it at +256 characters
        uint16_t sz = strlen(Ethernet::buffer) + 1;
        memcpy(Ethernet::buffer + 256, Ethernet::buffer, sz);

        // Clear our string from start, leaving the +256 intact
        memset(Ethernet::buffer, 0, sz);
        sz--; // Remove NULL

        // Here 256 characters is our new buffer location.  In real life this is likely to be our Response buffer while +0 will be our Request
        // allowing Response to still use some of the Request data even while it is being populated.
        bt.SetMaximum(sizeof(Ethernet::buffer) - 256);    // Maximum = sizeof(...) - 256
        bt.SetBuffer(Ethernet::buffer, sz, 256);        // Buffer+=256, Offset = 256, Length = original n characters

        if (bt.GetBuffer() != &Ethernet::buffer[256]) {
            printf("Test failed.  Buffer pointer '%p', should be '%p'??\n", bt.GetBuffer(), &Ethernet::buffer[256]);
            STOPLINE();
            return 1;
        }

        printf("H(%u): \n%p = %s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

        uint16_t nsz = strlen(Ethernet::buffer);
        if (nsz!=0) {
            printf("Test failed.  String length '%u', should be '%u'??\n", nsz, 0);
            STOPLINE();
            return 1;
        }

        // Shift our fake Response data back to beginning of buffer
        bt.PrepareZeroOffsetData();

        if (bt.GetBuffer() != &Ethernet::buffer[0]) {
            printf("Test failed.  Buffer pointer '%p', should be '%p'??\n", bt.GetBuffer(), &Ethernet::buffer[0]);
            STOPLINE();
            return 1;
        }

        printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

        nsz = strlen(Ethernet::buffer);
        if (nsz!=sz) {
            printf("Test failed.  Prepared string length '%u', should be '%u'??\n", nsz, sz);
            STOPLINE();
            return 1;
        }


        // Append test (which is Insert at end buffer)
        char test_str[][64] = {
            "Hello-World: something\r\n",    /* 0 */
            "Direction: stop=0; go=1\r\n",    /* 1 */
            "New-String: isitworking\r\n",    /* 2 */
            "Test: value=45\r\n",            /* 3 */
            "",                                /* that's it */
        };

        // Append test singles
        for(uint16_t n=0; ; n++) {
            if (!strlen(&test_str[n][0])) {
                break; // that's it
            }
            printf("\nTesting[%u] - HTTP Buffer (append from empty with string[%u])\n", test++, n);
            memset(Ethernet::buffer, 0, sizeof(Ethernet::buffer));

            bt.SetMaximum(sizeof(Ethernet::buffer));
            bt.SetBuffer(Ethernet::buffer);
        
            sz = bt.GetLength();
            if (sz) {
                printf("Test failed.  Before length '%u', should be '%u'??\n", sz, 0);
                STOPLINE();
                return 1;
            }

            bt.Append(&test_str[n][0]);

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

            sz = bt.GetLength();
            nsz = strlen(&test_str[n][0]);
            if (sz!=nsz) {
                printf("Test failed.  After length '%u', should be '%u'??\n", sz, nsz);
                STOPLINE();
                return 1;
            }
        }

        // Append test accumulative
        memset(Ethernet::buffer, 0, sizeof(Ethernet::buffer));

        bt.SetMaximum(sizeof(Ethernet::buffer));
        bt.SetBuffer(Ethernet::buffer);
        uint16_t asz = 0;
        for(uint16_t n=0; ; n++) {
            if (!strlen(&test_str[n][0])) {
                break; // that's it
            }
            printf("\nTesting[%u] - HTTP Buffer (append accumulative with string[%u])\n", test++, n);
        
            sz = bt.GetLength();
            if (sz!=asz) {
                printf("Test failed.  Before length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }

            bt.Append(&test_str[n][0]);

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

            sz = bt.GetLength();
            nsz = strlen(&test_str[n][0]);
            asz += nsz;
            if (sz!=asz) {
                printf("Test failed.  After length '%u', should be '%u'??\n", sz, nsz);
                STOPLINE();
                return 1;
            }
        }


        // Insert test
        Ethernet::Restore(false);
        sz = strlen(Ethernet::buffer);
        bt.SetMaximum(sizeof(Ethernet::buffer));
        bt.SetBuffer(Ethernet::buffer, sz);

        printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

        // Insert at start, accumulative
        asz = sz;
        for(uint16_t n=0; ; n++) {
            if (!strlen(&test_str[n][0])) {
                break; // that's it
            }
            printf("\nTesting[%u] - HTTP Buffer (insert at start, accumulative with string[%u])\n", test++, n);

            sz = bt.GetLength();
            if (sz!=asz) {
                printf("Test failed.  Before length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }

            bt.Insert(0, &test_str[n][0]);
            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

            sz = bt.GetLength();
            nsz = strlen(&test_str[n][0]);
            asz += nsz;
            if (sz!=asz) {
                printf("Test failed.  After length '%u', should be '%u'??\n", sz, nsz);
                STOPLINE();
                return 1;
            }
        }


        // Test Insert padding
        Ethernet::Restore(false);
        sz = strlen(Ethernet::buffer);
        bt.SetMaximum(sizeof(Ethernet::buffer));
        bt.SetBuffer(Ethernet::buffer, sz);

        printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

        // Fill remainder after initial null with pattern.  I used this to visually check when dumping
        char c = '+';
        for(uint16_t n=1+sz; n<256; n++) {
            bt.At(n, c);
            if ('+' == c) {
                c = '*';
            }else {
                c = '+';
            }
        }

        // Insert padding at start
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (insert padding at start, n=%u)\n", test++, n);

            char c = '0' + n -1;
            if (!bt.InsertPadding(0, c, n)) {
                printf("Test failed.  Insert padding didn't work?? (n = %u)\n", n);
                STOPLINE();
                return 1;
            }

            for(uint16_t u=0; u<n; u++) {
                if (bt.At(u) != c) {
                    printf("Test failed.  At(%u)=%x != %x\n", u, bt.At(u), c);
                    STOPLINE();
                    return 1;
                }
            }

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());
        }


        // Insert padding at end
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (insert padding at end, n=%u)\n", test++, n);

            char c = '0' + n -1;
            uint16_t blp = bt.GetLength(); // Buffer length before pad 
            if (!bt.InsertPadding(blp, c, n)) {
                printf("Test failed.  Insert padding didn't work?? (n = %u)\n", n);
                STOPLINE();
                return 1;
            }

            for(uint16_t u=0; u<n; u++) {
                if (bt.At(blp + u) != c) {
                    printf("Test failed.  At(%u)=%x != %x\n", blp + u, bt.At(blp + u), c);
                    STOPLINE();
                    return 1;
                }
            }

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());
        }


        // Insert padding at some mid point
        uint16_t i_mid = bt.GetLength()/2;
        for(uint16_t n=1; n<=5; n++) {
            printf("\nTesting[%u] - HTTP Buffer (insert padding at end, n=%u)\n", test++, n);

            char c = '0' + n -1;
            if (!bt.InsertPadding(i_mid, c, n)) {
                printf("Test failed.  Insert padding didn't work?? (n = %u)\n", n);
                STOPLINE();
                return 1;
            }

            for(uint16_t u=0; u<n; u++) {
                if (bt.At(i_mid + u) != c) {
                    printf("Test failed.  At(%u)=%x != %x\n", i_mid + u, bt.At(i_mid + u), c);
                    STOPLINE();
                    return 1;
                }
            }

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());
        }


        // Insert at some mid point, accumulative
        Ethernet::Restore(false);
        sz = strlen(Ethernet::buffer);
        bt.SetMaximum(sizeof(Ethernet::buffer));
        bt.SetBuffer(Ethernet::buffer, sz);

        printf("\n\nH(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

        uint16_t mp = bt.FindEOL(0, sz); // StringHelper::FindTerminator(0, bt.GetBuffer(), "\n", false, sz);
/*        for(mp=0; mp<sz; mp++) {
            if ('\n' == bt.At(mp)) {
                mp++; // Move over LF
                break;
            }
        }*/
        if (mp>=sz) {
            printf("Test failed.  Pre-test setup for insert mid point testing, failed to find a mid-point for inserts\n");
            STOPLINE();
            return 1;
        }
        mp++; // Move over LF

        asz = sz;
        for(uint16_t n=0; ; n++) {
            if (!strlen(&test_str[n][0])) {
                break; // that's it
            }
            printf("\nTesting[%u] - HTTP Buffer (insert at start, accumulative with string[%u])\n", test++, n);

            sz = bt.GetLength();
            if (sz!=asz) {
                printf("Test failed.  Before length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }

            bt.Insert(mp, &test_str[n][0]);
            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

            sz = bt.GetLength();
            nsz = strlen(&test_str[n][0]);
            asz += nsz;
            if (sz!=asz) {
                printf("Test failed.  After length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }
        }


        // Replace test.  Continue on from Insert at some mid point, accumulative
        // This also tests Remove and InsertPadding
        char test_replace_str[][64] = {
            "String1: something\r\n",                /* 0 */
            "String2: go=; stop=1\r\n",                /* 1 */
            "Oldish-String: no\r\n",                /* 2 */
            "Untested: value=5; ignore=true\r\n",    /* 3 */
            "",                                        /* that's it */
        };

        sz = strlen(Ethernet::buffer);

        asz = sz;
        for(uint16_t n=0; ; n++) {
            nsz = strlen(&test_replace_str[n][0]);
            if (0 == nsz) {
                break; // done...
            }

            // TODO: StringHelper::Find
            uint16_t k = 0;
            uint16_t i = 0, e = 0;
            for(mp=0; mp<sz; mp++) {
                if ('\n' == bt.At(mp)) {
                    if (k>0) {
                        i = e + 1;
                    }
                    e = mp;
                    if (k == n) {
                        break;
                    }
                    k++;
                }
            }
            printf("\nTesting[%u] - HTTP Buffer (replace, accumulative with string[%u of %u])\n", test++, n, k);

            printf("[%u].i = %u, .e = %u (length %u, replace length %u)\n", n, i, e, e - i, nsz);

            sz = bt.GetLength();
            if (sz!=asz) {
                printf("Test failed.  Before length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }

            if (!bt.Replace(i, e, &test_replace_str[n][0])) {
                printf("Test failed.  Replace unhappy??\n", sz, asz);
                STOPLINE();
                return 1;
            }

            printf("H(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());

            sz = bt.GetLength();
            asz += nsz;                // Add replace string length
            asz -= 1 + (e - i);        // Remove replaced string length
            if (sz!=asz) {
                printf("Test failed.  After length '%u', should be '%u'??\n", sz, asz);
                STOPLINE();
                return 1;
            }
        }


        // Test FindEOL
        for(uint16_t n=0, i = 0, e = 0; ; n++) {
            nsz = strlen(&test_replace_str[n][0]);
            if (0 == nsz) {
                break; // done...
            }

            printf("\nTesting[%u] - HTTP Buffer (find end of line, line string[%u])\n", test++, n);
            
            if (n==0) {
                printf("\nH(%u) %p:\n%s\n", bt.GetLength(), bt.GetBuffer(), bt.GetBuffer());
            }

            uint16_t bl = bt.GetLength();
            uint16_t fe = bt.FindEOL(i, bl);

            if (fe > bl) {
                printf("Test failed.  Find EoL expected??\n", fe, e);
                STOPLINE();
                return 1;
            }

            // Check range by looking at the data string
            for(uint32_t u=i, m=0; u<fe; u++, m++) {
                if (test_replace_str[n][m] != bt.At(u)) {
                    printf("Test failed.  Find EoL, string at character (found) [%u]0x%02x != [%u]0x%02x (actual)\n",u, bt.At(u), m, (uint32_t)test_replace_str[n][m]);
                    STOPLINE();
                    return 1;
                }
            }

            i = fe + 1;
        }


        printf("\nTesting[%u] - HTTP Buffer (operator +n)\n", test++);
        bt.SetLength(0);
        char *bf = bt.GetBuffer();
        bf[0]='\0';
        for(uint16_t i=0; i<10; i++) {
            char *addstr = "Hello\n";
            uint16_t addlen = strlen(addstr);

            strcat(bf, addstr);

            bt+=addlen;
            bf+=addlen;
        }
        bt.Append('\0');
        if (61 != bt.GetLength()) {
            printf("Test failed.  Unexpected length %d != %d\n", 61, bt.GetLength());
            STOPLINE();
            return 1;
        }
        printf("String:\n");
        printf("%s", bt.GetBuffer());
        printf("Length: %u\n", bt.GetLength());

        printf("Test passed\n\n");
    }
#endif // defined(TEST_BUFFER)

#if defined(TEST_RESPONSE_HEADER)
    {
        memset(Ethernet::buffer, 0, sizeof(Ethernet::buffer));

        HTTPResponse<HTTPResponseStatus> bt(Ethernet::buffer, sizeof(Ethernet::buffer), strlen(Ethernet::buffer));

        // TODO: add more checking to the tests

        for(uint32_t i=0; i<2; i++) {
            // Change status to not implemented
            if (!i) {
                printf("\nTesting[%u, i=%u] - HTTP Response header (no buffer data, status addition)\n", test++, i);
            }else {
                printf("\nTesting[%u, i=%u] - HTTP Response header (pre-assembled, status change not implemented)\n", test++, i);
            }
            printf("H: \n%s\n", Ethernet::buffer);
            bt.status.Set(bt.status.STATUS_NOT_IMPLEMENTED);
            if (!bt.Header("HTTP/1.1")) {
                printf("Test failed.  Adding or changing status header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");

            // Change status to forbidden
            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, status change forbidden)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, status change forbidden)\n", test++);
            }
            bt.status.Set(bt.status.STATUS_FORBIDDEN);
            if (!bt.Header("HTTP/1.1")) {
                printf("Test failed.  Adding or changing status header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");


            // Change status to see other (same length as forbidden)
            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, status change see other)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, status change see other)\n", test++);
            }
            bt.status.Set(bt.status.STATUS_SEE_OTHER);
            if (!bt.Header("HTTP/1.1")) {
                printf("Test failed.  Adding or changing status header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");


            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, append same header)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, append same header)\n", test++);
            }
            bt.Header("WWW-Authenticate: Negotiate", false);
            printf("H: \n%s\n", Ethernet::buffer);

            bt.Header("WWW-Authenticate: Negotiate", false);
            printf("H: \n%s\n", Ethernet::buffer);

            bt.Header("WWW-Authenticate: Negotiate", false);
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");

            // Change status to OK
            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, status change ok)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, status change ok)\n", test++);
            }
            bt.status.Set(bt.status.STATUS_OK);
            if (!bt.Header("HTTP/1.1")) {
                printf("Test failed.  Adding or changing status header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");

            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, append another header)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, append another header)\n", test++);
            }
            if (!bt.Header("New-Header: Somethingelse; one=true")) {
                printf("Test failed.  Adding header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");

            if (!i) {
                printf("\nTesting[%u] - HTTP Response header (no buffer data, append with replace on a previous header, 3off)\n", test++);
            }else {
                printf("\nTesting[%u] - HTTP Response header (pre-assembled, append with replace on a previous header, 3off)\n", test++);
            }
            if (!bt.Header("WWW-Authenticate: Test")) {
                printf("Test failed.  Replacing header\n");
                STOPLINE();
                return 1;
            }
            printf("H: \n%s\n", Ethernet::buffer);
            printf("Test passed\n\n");

            Ethernet::Restore(false);
            bt.SetBuffer(Ethernet::buffer, strlen(Ethernet::buffer));
        }
    }
#endif // defined(TEST_RESPONSE_HEADER)


#if defined(TEST_RESPONSE_BUILD)
    {
        uint16_t i;
        Ethernet::Restore(false);

        printf("\nTesting[%u] - HTTP Response (headers complete with content length)\n", test++);
        HTTPResponse<HTTPResponseStatus> bt(Ethernet::buffer, sizeof(Ethernet::buffer), strlen(Ethernet::buffer), 200);

        printf("Ethernet::buffer 0x%p\n\n", Ethernet::buffer);

        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        i = bt.GetEndOfHeaders(true);
        printf("EoH i = %u\n", i);

        bt.status.Set(bt.status.STATUS_OK);
        if (!bt.Header("HTTP/1.1")) {
            printf("Test failed.  Adding or changing status header\n");
            STOPLINE();
            return 1;
        }
        
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        i = bt.GetEndOfHeaders(true);
        printf("EoH i = %u\n", i);
        if (!i) {
            printf("Test failed.  Expected EOH index >0\n");
            STOPLINE();
            return 1;
        }

        i = bt.GetEndOfHeaders();
        printf("EoH i (complete) = %u\n", i);
        if (i) {
            printf("Test failed.  Expected EOH (complete) index ==0\n");
            return 1;
        }
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());


        // Check we can't add a header if badly formatted
        printf("\nCheck add header failure on bad format\n");
        uint16_t bhl = bt.GetLength();
        if (bt.Header("This Shouldnt: work")) {
            printf("Test failed.  Adding header success, expected failure\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Unexpected length change %u != %u after header addition which failed\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        if (bt.Header("This\tShouldnt: work")) {
            printf("Test failed.  Adding header success, expected failure\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Unexpected length change %u != %u after header addition which failed\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        if (bt.Header("This\tShouldnt work")) {
            printf("Test failed.  Adding header success, expected failure\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Unexpected length change %u != %u after header addition which failed\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }


        printf("\nHeader Remove(multiple) check\n\n");
        bhl = bt.GetLength();
        if (!bt.Header("New-Thing: value=34; state=off")) {
            printf("Test failed.  Couldn't add header\n");
            STOPLINE();
            return 1;
        }
        if (!bt.Header("New-Thing: value=963", false)) {
            printf("Test failed.  Couldn't add header\n");
            STOPLINE();
            return 1;
        }
        if (!bt.Header("New-Thing: value=-344; state=mixed; output=on", false)) {
            printf("Test failed.  Couldn't add header\n");
            STOPLINE();
            return 1;
        }
        bt.Dump("BR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        bool rs = bt.HeaderRemove("New-Thing: xxx", true);
        if (!rs) {
            printf("Test failed.  Failed to remove multiple headers??\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Length before addition %u != %u length after with remove\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        bt.Dump("AR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        printf("\nHeader Remove(single) check\n\n");
        if (!bt.Header("New-Thing: change=no; state=switched")) {
            printf("Test failed.  Couldn't add header\n");
            STOPLINE();
            return 1;
        }
        bt.Dump("BR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        rs = bt.HeaderRemove("New-Thing", false);
        if (!rs) {
            printf("Test failed.  Failed to remove multiple headers??\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Length before addition %u != %u length after with remove\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        bt.Dump("AR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        printf("\nHeader Remove(single, against multiple headers) check\n\n");
        uint16_t lentab[3];
        char rhs[3][48] = {
            "New-Thing: value=34; state=off",
            "New-Thing: value=963",
            "New-Thing: value=-344; state=mixed; output=on"
        };
        bhl = bt.GetLength();
        for(i=0; i<3; i++) {
            if (!bt.Header(&rhs[i][0], false)) {
                printf("Test failed.  Couldn't add header (i=%u), line %u\n", i, __LINE__);
                STOPLINE();
                return 1;
            }
            lentab[i] = bhl;
            for(uint16_t u=i+1; u<3; u++) {
                lentab[i] += strlen(&rhs[u][0]) + 2; // +CR+LF
            }
        }

        bt.Dump("BR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        for(i=0; i<3; i++) {
            rs = bt.HeaderRemove("New-Thing: value=963", false);
            if (!rs) {
                printf("Test failed.  Failed to remove single headers (i=%u)??\n", i);
                STOPLINE();
                return 1;
            }
            if (bt.GetLength() != lentab[i]) {
                printf("Test failed.  Length before addition %u != %u length after with remove (i=%u)\n", lentab[i], bt.GetLength(), i);
                STOPLINE();
                return 1;
            }
            printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        }
        bt.Dump("AR: ", 0, bt.GetLength());


        printf("\nHeader Remove(no like header exists so should fail) check\n\n");
        bt.Dump("BR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        bhl = bt.GetLength();
        rs = bt.HeaderRemove("New");
        if (!rs) {
            printf("Test failed.  Remove indicated success, expect failure??\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Length before addition %u != %u length after with remove\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        bt.Dump("AR: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());


        printf("\nCheck Header complete\n");
        // Add a header which after complete we'll try and remove
        if (!bt.Header("Try-Remove-Me: nothing")) {
            printf("Test failed.  Couldn't add header\n");
            STOPLINE();
            return 1;
        }
        bt.Dump("BC: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        if (!bt.HeaderComplete()) {
            printf("Test failed.  Could not complete response headers\n");
            STOPLINE();
            return 1;
        }
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());
        bt.Dump("AC: ", 0, bt.GetLength());
        i = bt.GetEndOfHeaders();
        printf("EoH i (complete) = %u\n", i);
        if (!i) {
            printf("Test failed.  End of headers should not be 0\n");
            STOPLINE();
            return 1;
        }
        // Check we can't remove a header after complete
        bhl = bt.GetLength();
        rs = bt.HeaderRemove("New-Thing");
        if (rs) {
            printf("Test failed.  Should have failed to remove headers??\n");
            STOPLINE();
            return 1;
        }
        if (bt.GetLength() != bhl) {
            printf("Test failed.  Length before remove attempt %u != %u length after\n", bhl, bt.GetLength());
            STOPLINE();
            return 1;
        }
        bt.Dump("ARA: ", 0, bt.GetLength());
        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());


        // Check adding content
        char* data1="<html><body><p>Test</p>";
        char* data2="</body></html>";
        if (!bt.Append(data1)) {
            printf("Test failed.  Append data1 = '%s' didn't work\n", data1);
            STOPLINE();
            return 1;
        }
        if (!bt.Append(data2)) {
            printf("Test failed.  Append data1 = '%s' didn't work\n", data2);
            STOPLINE();
            return 1;
        }

        uint32_t ccl = strlen(data1) + strlen(data2);

        printf("Content length = %u.  Should be %u\n", bt.ContentLength(), ccl);
        if (bt.ContentLength() != ccl) {
            printf("Test failed.  Calculated content length differs\n");
            STOPLINE();
            return 1;
        }

        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        if (!bt.Complete()) {
            printf("Test failed.  Could not complete response\n");
            STOPLINE();
            return 1;
        }
        printf("Request complete\n");

        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        bt.Dump("Dump", 0, 256);

        bt.Append(data2);
        ccl += strlen(data2);
        printf("Content length = %u.  Should be %u\n", bt.ContentLength(), ccl);
        if (bt.ContentLength() != ccl) {
            printf("Test failed.  Calculated content length differs\n");
            STOPLINE();
            return 1;
        }

        if (!bt.Complete()) {
            printf("Test failed.  Could not complete response\n");
            STOPLINE();
            return 1;
        }

        printf("H: 0x%p\n%s\n", bt.GetBuffer(), bt.GetBuffer());

        bt.Dump("Dump", 0, 256);

        printf("Test passed\n\n");
    }
#endif // defined(TEST_RESPONSE_BUILD)


#if defined(TEST_COMPLETE_ROUTE_MANAGER_REQUEST_RESPONSE)
    {
        printf("\nTesting[%u] - Route Manager, HTTP request with optional sessions\n", test++);
        Shared sd;
        RouteManager<10, Route<Shared>, Shared>  rm(&sd);    // Maximum of 10 routes in this example

        sd.client.ip4 = "10.0.0.10";    // This would have been setup before request processing
        sd.server.ip4 = "10.0.0.2";        // This would have been setup before request processing

        sd.response.SetMaximum(sizeof(Ethernet::buffer)-200);

        // Create our routes handled by this app (these would be globals in practice)
        RouteRoot route_root;
        RouteSomethingEx route_something;
        RouteTest route_test;
        RouteUnhandled route_unhandled;

        // Register them
        int16_t r = rm.Register(&route_root);
        if (-1 == r) {
            printf("Test failed.  Route manager register of root failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route root at index %d\n", r);
        int16_t r_something = rm.Register(&route_something);
        if (-1 == r_something) {
            printf("Test failed.  Route manager register of something failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route something at index %d\n", r_something);
        r = rm.Register(&route_test);
        if (-1 == r) {
            printf("Test failed.  Route manager register of test failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route test at index %d\n", r);

#if 0 // We can't have default if we want to capture the fact no route might have been taken
        // Default is last route
        r = rm.Register(&route_unhandled);
        if (-1 == r) {
            printf("Test failed.  Route manager register of default failed\n");
            STOPLINE();
            return 1;
        }
        printf("Register route default at index %d\n", r);
#endif // 0

        // This would be our request receive and parse main loop
        Ethernet::Restore();    // Effectively a socket RX

        // Parse our request...
        uint16_t ci = sd.request.Parse(Ethernet::buffer, strlen(Ethernet::buffer), sizeof(Ethernet::buffer));
        if (sd.request.ERR_NONE != sd.request.GetLastError()) {
            printf("Error: %u\n", sd.request.GetLastError());
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }

        int16_t route_ix = rm.Service();
        if (r_something != route_ix) {
            printf("Test failed.  No route or wrong route handled request (%d != %d)\n", r_something, route_ix);
            STOPLINE();
            return 1;
        }

        sd.session_manager.Service();

        sd.response.Dump("Response", 0, 256);

        printf("Test passed\n\n");
    }
#endif // defined(TEST_COMPLETE_ROUTE_MANAGER_REQUEST_RESPONSE)

#if defined(TEST_REQUEST_FRAGMENTED_DECODE_SUCCESS)
    {
        Ethernet::Restore();

        printf("%s\n", Ethernet::buffer);

        printf("\nTesting[%u] - HTTP request fragmented decode(success)\n", test++);
        SessionManager<10, Session<Global, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> sm;
        HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> r;

#if !defined(TEST_REQUEST_FRAGMENTED_DECODE_V09)
        uint16_t eoh = StringHelper::FindSubString(Ethernet::buffer, "\r\n\r\n", 0, true, sizeof(Ethernet::buffer)-1, 4);
        if (eoh>=sizeof(Ethernet::buffer)-1) {
            printf("Error: %u.  No EOH found in buffer\n", r.GetLastError());
            printf("Test failed\n");
            STOPLINE();            
            return 1;
        }
#else // defined(TEST_REQUEST_FRAGMENTED_DECODE_V09)
        uint16_t eoh = strlen(Ethernet::buffer)/2;
#endif

        // build up array of strings which can be segmented, making sure the EOH is intentionally broken up to test parsing
        struct {
            char *ptr;
            uint16_t length;
        }segment[8];
        memset(segment, 0, sizeof(segment));
        segment[0].ptr = &Ethernet::buffer[0];
        segment[0].length = eoh/2;    // Reducing this to < strlen(first line) still works
        uint16_t next = segment[0].length;
        uint16_t ts = /*(sizeof(Ethernet::buffer)-1)*/ strlen(Ethernet::buffer) - next;
        srand((unsigned int)time(NULL));
        uint16_t max_i=0;
        printf("Total length %u and actual request length %u\n", sizeof(Ethernet::buffer)-1, strlen(Ethernet::buffer));
        r.Clear();
        for(uint16_t i=1,n; i<8; i++) {
            if (ts==0) {
                break;
            }
            if (i==7 || ts<64) {
                n=ts;
            }else {
                do {
                    n=rand() % ts;
                }while(n<64 || n>800);
                if ((ts-n)<32) {
                    n = ts;
                }
            }
            max_i = i;
            segment[i].ptr = &Ethernet::buffer[next];
            segment[i].length = n;
            next+=n;
            ts-=n;
        }
        char rtxbuffer[sizeof(Ethernet::buffer)];
        ts = 0;
        for(uint16_t i=0; i<=max_i; i++) {
            printf("Segment [%u] ptr: %p, length %u\n", i, segment[i].ptr, segment[i].length);
#if defined(PRINT_SEGMENT)
            for(uint16_t u=0; u<segment[i].length; u++) {
                printf("%c", segment[i].ptr[u]);
            }
            printf("\n\n");
#endif // defined(PRINT_SEGMENT)

            memcpy(&rtxbuffer[ts], segment[i].ptr, segment[i].length);
            ts+=segment[i].length;

/*            printf("rq:\n");
            for(uint16_t u=0; u<ts; u++) {
                if (rtxbuffer[u]==0) {
                    printf(".");
                }else {
                    printf("%c", rtxbuffer[u]);
                }
            }
            printf("\n\n");*/
            printf("Request size %u\n", ts);

            uint16_t ci = r.Parse(rtxbuffer, r.GetLength() + segment[i].length /*ts*/, sizeof(rtxbuffer));
            if (r.ERR_SEGMENTATION == r.GetLastError()) {
                printf("Segment parsed\n");
                if (ts == strlen(Ethernet::buffer)) {
                    printf("Error: %u, this is the final transfer so shouldnt get segmentation??\n", r.GetLastError());
                    printf("Test failed\n");
                    STOPLINE();
                    return 1;
                }
            }else if (r.ERR_NONE != r.GetLastError()) {
                printf("Error: %u\n", r.GetLastError());
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }else {
                printf("Received all data\n");
            }
        } // for(uint16_t i=0...
/*
        if (!r.header["Authorization"]) {
            printf("Authorization doesnt exist??\n");
            STOPLINE();
            return 1;
        }
*/

        uint8_t i, c = r.header.count;
        printf("Headers: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.header.At(i);

            printf("\t%s: %s\n", h->Get(), h->value.Get());
        }
/*        if (8!=c) {
            printf("Expected 8 headers\n");
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }*/
        c = r.cookie.count;
        printf("Cookies: %u\n", c);
/*        if (5!=c) {
            printf("Test failed\n");
            STOPLINE();
            return 1;
        }*/
        for(i=0; i<c; i++) {
            auto h = r.cookie[i];

            printf("\t%s = %s\n", h->Get(), h->value.Get());
        }
        if (NULL != &r.session) {
            printf("SESSION is %s\n", &r.session);
        }
        if (NULL != &r.content) {
            printf("Content(%u):\n%s\n", strlen(&r.content), &r.content);
        }else {
            printf("No content\n");
            //printf("Test failed\n");
            //STOPLINE();
            //return 1;
        }
        printf("sizeof(sessionmgr_instance) is %u\n", sizeof(sm));
        printf("sizeof(request_instance) is %u\n", sizeof(r));
        printf("username: '%s'\n", r.username);
        printf("password: '%s'\n", r.password);
        printf("authorization username: '%s'\n", r.authorization_username);
        printf("authorization password: '%s'\n", r.authorization_password);
        printf("url: '%s'\n", r.url);
        if (r.url == "/login") {
            printf("route valid\n");
        }else {
            printf("route NOT valid\n");
        }
        printf("method: '%s'\n", r.method);
        printf("protocol: '%s'\n", r.protocol);
        printf("protocol_version: '%s'\n", r.protocol_version);
        printf("url_raw: '%s'\n", r.url_raw);

        c = r.content_type.count;
        printf("content type list: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.content_type.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

        c = r.query_variable.count;
        printf("url variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.query_variable.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
        c = r.post_variable.count;
        printf("post variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.post_variable.At(i);

            printf("\t%s = %s\n", h->Get(), h->value.Get());
            if (h->Get() == NULL) {
                printf("Name NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }

            if (h->value.Get() == NULL) {
                printf("Value NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }

        c = r.post_file_variable.count;
        printf("post file variables: %d\n", c);
        for(i=0; i<c; i++) {
            auto h = r.post_file_variable.At(i);
            const char* p = h->value.Get();

            printf("\t%s = ", h->Get());
            for(uint16_t a = 0; a<h->filesize; a++) {
                if (p[a]<32) {
                    printf("%02x", p[a]);
                }else {
                    printf("%c", p[a]);
                }
            }

            printf(" (%u) as %s, type %s\n", h->filesize, h->filename, h->mimetype);
            if (NULL == h->filename.Get()) {
                printf("Filename NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
            if (NULL == h->mimetype.Get()) {
                printf("Mimetype NULL\n");
                printf("Test failed\n");
                STOPLINE();
                return 1;
            }
        }
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1
        printf("Test passed\n\n");
    }
#endif // defined(TEST_REQUEST_FRAGMENTED_DECODE_SUCCESS)

    printf("All tests complete\n");
/*
    {
        uint8_t data[] = { 0x0a, 0x40, 0x41, 0x42, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x00 };

        FILE *fp = fopen("C:\\temp\\stuff\\bellbox\\sendtxt.bin", "wb");
        if (fp) {
            fwrite(data, sizeof(data), 1, fp);
            fclose(fp);
            printf("Saved file");
        }
    }
*/
    return 0;
}

