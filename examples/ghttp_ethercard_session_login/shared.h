/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_session_login
 * 
 * DG, 2020
 */

#if !defined(SHARED_H)
#define SHARED_H

namespace examples { namespace ghttp_ethercard_session_login {

/**
 * Application specific class holding everything required to process request/reponses.  PHP like super globals, here collected into a single class which can be passed 
 * around the system to keep encapsulation.   Add and remove variables that your application would want to use
 *
 * \note There is no set layout of this object however it would seem pretty obvious that a request and response instance would be required
 * \note Even if not using sessions you can put items in here that you need shared between your request/response implementation
 */
class Shared {
public:
    // Request
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_COOKIE> request;

    // Sessions
    ghttp::SessionManager<GHTTP_SESSIONMGR_MAX_SESSIONS, ghttp::Session<Global, ghttp::SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> session_manager;

    // Response
    ghttp::HTTPResponse<ghttp::HTTPResponseStatus> response;

#if GHTTP_SERVER_USE_SESSIONS == 1
    // Client
    struct {
        ghttp::IP4_t    ip4;
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        ghttp::MAC_t    mac;
#endif // GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    }client;
#endif // GHTTP_SERVER_USE_SESSIONS == 1

    // Server
    struct {
        ghttp::IP4_t    ip4;
        ghttp::MAC_t    mac;
        ghttp::IP4_t    gateway_ip4;    // Required for Ethercard library
    }server;

    // Miscellaneous
    struct {
        char *response_buffer;
    }misc;
    
    void GenericHeader() {
        response.Header(T("Date: Wed, 18 Mar 2020 09:29:07 GMT"));
        response.Header(T("Server: Apache/2.4.38 (Ubuntu)"));
        response.Header(T("Expires: Thu, 19 Nov 1981 08:52:00 GMT"));
        response.Header(T("Cache-Control: no-store, no-cache, must-revalidate"));
        response.Header(T("Pragma: no-cache"));
        response.Header(T("Content-Type: text/html; charset=UTF-8"));
        response.Header(T("Access-Control-Allow-Origin: *"));
        response.Header(T("Access-Control-Allow-Methods: GET, POST, OPTIONS"));
        response.Header(T("Access-Control-Allow-Headers: Content-Type, Authorization"));    
    }
}; // class Shared

} } // namespace examples::ghttp_ethercard_session_login

#endif // !defined(SHARED_H)
