/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_post_form_processing
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTESITECSS_H)
#define ROUTESITECSS_H

namespace examples { namespace ghttp_ethercard_get_post_form_processing {

/**
 * An example implemented route for HTTP GET requests covering route "/css/site.css"
 */
class RouteSiteCSS : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/css/site.css") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Set response maximum to be size of buffer
        s.response.SetMaximum(s.request.GetMaximum());

        // Set response buffer.  Cannot use request anymore as no offset which is fine for this route
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0);

        // Set status OK.  We are doing this because response instance is reused so may have last status of unauthorised
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));
        s.response.Header(T("Content-Type: text/css"));
        s.response.HeaderComplete();

        // Output response data + Complete
        const char *sitecss_content =
            "form div { padding: 1em 1em 1em 1em; }" \
            "form div label { padding: 0 0 5 0em; }" \
            "input { padding-left: 2em; }" \
            "button { margin-top: 1em; }" \
            "label { margin-right: 1em; }" \
            "textarea { background-color: #cccccc; }" \
            ".centre { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); text-align: center; display: block; margin: auto; width: 25%; background-color: #cccccc; border: 0px; border-radius: 25% 10%; }" \
            ".centre div { padding: 1em 1em 1em 1em; }";
        s.response.Append(sitecss_content);
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteSiteCSS

} } // namespace examples::ghttp_ethercard_get_post_form_processing

#endif // !defined(ROUTESITECSS_H)
