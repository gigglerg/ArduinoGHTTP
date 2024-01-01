/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_unhandled_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEBAR_H)
#define ROUTEBAR_H

namespace examples { namespace ghttp_static_unhandled_route_request_response {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/bar".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteBar : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/bar") || s.request.method!=T("post") || s.request.protocol!=T("http")) {
            // Not handled
            return false;
        }

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Bar</p></body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteBar

} } // namespace examples::ghttp_static_unhandled_route_request_response

#endif // !defined(ROUTEBAR_H)
