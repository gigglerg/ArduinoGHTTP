/**
 * \file
 * Embedded HTTP request/response handling framework - Session Id
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_SESSIONID_H
#define GHTTP_SESSIONID_H

namespace ghttp {

/**
 * A class representing a session ID with helpers for comparison checking, length and session id string storage
 *
 * \tparam N Session id string length, characters (including NULL)
 */
template<uint8_t N>
class SessionId {
public:
    /**
     * Default constructor, make stable instance
     */
    SessionId() {
        Clear();
    }
    

    /**
     * Constructor accepting source ASCII string which is copied to internal session string, also makes stable instance
     *
     * \param[in] s Source session id string (NULL terminated)
     */
    SessionId(const char* s) {
        if (s) {
            strncpy(sid_, s, N);
            sid_[N-1] = '\0';
        }else {
            Clear();
        }
    }


    /**
     * Clear session id string making it 0 characters in length
     */
    void Clear() {
        sid_[0] = '\0';
    }


    /**
     * Get size of session id string.  This is fixed to template N characters
     *
     * \return Size characters (including NULL)
     */
    uint8_t Size() const {
        return N;
    }


    /**
     * Generate session id
     *
     * \note Uses macro \ref GHTTP_SESSIONRND_GENERATE
     */
    void Generate() {
        // The problem is we don't want 2 sids the same and here we assign it then later some code would check for duplicates
        strncpy(sid_, GHTTP_SESSIONRND_GENERATE(N), N);
        sid_[N-1] = '\0';
    }


    /**
     * Get session id string
     *
     * \return String pointer (internal storage, NULL terminated)
     */
    const char* String() const {
        return &sid_[0];
    }


    /**
     * Equals operator, populate intenal session string with given string
     *
     * \param[in] s Source string pointer (NULL terminated)
     * \return This reference
     */
    SessionId<N>& operator=(const char* s) {
        strncpy(sid_, s, N);
        sid_[N-1] = '\0';

        return *this;
    }


    /**
     * Equals operator, populate intenal session string with given SessionId instance
     *
     * \param[in] o Source SessionId reference
     * \return This reference
     */
    SessionId<N>& operator=(SessionId<N>& o) {
        if (o!=*this) {
            strncpy(sid_, o.sid_, N);            
        }

        return *this;
    }


    /**
     * Equals comparison operator, check session string against given ASCII
     *
     * \param[in] rhs ASCII session string
     * \retval true when session id's match
     * \retval false when session id's differ
     */
    bool operator==(const char* rhs) const {
        bool eq = false;
        if (rhs) {
            uint16_t rhs_l = strlen(rhs);
            uint16_t l = strlen(sid_);

            if (l == (N-1) && l == rhs_l) {
                if (0 == StringHelper::StrNCmp(sid_, rhs, l)) {
                    eq = true;
                }
            }
        }

        return eq;
    }


    /**
     * Not equals comparison operator, check session string against given ASCII
     *
     * \param[in] rhs ASCII session string
     * \retval true when session id's differ
     * \retval false when session id's match
     */
    bool operator!=(const char* rhs) const {
        return !(*this == rhs);
    }


    /**
     * Equals comparison operator, check session string against given SessionId instance
     *
     * \param[in] rhs SessionId reference to check against
     * \retval true when session id's match
     * \retval false when session id's differ
     */
    bool operator==(const SessionId<N>& rhs) const {
        return *this == &rhs.sid_[0];
    }


    /**
     * Not equals comparison operator, check session string against given SessionId instance
     *
     * \param[in] rhs SessionId reference to check against
     * \retval true when session id's differ
     * \retval false when session id's match
     */
    bool operator!=(const SessionId<N>& rhs) const {
        return !(*this == rhs);
    }

protected:
    char         sid_[N];                           ///< Session id string storage
}; // class SessionId

}; // namespace ghttp

#endif // GHTTP_SESSIONID_H
