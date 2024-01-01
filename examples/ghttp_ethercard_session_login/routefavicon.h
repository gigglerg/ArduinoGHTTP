/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_session_login
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEFAVICON_H)
#define ROUTEFAVICON_H

#include "resource/favicon.h"

namespace examples { namespace ghttp_ethercard_session_login {

/**
 * An example implemented route for HTTP GET requests on "/favicon.ico" ()
 */
class RouteFavIcon : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/favicon.ico") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Use this helper to get session globals + set status
        auto& session_vars = ghttp::HTTPHelper::StartSession<Shared, Global>(s, 0, NULL, s.misc.response_buffer);

        // Set status + output headers + raw image data
        s.response.Header(T("HTTP/1.1"));
        s.GenericHeader();
        s.response.Header(T("Content-Type: image/png"));
        s.response.HeaderComplete();
        s.response.Append(__resource_favicon_png, sizeof(__resource_favicon_png));
        s.response.Complete();

        return true; // We have handled
    }
}; // class RouteFavIcon

} } // namespace examples::ghttp_ethercard_session_login

#endif // !defined(ROUTEFAVICON_H)
