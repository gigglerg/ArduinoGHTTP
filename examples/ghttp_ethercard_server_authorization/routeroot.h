/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_server_authorization
 * 
 * DG, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_server_authorization {

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

        // Set response buffer
        s.response.SetBuffer(s.misc.response_buffer, 0, 256); // Finger in the air, 256 character overlap between end of request and start of response to allow response building use of request fields

        // Set status OK.  We are doing this because response instance is reused so may have last status of unauthorised
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        // Authentication with Apache, these details would be under your sites ".htpasswd" file but here we simply use hardcoded strings as an example

        // Has user post form content? (case sensitive hence the use of StrNCmp(...) rather than == operator).  Length+1 => NULL included otherwise "max2" "powerful" would be accepted
        if (s.request.authorization_username.StrNCmp(T("max"), true, 4) || s.request.authorization_password.StrNCmp(T("power"), true, 6)) {
            // Authentication failed
            s.response.Header(T("WWW-Authenticate: Basic realm='You must authenticate with server', charset='iso-8859-3'"));
            ghttp::HTTPHelper::Redirect<Shared>(s, "/", ghttp::HTTPResponseStatus::STATUS_UNAUTHORIZED);
            return true;
        }
        // Authenticated...

        s.response.HeaderComplete();
        // Output response data + Complete
        s.response+=T("<html><body><p>Root: You've authenticated with this server</p></body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_server_authorization

#endif // !defined(ROUTEROOT_H)
