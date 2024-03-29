/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_windoze_toast_notification
 * 
 * DG, 2020
 */

#if !defined(SHARED_H)
#define SHARED_H

namespace examples { namespace ghttp_ethercard_windoze_toast_notification {

/**
 * Application specific class holding everything required to process request/reponses.  PHP like super globals, here collected into a single class which can be passed 
 * around the system to keep encapsulation.   Add and remove variables that your application would want to use
 *
 * \note There is no set layout of this object however it would seem pretty obvious that a request and response instance would be required
 * \note Even if not using sessions you can put items in here that you need shared between your request/response implementation
 */
class Shared {
public:
    /* Request */
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_COOKIE> request;

    /* Response */
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

    /* Server */
    struct {
        ghttp::IP4_t    ip4;
        ghttp::MAC_t    mac;
        ghttp::IP4_t    gateway_ip4;    // Required for Ethercard library
    }server;

    /* Miscellaneous */
    struct {
        char *response_buffer;
    }misc;
}; // class Shared

} } // namespace examples::ghttp_ethercard_windoze_toast_notification

#endif // !defined(SHARED_H)
