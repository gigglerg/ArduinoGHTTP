/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_json_rest_die
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEDIEJS_H)
#define ROUTEDIEJS_H

// Include our JS binary array from header.  It has to fit within our response buffer
#include "../resource/die.h"


namespace examples { namespace ghttp_ethercard_get_json_rest_die {

/**
 * An example implemented route for HTTP GET requests covering route "/js/die.js"
 */
class RouteDieJs : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // Set response maximum to be size of buffer
        if (s.request.url!=T("/js/die.js") || s.request.method!=T("get")) {
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
        s.response.Header(T("Content-Type: text/javascript"));
        s.response.HeaderComplete();
        s.response.Append(reinterpret_cast<const char*>(&__resource_die_js[0]), sizeof(__resource_die_js));
        s.response.Complete();

        return true; // We have handled
    }
}; // class RouteDieJs

} } // namespace examples::ghttp_ethercard_get_json_rest_die

#endif // !defined(ROUTEDIEJS_H)
