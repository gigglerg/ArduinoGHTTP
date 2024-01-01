/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_redirect_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTETWO_H)
#define ROUTETWO_H

namespace examples { namespace ghttp_static_redirect_route_request_response {

/**
 * An example implemented route for protocol HTTP, GET requests covering route "/two".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteTwo : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/two") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Our response instance is to use the same buffer as request to reduce memory footprint
        s.response.SetMaximum( s.request.GetMaximum() );
        s.response.SetBuffer( s.request.GetBuffer(), 0);

        // Set status
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Two</p></body></html>");
        
        /* This redirect here is to show that even when the response is almost complete you can still redirect ignoring everything until this point
         * Use HTTPResponseStatus::STATUS_FOUND, redirecting to route "<server ip>/msg"
         */
        ghttp::HTTPHelper::Redirect<Shared>(s, "/msg?text=testing", ghttp::HTTPResponseStatus::STATUS_FOUND, true);
        return true; // We should not continue working on the original response, just stop now as redirect helper has done everything required

        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteTwo

} } // namespace examples::ghttp_static_redirect_route_request_response

#endif // !defined(ROUTETWO_H)
