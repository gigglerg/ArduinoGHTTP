/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_static_request_response_simple
 * 
 * DG, 2020
 */

#if !defined(GLOBAL_H)
#define GLOBAL_H

/* Only required if sessions enabled */
#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1

namespace examples { namespace ghttp_static_request_response_simple {

/**
 * An example session user data class and in this case a class representing application specific globals.  During request handling 
 * with cookies and sessions, an instance of this class is associated to each.  This very much like PHP $_SESSION
 *
 * \note I've called this class Global to help your understanding, it could be called anything as it's a template parameter to Session
 */
class Global {
public:
    /*
     * Request counter.  This isn't shared amongst all request, only those within the same session
     */
    uint8_t    count;

public:
    /*
     * Default constructor, make stable instance
     *
     * \note Initial state should have everything at defaults ready to be used
     */
    Global() {
        Clear();
    }


    /*
     * Invoked by SessionManager to clear session data before \ref Start
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Clear(const uint8_t session_idx) {
        // We should clear or set our session specific default values here...
        count = 0;
    }


    /*
     * Invoked by SessionManager at start of new session.  \ref Clear would have been invoked first as objects are reused the constructor is only invoked once so you can 
     * consider this as a dynamic constructor invoke where you can alter globals before handling the initial request
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Start(const uint8_t session_idx) {
        // This application doesn't need to do anything when this global instance is assigned or started to be used
    }


    /*
     * Invoked when related session is stale and no longer used when handling new requests or if it is about to be re-used by SessionManager with a \ref Clear and \ref Start
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Stop(const uint8_t session_idx) {
        // This application doesn't need to do anything when this global instance is retired
    }


    /*
     * Invoked by SessionManager upon next request using related session
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Continue(const uint8_t session_idx) {
        // This application doesn't need to do anything when this global instance is reused between requests of the same session
    }
}; // class Global

} } // namespace examples::ghttp_static_request_response_simple

#endif // GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1

#endif // !defined(GLOBAL_H)
