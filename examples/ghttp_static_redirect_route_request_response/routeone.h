/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_redirect_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEONE_H)
#define ROUTEONE_H

namespace examples { namespace ghttp_static_redirect_route_request_response {

/**
 * An example implemented route for protocol HTTP, GET requests covering route "/one".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteOne : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/one") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Our response instance is to use the same buffer as request to reduce memory footprint
        s.response.SetMaximum( s.request.GetMaximum() );
        s.response.SetBuffer( s.request.GetBuffer(), 0);

        // Set status
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);

        // Do nothing but redirect.
        ghttp::HTTPHelper::Redirect<Shared>(s, "/two", ghttp::HTTPResponseStatus::STATUS_MOVED_PERMANENTLY, true);
        
        return true; // We have handled
    }
}; // class RouteOne

} } // namespace examples::ghttp_static_redirect_route_request_response

#endif // !defined(ROUTEONE_H)
