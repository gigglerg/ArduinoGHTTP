/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_png_via_link
 * 
 * DG, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_get_png_via_link {

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
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0); // Cannot use request anymore as no offset which is fine for this route

        // Set status OK.  We are doing this because response instance is reused so may have last status of unauthorised
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));
        s.response.HeaderComplete();

        // Output response data + Complete
        const char *content =
            "<html><head>" \
            "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>" \
            "<script>" \
            "$(window).on('load', function() {" \
            "    $( '#logo' ).fadeIn( 'slow', function() {" \
            "        $( '#logo' ).slideUp( function() {" \
            "            $( '#logo' ).slideDown( function() {" \
            "            });" \
            "        });" \
            "    });" \
            "});" \
            "</script>" \
            "<style>" \
            "body { display: block;margin-left: auto;margin-right: auto;width: 50%; }" \
            "img { display: none; position: relative; top: 50%; transform: translateY(-50%); }" \
            "</style>" \
            "</head>" \
            "<body>" \
            "<img src='/image/logo.jpg' alt='ghttp logo' width=989 height=269 id='logo' />" \
            "</body></html>";
        s.response.Append(content);
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_get_png_via_link

#endif // !defined(ROUTEROOT_H)
