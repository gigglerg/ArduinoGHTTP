/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_session_login
 * 
 * DG, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_session_login {

/**
 * An example implemented route for HTTP GET requests covering route "/"
 */
class RouteRoot : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Use this helper to get session globals + set status
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s, 0, "path=/", s.misc.response_buffer);

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));
        s.GenericHeader();

        // If not logged in, redirect to our login form
        if (!session_vars.logged_in) {
            ghttp::HTTPHelper::Redirect<Shared>(s, "/login");
            
            // We didn't handle content but did issue the redirect so indicate route handled
            return true;
        }

        s.response.HeaderComplete();        // No more headers

        // Output response data + Complete
        s.response+=T("<html><body><p>User &quot;");
        s.response+=session_vars.username;
        s.response+=T("&quot; is logged-in.  Session reuse counter: ");
        s.response.AppendUInt<uint16_t>(session_vars.reuse_counter);
        s.response+=T("</p><div><form action='/logoff' method='get'><input type='submit' value='Logoff'/></form></div></body></html>");
        s.response.Complete();
         
        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_session_login

#endif // !defined(ROUTEROOT_H)
