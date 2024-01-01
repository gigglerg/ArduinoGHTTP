/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_windoze_toast_notification
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTETOASTJPG_H)
#define ROUTETOASTJPG_H

#include "../resource/toastjpg.h"

namespace examples { namespace ghttp_ethercard_windoze_toast_notification {

/**
 * An example implemented route for HTTP GET requests on "image/toast.jpg"
 */
class RouteToastJpg : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/image/toast.jpg") || s.request.method!=T("get")) {
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

        s.response.Header(T("Content-Type: image/jpeg"));
        s.response.HeaderComplete();
#if defined(ARDUINO_ARCH_AVR)
        // AVR flash accessible by different bus for SRAM.  There has to be a nicer way to do this??
        s.response.Append(reinterpret_cast<PGM_VOID_P>(const_cast<char*>(__resource_toast_jpg)), sizeof(__resource_toast_jpg));
#else
        // STM32 flash accessible by same bus as SRAM
        s.response.Append(__resource_toast_jpg, sizeof(__resource_toast_jpg));
#endif
        s.response.Complete();
        
        Serial.print("Length: ");
        Serial.println(s.response.GetLength(), DEC);

        return true; // We have handled
    }
}; // class RouteToastJpg

} } // namespace examples::ghttp_ethercard_windoze_toast_notification

#endif // !defined(ROUTETOASTJPG_H)
