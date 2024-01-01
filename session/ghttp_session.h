/**
 * \file
 * Embedded HTTP request/response handling framework - Session
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_SESSION_H
#define GHTTP_SESSION_H

namespace ghttp {

/**
 * A container class for a session object wrapping user supplied ADT accessible during session use and a related Session ID (which you can define or use 
 * \ref SessionId)
 *
 * Possible example might be:  Session<Global, SessionId<SESSIONID_LENGTH>, 600>
 *
 * \attention Define GHTTP_SESSIONMGR_INCLUDE_MAC as 1 if you want sessions related to MAC/IP combo otherwise will be IP
 *
 * \tparam UT User type/class for session storage of custom, application specific data
 * \tparam ST Session ID class used to relate session data at runtime
 * \tparam TO Timeout period, Seconds
 */
template<class UT, class ST, uint16_t TO>
class Session {
public:
    UT    user_data;                                ///< Session user data

public:
    /**
     * Default constructor, make stable instance
     */
    Session() : last_use_(0) { }


    /**
     * Invoked by SessionManager to clear session data.  Normally invoked at start of new session or when a session is re-used due to timeout.  Everything that 
     * relates this session to its user data is cleared, along with the user data itself.
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void Clear(const uint8_t session_idx) {
        if (IsInUse()) {
            user_data.Stop(session_idx);
        }
        last_use_ = 0;
        ip4_.Clear();
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
        mac_.Clear();
#endif
        sid_.Clear();
        user_data.Clear(session_idx);
    }


    /**
     * Get elapsed time between now and session last use
     *
     * \return Seconds
     */
    uint16_t GetLastUse() const {
        return last_use_;
    }


    /**
     * Set elapsed time between now and session last use
     *
     * \param[in] last_use Seconds
     */
    void LastUse(const uint16_t last_use) {
        last_use_ = last_use;
    }


    /**
     * Expire process is a service call invoked periodically by SessionManager on a per Second basis.  It handles the case where a session should clear itself due to timeout
     *
     * \attention Define SESSION_EXPIRE_TIME to set your own application specific timeout. 0 => no timeout
     *
     * \param[in] session_idx Session index of this session within SessionManager
     */
    void ExpireProcess(const uint8_t session_idx) {
        if (TO && IsInUse()) {
            // Has session[i] expired?
            if (GetLastUse() >= TO) {
                Clear(session_idx);    // Destroy(...)
            }else {
                last_use_++;
#if defined(_MSC_VER)
                printf("[%u] = last_use_ %u\n", session_idx, last_use_);
#endif
            }
        }
    }


    /**
     * Query session used state
     *
     * \note A session not in-use will have a 0 length session id string
     *
     * \retval true When session in use
     * \retval false When session not used
     */
    bool IsInUse() const {
        return strlen(sid_.String())?true:false;
    }


    /**
     * Get session ID
     *
     * \return Reference to related session id instance
     */
    ST& SessionId() {
        return sid_;
    }


    /**
     * Get IP
     *
     * \return Reference to related IP4_t
     */
    const IP4_t& IP() {
        return ip4_;
    }


    /**
     * Set IP
     *
     * \param[in] ip Reference to IP4_t
     */
    void IP(const IP4_t& ip) {
        ip4_ = ip;
    }


#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    /**
     * Get MAC
     *
     * \return Reference to related MAC_t
     */
    const MAC_t& MAC() {
        return mac_;
    }


    /**
     * Set MAC
     *
     * \param[in] mac Reference to MAC_t
     */
    void MAC(const MAC_t& mac) {
        mac_ = mac;
    }
#endif
    
protected:
    uint16_t    last_use_;                          ///< Seconds since last use
    ST          sid_;                               ///< SID from/for request
    IP4_t       ip4_;                               ///< Client IP
#if GHTTP_SESSIONMGR_INCLUDE_MAC == 1
    MAC_t       mac_;                               ///< Client MAC (might be our router/gateway is over WAN)
#endif

}; // class Session

}; // namespace ghttp

#endif // GHTTP_SESSION_H
