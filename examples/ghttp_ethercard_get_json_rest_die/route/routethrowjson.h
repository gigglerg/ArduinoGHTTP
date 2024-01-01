/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_json_rest_die
 * 
 * DG, 2020
 */

#if !defined(ROUTETHROWJS_H)
#define ROUTETHROWJS_H

namespace examples { namespace ghttp_ethercard_get_json_rest_die {

/**
 * An example implemented route for HTTP GET requests covering route "/json/throw"
 */
class RouteThrowJSON : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        // Set response maximum to be size of buffer
        if (s.request.url!=T("/json/throw") || s.request.method!=T("post")) {
            // Not handled
            return false;
        }

        uint8_t die_count = 1;

        // Like PHP isset($_POST["req"]) && $_POST["req"] == "req" && isset($_POST["die"])
        if (s.request.post_variable[T("req")] && (*s.request.post_variable[T("req")]) == T("throw") && s.request.post_variable[T("die")]) {
            auto *dv = s.request.post_variable[T("die")]->value.Get();
            if (dv) {
                uint8_t dc = dv[0] - '0';
        
                // Take die count from client
                die_count = dc>0 && dc<8 ? dc : 1;
            }
        }
        
        // Set response maximum to be size of buffer - no offset as not accessing request while building response
        s.response.SetMaximum(s.request.GetMaximum());

        // Set response buffer
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0);  // Don't use request after this...

        // Set status OK
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));
        s.response.Header(T("Content-Type: application/json; charset=iso-8859-1"));
        s.response.HeaderComplete();
        s.response.Append("[");
        for(uint8_t i=0; i<die_count; i++) {
            if (i) {
                s.response.Append(",");
            }

            // Generate die[i] face value (dots) and roll direction/orientation (0 or 1)
            uint8_t face_value = 1 + (rand() % 6);
            uint8_t face_direction = rand() % 2;
            
            s.response.Append("{\"value\":");
            s.response.AppendUInt<uint8_t>(face_value);
            s.response.Append(", \"dir\":");
            s.response.AppendUInt<uint8_t>(face_direction);
            s.response.Append("}");
        }
        s.response.Append("]");
        s.response.Complete();

        return true; // We have handled
    }
}; // class RouteThrowJSON

} } // namespace examples::ghttp_ethercard_get_json_rest_die

#endif // !defined(ROUTETHROWJS_H)
