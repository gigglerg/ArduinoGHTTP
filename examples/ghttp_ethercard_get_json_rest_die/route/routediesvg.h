/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_json_rest_die
 * 
 * DG, 2020
 */

#if !defined(ROUTEDIESVG_H)
#define ROUTEDIESVG_H

// Include our SVG binary array from header.  It has to fit within our response buffer
#include "../resource/die_optimised.h"


namespace examples { namespace ghttp_ethercard_get_json_rest_die {

/**
 * An example implemented route for HTTP GET requests covering route "/image/die_optimised.svg"
 */
class RouteDieSVG : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // Set response maximum to be size of buffer
        if (s.request.url!=T("/image/die_optimised.svg") || s.request.method!=T("get")) {
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
        s.response.Header(T("Content-Type: image/svg+xml"));
        // Content length here isn't important
        char content_length[24] = "Content-Length: ";
        ghttp::StringHelper::FromUInt<uint16_t>(static_cast<uint16_t>(sizeof(__resource_die_optimised_svg)), &content_length[16]);
        s.response.Header(content_length);
        s.response.HeaderComplete();
        s.response.Append(reinterpret_cast<const char*>(&__resource_die_optimised_svg[0]), sizeof(__resource_die_optimised_svg));
        s.response.Complete(false);

        return true; // We have handled
    }
}; // class RouteDieSVG

} } // namespace examples::ghttp_ethercard_get_json_rest_die

#endif // !defined(ROUTEDIESVG_H)
