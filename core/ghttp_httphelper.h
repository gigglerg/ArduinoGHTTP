/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP helper
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPHELPER_H
#define GHTTP_HTTPHELPER_H

namespace ghttp {


#if GHTTP_USE_HELPERS == 1
/**
 * A class with application helpers grouped together offering static methods to reduce repeated work performed by classes implementing \ref Route
 */
class HTTPHelper {
public:
    /**
     * Build a valid redirect HTTP response using your application's HTTPResponse instance
     *
     * \pre Caller should have setup response maximum and buffer length to suit
     *
     * \note If you specify a status code than you are responsible for ensuring there is no content as per the status you've chosen
     *
     * \note Older versions of HTTP require the relocation location to be absolute.  HTTP1.1 onward allow relative locations (those found on this server, 
     * example: "/some/place/here" as opposed to "http://10.0.0.1/some/place/here").  Either include in location string or use flag prepend_local_ip
     *
     * \tparam T Class of your application specific shared data
     * \param[in] s Reference to shared instance (application specific)
     * \param[in] location String pointer (NULL terminated) for redirect location
     * \param[in] status HTTP response status code, default 0 => HTTPResponseStatus::STATUS_FOUND if the current status isn't HTTPResponseStatus::STATUS_CREATED or a 3XX code
     * \param[in] prepend_local_ip Flag indicating location should be absolute with servier IP prepended as in "<ip>[/]<location>".  Default false
     * \param[in] protocol_and_version String pointer to supported protocol and version, like "HTTP/1.0".  Default NULL => "HTTP/1.1"
     * \return State
     * \retval false Failure occured during header or content setup
     * \retval true Success
     */
    template<class T>
    static bool Redirect(T &s, char *location, const uint16_t status = 0U, const bool prepend_local_ip = false, char *protocol_and_version = NULL) {
        bool set = false;
        uint16_t current_status = s.response.status.Get();
        char ip4_str[16] = { '\0' }; // Format "xxx.xxx.xxx.xxx\0"

        if (!status) {
            // Caller didn't specify a redirect so use default and ensure no content
            s.response.NoContent();
        }

        if (!status) {
            if (HTTPResponseStatus::STATUS_CREATED == current_status || (current_status>=300 && current_status<400)) {
                // Accept existing status
            }else {
                s.response.status.Set(HTTPResponseStatus::STATUS_FOUND);
            }
        }else {
            if (current_status |= status) {
                s.response.status.Set(status);
            }
        }

        // Add status header
        if (!protocol_and_version) {
            set = s.response.Header(T("HTTP/1.1"));
        }else {
            set = s.response.Header(protocol_and_version);
        }

        // Add location header
        if (set && location) {
            // Are headers complete?  Undo complete.  This is safe because we know there is no content
            if (s.response.GetEndOfHeaders()) {
                s.response.SetLength( s.response.GetLength() - 2 );
            }
            set = s.response.HeaderRemove(T("Location"));    // Checks EoH, only true of not

            if (set) {
                set = s.response.Append(T("Location: "));
                if (set) {
                    if (prepend_local_ip) {
                        set = s.response.Append(T("http://"));
                        s.server.ip4.String(ip4_str);
                        set = s.response.Append(ip4_str);
                        if ('/' != location[0]) {
                            set = s.response.Append(T("/"));
                        }
                    }
                    set = s.response.Append(location);
                    if (set) {
                        set = s.response.Append(T("\r\n"));
                    }
                }
            }
        }
        s.response.HeaderComplete();
        s.response.Complete();

        return set;
    } // Redirect(...)


#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
    /**
     * Start session.  Uses request session cookies to find related session variables, otherwises gives new session variables.  Setup response buffer ready for reply building along 
     * with setting the inital response status to HTTPResponseStatus::STATUS_OK and setting response session cookie header
     *
     * \tparam T Class of your application specific shared data
     * \tparam G Application specific global session data class used to encapsulate whatever session data you require
     * \param[in] s Reference to shared instance (application specific)
     * \param[in] response_offset Offset (characters) in buffer between response building start and request.  Anything larger than 0 implies some request information maybe required 
     * while building response.  Default is 0 => build response over existing request
     * \param[in] session_cookie_addition String pointer (NULL terminated) for session cookie addition (if any), example "HttpOnly".  Default NULL (nothing)
     * \param[in] response_buffer Optional response buffer where it isn't directly the same as request
     * \param[out] session_index Optional pointer to integer for session index from Session Manager (default NULL).  This index can be used with Session Manager api
     * \return S
     */
    template<class T, class G>
    static G& StartSession(T &s, const uint16_t response_offset = 0, const char* session_cookie_addition=NULL, char *response_buffer=NULL, uint8_t *session_index=NULL) {
        // Setup response
        s.response.SetMaximum(s.request.GetMaximum() - response_offset);
        if (response_buffer) {
            s.response.SetBuffer(response_buffer, 0, response_offset);
        }else {
            s.response.SetBuffer(s.request.GetBuffer(), 0, response_offset);
        }

        // Start session and get session variables
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 0
        auto &session = s.session_manager.StartSession(s.client.ip4, s.request.session.Get(), session_index);
#else // GHTTP_SESSIONMGR_INCLUDE_MAC != 0
        auto &session = s.session_manager.StartSession(s.client.ip4, s.client.mac, s.request.session.Get(), session_index);
#endif
        G &session_vars = session.user_data;

        // Set response status
        s.response.status.Set(HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        // Set session cookie.  Headers shouldn't be complete since we're starting them but should check
        if (!s.response.GetEndOfHeaders()) {
            if (s.response.Append(T("Set-Cookie: "))) {
                if (s.response.Append(GHTTP_SERVER_STRING_SESSION_COOKIE)) {
                    if (s.response.Append(T("="))) {
                        if (s.response.Append(session.SessionId().String())) {
                            if (session_cookie_addition && strlen(session_cookie_addition)) {
                                if (s.response.Append(T("; "))) {
                                    s.response.Append(session_cookie_addition);
                                }
                            }
                            s.response.Append(T("\r\n"));
                        }
                    }
                }
            }
        }

        return session_vars;
    } // StartSession(...)
#endif // GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
}; // class HTTPHelper
#endif // GHTTP_USE_HELPERS == 1

}; // namespace ghttp

#endif // GHTTP_HTTPHELPER_H
