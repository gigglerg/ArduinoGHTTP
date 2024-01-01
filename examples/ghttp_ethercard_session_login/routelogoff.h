/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_session_login
 * 
 * DG, 2020
 */

#if !defined(ROUTELOGOFF_H)
#define ROUTELOGOFF_H

namespace examples { namespace ghttp_ethercard_session_login {

/**
 * An example implemented route for HTTP GET requests covering route "/logoff"
 */
class RouteLogoff : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/logoff") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Use this helper to get session globals + set status
        uint8_t session_index;
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s, 0, NULL, s.misc.response_buffer, &session_index);

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));
        s.GenericHeader();

        // If logged in, redirect to our root route
        if (session_vars.logged_in) {

            // I could just set flag logged_in to false but it is more secure to not reuse this session directly by asking session manager to destroy it
            // NOTE: You could use s.session_manager.Destroy(s.request.session), be careful as response started so may have squashed the original request session id string
            s.session_manager.Destroy(session_index);

            s.response.HeaderComplete();        // No more headers

            // Output response data + Complete
            s.response+=T("<html><body><p>You are no longer logged in</p>");
            s.response+=T("<div><form action='/' method='get'><input type='submit' value='Root'/></form></div></body></html>");
            s.response.Complete();
            return true;
        }

        ghttp::HTTPHelper::Redirect<Shared>(s, "/");

        return true; // We have handled
    }
}; // class RouteLogoff

} } // namespace examples::ghttp_ethercard_session_login

#endif // !defined(ROUTELOGOFF_H)
