/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_request_response_session_simple
 * 
 * DG, 2020
 */

#if !defined(ROUTECOUNTER_H)
#define ROUTECOUNTER_H

namespace examples { namespace ghttp_static_request_response_session_simple {

/**
 * An example implemented route for protocol HTTP, POST requests covering route "/counter".  You would break down your application into a number of
 * OOD route handlers like this to simplify implementation
 */
class RouteCounter : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/counter") || s.request.method!=T("post") || s.request.protocol!=T("http")) {
            // Not handled
            return false;
        }

#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
        // Use this helper to get session globals + set status
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s,100);


        // Incrememnt a counter added to my session instance of Global class
        session_vars.count++;

        if (s.request.query_variable[T("clear")]) {                        // Like PHP isset($_GET["clear"])
            session_vars.count=0;                                        // Like PHP $_SESSION['count']++
        }

#else
        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));
        
        // Tell our response instance to use the same buffer as request to reduce memory footprint
        sd.response.SetMaximum(sizeof(buffer));
        sd.response.SetBuffer(buffer, 0);
#endif

        // No more response headers to come
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Session counter ");
#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
        s.response+=ghttp::StringHelper::FromUInt<uint8_t>(session_vars.count, s.response.GetBuffer() + s.response.GetLength());
        // or
        // s.response.AppendUInt<uint8_t>(session_vars.count)
#else
        s.response+=T("0 (NO SESSION)");
#endif
        s.response+=T("</p></body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteCounter

} } // namespace examples::ghttp_static_request_response_session_simple

#endif // !defined(ROUTECOUNTER_H)
