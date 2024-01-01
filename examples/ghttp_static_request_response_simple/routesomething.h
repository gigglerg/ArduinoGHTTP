/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_request_response_simple
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTESOMETHING_H)
#define ROUTESOMETHING_H

namespace examples { namespace ghttp_static_request_response_simple {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/something".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteSomething : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/something") || s.request.method!=T("post") || s.request.protocol!=T("http")) {
            // Not handled
            return false;
        }

#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
        // Use this helper to get session globals + set status
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s);
#else
        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));
#endif

        // Now we are building response we don't access request anymore since they share the same buffer.  You could have set an overlap offset on StartSession if you still require some access like headers, cookies etc.
        s.response.HeaderComplete();

        // Output response data + Complete
        // NOTE: The use of request during the building of response implies some offset is required and in this example it was specified previously via a call to response.SetBuffer(...) from the ".ino" file
        s.response+=T("<html><body><p>Anything to say? ");
        if (s.request.post_variable[T("text")]) {                                    // Like PHP isset($_POST["text"])
            s.response+=s.request.post_variable[T("text")]->value;
        }else {
            s.response+=T("No");
        }
        s.response+=", page ";
        if (s.request.query_variable[T("page")]) {                                    // Like PHP isset($_GET["page"])
            s.response+=s.request.query_variable[T("page")]->value;
        }else {
            s.response+=T("0");
        }
        s.response+=T("</p></body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteSomething

} } // namespace examples::ghttp_static_request_response_simple

#endif // !defined(ROUTESOMETHING_H)
