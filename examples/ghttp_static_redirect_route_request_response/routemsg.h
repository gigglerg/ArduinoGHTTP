/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_redirect_route_request_response
 * 
 * DG, 2020
 */

#if !defined(ROUTEMSG_H)
#define ROUTEMSG_H

namespace examples { namespace ghttp_static_redirect_route_request_response {

/**
 * An example implemented route for protocol HTTP, GET requests covering route "/msg".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteMsg : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/msg") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Our response instance is to use the same buffer as request to reduce memory footprint.  There is an offset because we want to access some header fields
        // and the actual figure is a guesstimate based upon known request header layout as they are fictitious in this example 
        s.response.SetMaximum( s.request.GetMaximum()-164 );
        s.response.SetBuffer( s.request.GetBuffer(), 0, 164);

        // Set status
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer.  See offset on HTTPBuffer::SetBuffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Msg: ");
        if (s.request.query_variable[T("text")]) {                                    // Like PHP isset($_GET["text"])
            s.response+=s.request.query_variable[T("text")]->value.Get();
        }else {
            s.response+=T("Unknown");
        }
        s.response+=T("</p></body></html>");
        s.response.Complete();

        return true; // We have handled.  This is important for an unhandled handler as this indicates to route manager to stop working
    }
}; // class RouteMsg

} } // namespace examples::ghttp_static_redirect_route_request_response

#endif // !defined(ROUTEMSG_H)
