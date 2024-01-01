/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_pre_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(SHARED_H)
#define SHARED_H

namespace examples { namespace ghttp_static_pre_route_request_response {

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
    
    /* I added this application specific data area instance which you could call it what you want, it is passed to every route so essentially a global, not in session 
     * data but still available to all routes during processing.  I simply record the last route taken and the next route will print it for no reason other than show 
     * you how you could add application wide globals
     */
    class Application {
    public:
        Application() : idx_(0), url_{ {"\0"}, {"\0"} } {}
        
        void Set(const char *url) {
            uint8_t l = strlen(url);

            if (l>15) {
                l = 15;
            }
            strncpy(url_[idx_], url, l);
            idx_^=1;
        }
        
        const char* GetLast() const {
            return &url_[idx_ ^ 1][0];
        }
        
    private:
        uint8_t idx_;
        char url_[2][16];
    }app;
}; // class Shared

} } // namespace examples::ghttp_static_pre_route_request_response

#endif // !defined(SHARED_H)
