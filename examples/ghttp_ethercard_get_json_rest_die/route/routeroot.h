/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_json_rest_die
 * 
 * DG, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

// Include our SVG binary array from header.  It has to fit within our response buffer
#include "../resource/root.h"


namespace examples { namespace ghttp_ethercard_get_json_rest_die {

/**
 * An example implemented route for HTTP GET requests covering route "/"
 */
class RouteRoot : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // Set response maximum to be size of buffer
        if (s.request.url!=T("/") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Set response maximum to be size of buffer - no offset as not accessing request while building response
        s.response.SetMaximum(s.request.GetMaximum());

        // Set response buffer
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0);  // Don't use request after this...

        // Set status OK
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));        
        s.response.HeaderComplete();
        s.response.Append(reinterpret_cast<const char*>(&__resource_root_htm[0]), sizeof(__resource_root_htm));
        s.response.Complete();

        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_get_json_rest_die

#endif // !defined(ROUTEROOT_H)
