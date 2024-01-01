/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_session_login
 * 
 * DG, 2020
 */

#if !defined(ROUTELOGIN_H)
#define ROUTELOGIN_H

namespace examples { namespace ghttp_ethercard_session_login {

/**
 * An example implemented route for HTTP GET/POST requests covering route "/login"
 */
class RouteLogin : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/login") || (s.request.method!=T("get") && s.request.method!=T("post"))) {
            // Not handled
            return false;
        }

        // Use this helper to get session globals + set status.  NOTE: arbitrary 164 characters of request still available while response building
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s, 164, "path=/", s.misc.response_buffer);

        bool failed_login = false;

        // Has user post form content?
        if (s.request.post_variable[T("user")] && s.request.post_variable[T("pass")]) {        // Like PHP isset($_POST["user"]) && isset($_POST["pass"])
            // WARNING: These type of string comparisons are NOT case sensitive.  See example ghttp_ethercard_server_authorization.ino where they are
            if (strlen(&s.request.post_variable[T("user")]->value)>0 && s.request.post_variable[T("pass")]->value==T("donotask")) {
                // Authenticated...
                session_vars.logged_in = true;
                strncpy(session_vars.username, &s.request.post_variable[T("user")]->value, sizeof(session_vars.username)-1);
                session_vars.username[sizeof(session_vars.username)-1] = '\0';
            }else {
                failed_login = true;
            }
        }

        // If logged in, redirect to our root route
        if (session_vars.logged_in) {
            s.response.Header(T("HTTP/1.1"));
            s.GenericHeader();
            ghttp::HTTPHelper::Redirect<Shared>(s, "/");
            
            // We didn't handle content but did issue the redirect so indicate route handled
            return true;
        }

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));
        s.GenericHeader();
        s.response.HeaderComplete();        // No more headers

        // Output response data + Complete
        const char* content =    "<html><head><link rel='shortcut icon' type='image/png' href='/favicon.ico'/></head>" \
                                "<body><form action='/login' method='post'>" \
                                "<label for='user'>Username:</label><input type='text' name='user' value='' maxlength='15' required/>" \
                                "<label for='pass'>Password:</label><input type='password' name='pass' value='' maxlength='15' required/>" \
                                "<div><input type='submit' value='submit'/></div>" \
                                "</form></div>";
        s.response.Append(content);
        if (failed_login) {
            s.response.Append(T("<div style='color:red;background-color:black;'>ERROR: Login attempt failed</div>"));
        }
        s.response+=T("</body></html>");
        s.response.Complete();

        return true; // We have handled
    }
}; // class RouteLogin

} } // namespace examples::ghttp_ethercard_session_login

#endif // !defined(ROUTELOGIN_H)
