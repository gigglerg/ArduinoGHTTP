/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_unhandled_route_request_response
 * 
 * DG, 2020
 */

#if !defined(SHARED_H)
#define SHARED_H

namespace examples { namespace ghttp_static_unhandled_route_request_response {

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
    ghttp::HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE> request;

    // Response
    ghttp::HTTPResponse<ghttp::HTTPResponseStatus> response;

    // Client
    struct {
        ghttp::IP4_t    ip4;
    }client;

    // Server
    struct {
        ghttp::IP4_t    ip4;
    }server;
}; // class Shared

} } // namespace examples::ghttp_static_unhandled_route_request_response

#endif // !defined(SHARED_H)
