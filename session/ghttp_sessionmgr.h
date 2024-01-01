/**
 * \file
 * Embedded HTTP request/response handling framework - Session manager
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_SESSIONMGR_H
#define GHTTP_SESSIONMGR_H

namespace ghttp {

/**
 * Session manager class, containing multiple Session instances.  Includes helpers to search and control sessions
 *
 * \todo Add task for age
 *
 * \tparam S_MAX Maximum sessions handled
 * \tparam ST Session class
 * \tparam SWT StopWatch class primitive (POD) type.  If in dout use uint32_t as the type has to be able to hold
 * session timeout period
 */
template<uint8_t S_MAX, class ST, typename SWT>
class SessionManager {
public:
    /**
     * Default constructor, make stable instance
     */
    SessionManager() : session_(), service_timeout_() {
        service_timeout_.Start();
    }


#if GHTTP_SESSIONMGR_INCLUDE_MAC == 0
    /**
     * Start session (excludes MAC).  Will look up session by passed session id or assign new session if none exists.  Validates any found session against the network 
     * address detail given to ensure correct session
     *
     * \param[in] ip4 Client/session owner IP address reference
     * \param[in] existing_session_id Session ID string pointer (NULL terminated).  Default NULL.  Source HTTP request 
     * session cookie
     * \param[out] index Optional pointer to integer for session index from Session Manager (default NULL).  This index can be used with Session Manager api
     * \return Session reference
     */
    ST& StartSession(const IP4_t& ip4, const char* existing_session_id=NULL, uint8_t *index=NULL) {
        uint8_t idx = 0;

        // Look up existing session (if any)
        ST*f = FindBySessionId(existing_session_id, &idx);

        // Did we find by session id?  If so check other parameters
        if (f && ip4 == f->IP()) {
            // Reset last use timer and continue...
            f->LastUse(0);
            if (index) {
                *index = idx;
            }
            f->user_data.Continue(idx);
            return *f;
        }

        ST& r = Next(&idx);
        if (index) {
            *index = idx;
        }
        Destroy(idx);
        // Generate unique session id, ensured by repeated attempts if duplicate in set exist
        do {
            r.SessionId().Generate();
            f = FindBySessionIdWithExclude( r.SessionId().String(), &r);
        }while(f);
        r.user_data.Start(idx);
        r.IP(ip4);

        return r;
    }

#else // GHTTP_SESSIONMGR_INCLUDE_MAC != 0
    /**
     * Start session.  Will look up session by passed session id or assign new session if none exists.  Validates any found session against the network address detail given 
     * to ensure correct session
     *
     * \param[in] ip4 Client/session owner IP address reference
     * \param[in] MAC Client/session owner MAC address reference
     * \param[in] existing_session_id Session ID string pointer (NULL terminated).  Default NULL.  Source HTTP request 
     * session cookie
     * \param[out] index Optional pointer to integer for session index from Session Manager (default NULL).  This index can be used with Session Manager api
     * \return Session reference
     */
    ST& StartSession(const IP4_t& ip4, const MAC_t& mac, const char* existing_session_id=NULL, uint8_t *index=NULL) {
        uint8_t idx = 0;

        // Look up existing session (if any)
        ST*f = FindBySessionId(existing_session_id, &idx);

        // Did we find by session id?  If so check other parameters
        if (f && ip4 == f->IP() && mac == f->MAC()) {
            // Reset last use timer and continue...
            f->LastUse(0);
            if (index) {
                *index = idx;
            }
            f->user_data.Continue(idx);
            return *f;
        }

        ST& r = Next(&idx);
        if (index) {
            *index = idx;
        }
        Destroy(idx);
        // Generate unique session id, ensured by repeated attempts if duplicate in set exist
        do {
            r.SessionId().Generate();
            f = FindBySessionIdWithExclude( r.SessionId().String(), &r);
        }while(f);
        r.IP(ip4);
        r.MAC(mac);
        r.user_data.Start(idx);

        return r;
    }
#endif // GHTTP_SESSIONMGR_INCLUDE_MAC != 0


    /**
     * Get session by index
     *
     * \param[in] index Session 0>= index <S_MAX
     * \return Session reference
     */
    ST& GetSession(const uint8_t index) {
        uint8_t i = index;

        if (i>=S_MAX) {
            i = S_MAX-1;
        }

        return session_[i];
    }


    /**
     * Destory all sessions
     */
    void DestroyAll() {
        for(uint8_t i=0; i<S_MAX; i++) {
            Destroy(i);
        }
    }


    /**
     * Destory session by index
     *
     * \param[in] index Session 0>= index <S_MAX
     */
    void Destroy(const uint8_t index) {
        uint8_t i = index;

        if (i>=S_MAX) {
            i = S_MAX-1;
        }

        session_[i].Clear(i);
    }


    /**
     * Destory session by session id
     *
     * \param[in] id Pointer to session id string
     * \retval true Success
     * \retval false Failed to find session by given id string
     */
    bool Destroy(const char* id=NULL) {
        bool done = false;
        uint8_t i;

        // Look up existing session
        if (FindBySessionId(id)) {
            session_[i].Clear(i);
            done = true;
        }

        return done;
    }


    /**
     * Match existing session with given session id.  Rather than store say a CRC of session information I 
     * decided the best approach was to actually check by character string because of the nature of CRC 
     * probability
     *
     * \param[in] session_id Session id string pointer (NULL terminated)
     * \param[out] session_idx Session 0>= index <S_MAX or NULL (default)
     * \return Session reference
     */
    ST* FindBySessionId(const char* session_id, uint8_t* session_idx=NULL) {
        return FindBySessionIdWithExclude(session_id, NULL, session_idx);
    }


    /**
     * Service sessions.  Invoke at minimum once per Second - updates session last used timers and destory expired sessions
     * (session garbarge collection)
     *
     * \return Serviced state
     * \retval true Sessions serviced
     * \retval false Timer not finished, no session service yet
     */
    bool Service() {
        bool processed = false;

        if (!service_timeout_.IsRunning() || (service_timeout_.IsRunning() && service_timeout_.Since()>1000UL)) {
            processed = true;
            service_timeout_.Start();

            for(uint8_t i=0; i<S_MAX; i++) {
                session_[i].ExpireProcess(i);
            }
        }

        return processed;
    }


protected:

    /**
     * Get next free session or existing session by index
     *
     * \param[in] session_idx Session 0>= index <S_MAX or NULL (default)
     * \return Session reference
     */
    ST& Next(uint8_t* session_idx=NULL) {
        uint8_t selected = S_MAX;

        // Find any that arn't in use
        for(uint8_t i=0; i<S_MAX; i++) {
            if (!session_[i].IsInUse()) {
                selected = i;
                break;
            }
        }

        // Are all in use?
        if (selected == S_MAX) {
            // If so select session that hasn't been used for the longest period but default to first session
            selected = rand() % S_MAX;
            for(uint8_t i=0; i<S_MAX; i++) {
                if (i!=selected && session_[i].GetLastUse() > session_[selected].GetLastUse()) {
                    selected = i;
                }
            }
        }


        if (session_idx) {
            *session_idx = selected;
        }

        return session_[selected];
    }


    /**
     * Match existing session with given session id.  Rather than store say a CRC of session information I 
     * decided the best approach was to actually check by character string because of the nature of CRC 
     * probability.  Search can exclude a specific session
     *
     * \param[in] session_id Session id string pointer (NULL terminated)
     * \param[in] exclude Session object pointer or NULL (default, no session excluded)
     * \param[in] session_idx Session 0>= index <S_MAX or NULL (default, next free)
     * \return Session reference
     */
    ST* FindBySessionIdWithExclude(const char* session_id, ST* exclude=NULL, uint8_t* session_idx=NULL) {
        ST* f = NULL;

        if (session_id && (0 < strlen(session_id))) {
            if (!exclude) {
                for(uint8_t i=0; i<S_MAX; i++) {
                    if (session_[i].SessionId() == session_id) {
                        f = &session_[i];
                        if (session_idx) {
                            *session_idx = i;
                        }
                        break;
                    }
                }
            }else {
                for(uint8_t i=0; i<S_MAX; i++) {
                    if (&session_[i] != exclude && session_[i].SessionId() == session_id) {
                        f = &session_[i];
                        if (session_idx) {
                            *session_idx = i;
                        }
                        break;
                    }
                }
            }
        }

        return f;
    }


protected:
    StopWatch<SWT>  service_timeout_;               ///< Stopwatch service timeout
    ST              session_[S_MAX];                ///< Session object array
}; // class SessionManager

}; // namespace ghttp

#endif // GHTTP_SESSIONMGR_H
