/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_request_response_simple
 * 
 * DG, 2020
 */

#if !defined(SHARED_H)
#define SHARED_H

namespace examples { namespace ghttp_static_request_response_simple {

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
    // This application has a maximum of 10 active sessions
    ghttp::SessionManager<10, ghttp::Session<Global, ghttp::SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> session_manager;
#endif

    // Request
#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1 && GHTTP_REQUEST_PROCESS_COOKIES == 1
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> request;

#elif GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1 && GHTTP_REQUEST_PROCESS_COOKIES == 0
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE> request;

#elif GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 0 && GHTTP_REQUEST_PROCESS_COOKIES == 1
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_COOKIE> request;

#else // GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 0 && GHTTP_REQUEST_PROCESS_COOKIES == 0
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE> request;
#endif

    // Response
    ghttp::HTTPResponse<ghttp::HTTPResponseStatus> response;

    // Client
    struct {
        ghttp::IP4_t    ip4;
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        ghttp::MAC_t    mac;
#endif
    }client;

    // Server
    struct {
        ghttp::IP4_t    ip4;
    }server;
}; // class Shared

} } // namespace examples::ghttp_static_request_response_simple

#endif // !defined(SHARED_H)
