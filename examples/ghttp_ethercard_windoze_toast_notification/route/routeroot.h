/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_windoze_toast_notification
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_windoze_toast_notification {

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

        // Set response maximum to be size of buffer.  This isnt right but enough for this example, it should be -TCP/IP headers
        s.response.SetMaximum(s.request.GetMaximum());

        // Set response buffer
        s.response.SetBuffer(s.misc.response_buffer, 0, 0);  // Don't use request after this...

        // Set status OK
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));
        s.response.HeaderComplete();
        // Output response data + Complete
        const char* content =    "<html>" \
                                "<head><script type='text/javascript' src='js/notification.js'></script></head>" \
                                "<body>" \
                                "<h2>HTML5 Web Notifications From H/W Device</h2>" \
                                "<a href='https://www.w3.org/TR/notifications/#activating-a-notification' title='W3C notifications'>Source: Notifications from W3C</a>" \
                                "<img alt='toast' src='image/toast.jpg' style='float:right;width:100px;height:100px;'/>" \
                                "<div><button onclick='notify.authorise()'>Authorise Notification</button></div>" \
                                "<div><button onclick='notify.send()'>Send Notification</button></div>" \
                                "<p><strong>NOTE:</strong>This feature only works in Edge.  Chrome requires SSL certificate</p>" \
                                "<div><textarea id='talog' readonly rows='25' cols='70'>Log:</textarea>" \
                                "</body>" \
                                "</html>";
        s.response.Append(content);
        s.response.Complete();
        return true;
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_windoze_toast_notification

#endif // !defined(ROUTEROOT_H)
