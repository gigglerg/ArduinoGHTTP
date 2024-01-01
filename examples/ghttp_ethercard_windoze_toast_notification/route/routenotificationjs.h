/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_windoze_toast_notification
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTENOTIFICATIONJS_H)
#define ROUTENOTIFICATIONJS_H

namespace examples { namespace ghttp_ethercard_windoze_toast_notification {

/**
 * An example implemented route for HTTP GET requests covering route "/js/notification.js"
 */
class RouteNotificationJs : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/js/notification.js") || s.request.method!=T("get")) {
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

        s.response.Header(T("Content-Type: application/javascript; charset=iso-8859-3"));
        s.response.HeaderComplete();
        // Output response data + Complete
        const char* content = "window.onload = function() {\n" \
            "    window.notify = {\n" \
            "        list: [],\n" \
            "        id: 0,\n" \
            "        log: function(msg) {\n" \
            "            var ta = document.getElementById('talog');\n" \
            "            ta.innerHTML += ('\\n'+msg);\n" \
            "            ta.scrollTop = ta.scrollHeight;\n" \
            "        }," \
            "        compatible: function() {\n" \
            "            if (typeof Notification === 'undefined') {\n" \
            "                return false;\n" \
            "            }\n" \
            "            return true;\n" \
            "        },\n" \
            "        authorise: function() {\n" \
            "            if (notify.compatible()) {\n" \
            "                Notification.requestPermission(function(permission) {\n" \
            "                    notify.log('Permission to display: '+permission);\n" \
            "                });\n" \
            "            }\n" \
            "        },\n" \
            "        send: function() {\n" \
            "            if (typeof Notification === 'undefined') { notify.log('Notifications are not available for your browser.'); return; }\n" \
            "            if (notify.compatible()) {\n" \
            "                // we need a unique number for each toast notification\n" \
            "                notify.id++;\n" \
            "                var id = notify.id;\n" \
            "                notify.list[id] = new Notification('Notification #'+id, {\n" \
            "                    body: 'This is the body of a notification. \\nYou can put anything you want here?',\n" \
            "                    tag: id,\n" \
            "                    icon: 'image/toast.jpg',\n" \
            "                    lang: '',\n" \
            "                    dir: 'auto',\n" \
            "                });\n" \
            "                notify.list[id].onclick = function() { notify.logEvent(id, 'clicked'); };\n" \
            "                notify.list[id].onshow  = function() { notify.logEvent(id, 'showed');  };\n" \
            "                notify.list[id].onerror = function() { notify.logEvent(id, 'errored'); };\n" \
            "                notify.list[id].onclose = function() { notify.logEvent(id, 'closed');  };\n" \
            "                console.log('Created a new notification ...');\n" \
            "                console.log(notify.list[id]);\n" \
            "            }else {\n" \
            "                notify.log('Notifications are not available for your client');\n" \
            "            }\n" \
            "        },\n" \
            "        logEvent: function(id, event) {\n" \
            "            notify.log('Notification #'+id+' '+event);\n" \
            "        }\n" \
            "    };\n" \
            "};\n";
        s.response.Append(content);
        s.response.Complete();
        return true;
    }
}; // class RouteNotificationJs

} } // namespace examples::ghttp_ethercard_windoze_toast_notification

#endif // !defined(ROUTENOTIFICATIONJS_H)
