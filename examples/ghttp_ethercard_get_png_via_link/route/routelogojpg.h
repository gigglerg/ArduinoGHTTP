/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_png_via_link
 * 
 * Dave.C, 2020
 */


#if !defined(ROUTELOGOJPG_H)
#define ROUTELOGOJPG_H

// Include Ethercard wrap as we are going to send a large block data directly from on-board flash as it is to large for our request/response buffer
#include "../ecwrap.h"

// Include our JPG binary array from header.  It has to fit within our response buffer
#include "../resource/ghttp_chrome.h"

namespace examples { namespace ghttp_ethercard_get_png_via_link {

/**
 * An example implemented route for HTTP GET requests on "image/logo.jpg"
 */
class RouteLogoJpg : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/image/logo.jpg") || s.request.method!=T("get")) {
            // Not handled
            return false;
        }

        // Set response buffer
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0);  // Don't use request after this...

        // Set status OK
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        s.response.Header(T("Content-Type: image/jpeg"));
        /*
         * Here we simply alter the size of the response to include raw image data length.  This will alter the headers but actual buffering 
         * isn't present due to size.  So we aren't going to use the SRAM buffer for anything other than the headers.
         */
        uint16_t raw_image_length = static_cast<uint16_t>(sizeof(__resource_ghttp_chrome_jpg));
        
        char content_length[24] = "Content-Length: ";
        ghttp::StringHelper::FromUInt<uint16_t>(raw_image_length, &content_length[16]);
        s.response.Header(content_length);
        s.response.HeaderComplete();
        s.response.Complete(false); // false as we've added our own content length, ignore anything in response buffer

        /*
         * The send will be broken down into 2, first the response headers, second the raw image data
         */
        ECWrap::Send(s.response.GetBuffer(), s.response.GetLength(), true, true);
        
        // STM32 flash accessible by same bus as SRAM
        ECWrap::Send(reinterpret_cast<const char*>(&__resource_ghttp_chrome_jpg[0]), raw_image_length, false);

        /*
         * Last important point.  Because we've handled the request and actually sent the response we don't want the main application to do 
         * anything with response buffer so I set the length to 0.  This is application specific and just how I've chosen to signal this fact
         * but you're free to do what you like...
         */
        s.response.SetLength(0);

        return true; // We have handled
    }
}; // class RouteLogoJpg

} } // namespace examples::ghttp_ethercard_get_png_via_link

#endif // !defined(ROUTELOGOJPG_H)
