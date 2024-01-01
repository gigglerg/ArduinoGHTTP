/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_unhandled_route_request_response
 * 
 * DG, 2020
 */

#if !defined(ROUTEFOO_H)
#define ROUTEFOO_H

namespace examples { namespace ghttp_static_unhandled_route_request_response {

/**
 * An example implemented route for protocol HTTP, GET requests covering route "/foo".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteFoo : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/foo") || s.request.method!=T("get") || s.request.protocol!=T("http")) {
            // Not handled
            return false;
        }

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Foo</p></body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteFoo

} } // namespace examples::ghttp_static_unhandled_route_request_response

#endif // !defined(ROUTEFOO_H)
