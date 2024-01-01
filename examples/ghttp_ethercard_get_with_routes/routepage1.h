/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_with_routes
 * 
 * DG, 2020
 */

#if !defined(ROUTEPAGE1_H)
#define ROUTEPAGE1_H

namespace examples { namespace ghttp_ethercard_get_with_routes {

/**
 * An example implemented route for HTTP GET requests covering route "/page1"
 */
class RoutePage1 : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/page1") || s.request.method!=T("get")) {
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
        s.response+=T("<html><body><p>Page1: Counter ");
        s.response.AppendUInt<uint16_t>(s.counter.page1);
        s.response+=T("</p><div><a href='/'>Root</a></div><div><a href='/page2'>Page2</a></div></body></html>");
        s.response.Complete();

        s.counter.page1++;
        
        return true; // We have handled
    }
}; // class RoutePage1

} } // namespace examples::ghttp_ethercard_get_with_routes

#endif // !defined(ROUTEPAGE1_H)
