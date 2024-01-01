/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_with_routes
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEPAGE2_H)
#define ROUTEPAGE2_H

namespace examples { namespace ghttp_ethercard_get_with_routes {

/**
 * An example implemented route for HTTP GET requests covering route "/page2"
 */
class RoutePage2 : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/page2") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Set response buffer
        s.response.SetBuffer(s.misc.response_buffer, 0);

        // Set status.  Default is OK, no change here
        s.response.Header(T("HTTP/1.1"));

        // Now we are building response we don't to the request anymore since they share the same buffer
        s.response.HeaderComplete();

        // Output response data + Complete
        s.response+=T("<html><body><p>Page2: Counter ");
        s.response.AppendUInt<uint16_t>(s.counter.page2);
        s.response+=T("</p><div><a href='/'>Root</a></div><div><a href='/page1'>Page1</a></div></body></html>");
        s.response.Complete();

        s.counter.page2++;
        
        return true; // We have handled
    }
}; // class RoutePage2

} } // namespace examples::ghttp_ethercard_get_with_routes

#endif // !defined(ROUTEPAGE2_H)
