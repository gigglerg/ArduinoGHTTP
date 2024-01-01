/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_pre_route_request_response
 * 
 * DG, 2020
 */

#if !defined(ROUTEPRE_H)
#define ROUTEPRE_H

namespace examples { namespace ghttp_static_pre_route_request_response {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/something".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RoutePre : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // All routes only accept GET requests over HTTP.   I can't think of anything special right now for an example pre-route handler but I am sure you can??
        if (s.request.method!=T("get") || s.request.protocol!=T("http")) {
            ghttp::HTTPHelper::Redirect<Shared>(s, "/error");
            return true;
        }

        return false; // We have not handled.  This is important for a pre-route handler as this indicates to route manager to continue working
    }
}; // class RoutePre

} } // namespace examples::ghttp_static_pre_route_request_response

#endif // !defined(ROUTEPRE_H)
