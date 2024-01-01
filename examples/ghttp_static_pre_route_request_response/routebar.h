/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_pre_route_request_response
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEBAR_H)
#define ROUTEBAR_H

namespace examples { namespace ghttp_static_pre_route_request_response {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/something".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteBar : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/bar")) { // Our pre-route checked other parameters so we don't have to
            // Not handled
            return false;
        }

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't access much of the request anymore since they share the same buffer.  See offset on HTTPBuffer::SetBuffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Bar.  Last route: ");
        const char* last_str = s.app.GetLast();
        if (strlen(last_str)) {
            s.response+=last_str;
        }else {
            s.response+="none";
        }
        s.response+=T("</p></body></html>");

        // Update my app instance which is shared between all requests with this route's url
        // NOTE: It is very important we finish accessing request fields before issuing s.response.Complete() as these then get overwritten
        s.app.Set(&s.request.url);

        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteBar

} } // namespace examples::ghttp_static_pre_route_request_response

#endif // !defined(ROUTEBAR_H)
