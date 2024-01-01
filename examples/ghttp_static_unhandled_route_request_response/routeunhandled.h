/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_unhandled_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEUNHANDLED_H)
#define ROUTEUNHANDLED_H

namespace examples { namespace ghttp_static_unhandled_route_request_response {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/something".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteUnhandled : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer.  See offset on HTTPBuffer::SetBuffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Unhandled</p></body></html>");
        s.response.Complete();

        return true; // We have handled.  This is important for an unhandled handler as this indicates to route manager to stop working
    }
}; // class RouteUnhandled

} } // namespace examples::ghttp_static_unhandled_route_request_response

#endif // !defined(ROUTEUNHANDLED_H)
