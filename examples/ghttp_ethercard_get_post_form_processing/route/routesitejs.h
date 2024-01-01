/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_post_form_processing
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTESITEJS_H)
#define ROUTESITEJS_H

namespace examples { namespace ghttp_ethercard_get_post_form_processing {

/**
 * An example implemented route for HTTP GET requests covering route "/js/site.js"
 */
class RouteSiteJS : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/js/site.js") || s.request.method!=T("get")) {
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
        s.response.Header(T("Content-Type: text/javascript"));
        s.response.HeaderComplete();

        // Output response data + Complete
        const char *sitejs_content =
            "$(function() {" \
            "    $('#method').click(function() {" \
            "        var f = $('#cform');" \
            "        var e = $(this);" \
            "        console.log(f.attr('method'));" \
            "        if (f.attr('method') == 'get') {" \
            "            $(this).html('Use POST');" \
            "            f.attr('method', 'post');" \
            "        }else {" \
            "            $(this).html('Use GET');" \
            "            f.attr('method', 'get');" \
            "        }" \
            "    });" \
            "});";
        s.response.Append(sitejs_content);
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteSiteJS

} } // namespace examples::ghttp_ethercard_get_post_form_processing

#endif // !defined(ROUTESITEJS_H)
