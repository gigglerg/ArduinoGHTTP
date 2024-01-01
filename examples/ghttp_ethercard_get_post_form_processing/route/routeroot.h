/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_post_form_processing
 * 
 * DG, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_get_post_form_processing {

/**
 * An example implemented route for HTTP GET/POST requests covering route "/"
 */
class RouteRoot : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/")) {
            // Not handled
            return false;
        }

        // Set response maximum to be size of buffer - offset move over entire request, now this doesn't include s.request.url_raw but I don't want this additional field
        s.response.SetMaximum(s.request.GetMaximum() - (1+s.request.GetLength()));

        // Set response buffer
        s.response.SetBuffer(s.request.GetBuffer(), 0, (1+s.request.GetLength()));
        
        // Set status OK.  We are doing this because response instance is reused so may have last status of unauthorised
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));
        s.response.HeaderComplete();

        // Output response data + Complete
        const char *content_top =
            "<html>" \
            "    <head>" \
            "        <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>" \
            "       <!-- these are other routes on our device -->" \
            "        <script src='js/site.js'></script>" \
            "        <link rel='stylesheet' href='css/site.css' />" \
            "    </head>" \
            "    <body>" \
            "        <span class='centre'>" \
            "            <button type='button' id='method'>Use GET</button>" \
            "            <form action='/' method='get' id='cform' enctype='application/x-www-form-urlencoded'>" \
            "                <div><label for='firstname'>First Name</label><input type='text' name='firstname' id='firstname' placeholder='John' maxlength='16'/></div>" \
            "                <div><label for='lastname'>Last Name</label><input type='text' name='lastname' id='lastname' placeholder='Doe' maxlength='16'/></div>" \
            "                <div><label for='email'>Email</label><input type='email' name='email' id='email' placeholder='john@doe.com' maxlength='50'/></div>" \
            "                <div><input type='radio' id='male' name='gender' value='male' checked>" \
            "                <label for='male'>Male</label>" \
            "                <input type='radio' id='female' name='gender' value='female'>" \
            "                <label for='female'>Female</label>" \
            "                <input type='radio' id='other' name='gender' value='other'>" \
            "                <label for='other'>Other</label></div>" \
            "                <div><select name='car'>" \
            "                  <option selected value='Volvo'>Volvo</option>" \
            "                  <option value='Saab'>Saab</option>" \
            "                  <option value='Fiat'>Fiat</option>" \
            "                  <option value='Audi'>Audi</option>" \
            "                </select></div>" \
            "                <div><input type='submit' value='Send' /></div>" \
            "            </form>" \
            "            <div>" \
            "                <textarea readonly rows='10' cols='40'>";

        const char *content_bottom =
            "                </textarea>" \
            "            </div>" \
            "        </span>" \
            "    </body>" \
            "</html>";
        s.response.Append(content_top);

        // Output HTTP method in HTML
        s.response.Append(T("HTTP "));
        s.response.Append(s.request.method.Get());
    
        // Has user added get variables?
        for(uint8_t i=0; i<s.request.query_variable.count; i++) {
            auto h = s.request.query_variable.At(i);

            if (!i) {
                s.response.Append(T("\nGET Variables\n"));
            }

            if (h->Get() != NULL) {
                // Output get variable[i] in HTML
                s.response.Append(T("["));
                s.response.AppendUInt<uint8_t>(i);
                s.response.Append(T("] '"));
                s.response.Append(h->Get());
                s.response.Append(T("': '"));
                if (h->value.Get() != NULL) {
                    s.response.Append(h->value.Get());
                }
                s.response.Append(T("'\n"));
            }
        }
        
        // Has user added post variables?
        for(uint8_t i=0; i<s.request.post_variable.count; i++) {
            auto h = s.request.post_variable.At(i);

            if (!i) {
                s.response.Append(T("\nPOST Variables\n"));
            }

            if (h->Get() != NULL) {
                // Output post variable[i] in HTML
                s.response.Append(T("["));
                s.response.AppendUInt<uint8_t>(i);
                s.response.Append(T("] '"));
                s.response.Append(h->Get());
                s.response.Append(T("': '"));
                if (h->value.Get() != NULL) {
                    s.response.Append(h->value.Get());
                }
                s.response.Append(T("'\n"));
            }
        }

        s.response.Append(content_bottom);
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_get_post_form_processing

#endif // !defined(ROUTEROOT_H)
