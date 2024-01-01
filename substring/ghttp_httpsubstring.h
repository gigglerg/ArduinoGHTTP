/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP substring
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPSUBSTRING_H
#define GHTTP_HTTPSUBSTRING_H

namespace ghttp {

/**
 * HTTP substring employed by both request and response processors.  Essentially a pointer which can be NULL or pointing to a NULL terminated string.  Includes various operators 
 * and helpers to assist
 */
class HTTPSubstring {
public:
    /**
     * Default constructor, make stable instance.  String pointer is NULL
     */
    HTTPSubstring() : str_ptr_(NULL) {
    }


    /**
     * Constructor, make stable instance.  Set string to given buffer with start character index and length (characters)
     *
     * \note A NULL terminator will be installed in buffer at start + length
     *
     * \param[in] buffer String buffer pointer
     * \param[in] start Start index of string into string buffer (characters)
     * \param[in] length Length in characters of string, default 0
     */
    HTTPSubstring(char* buffer, const uint16_t start, const uint16_t length=0) {
        Set(buffer, start, length);
    }


    /**
     * Set string to given buffer with start character index and length (characters)
     *
     * \note A NULL terminator will be installed in buffer at start + length
     *
     * \param[in] buffer String buffer pointer
     * \param[in] start Start index of string into string buffer (characters)
     * \param[in] length Length in characters of string, default 0
     */
    void Set(char* buffer, const uint16_t start, const uint16_t length=0) {
        str_ptr_ = &buffer[start];
        str_ptr_[length] = '\0';
    }


    /**
     * Set string pointer directly.  Can be used to NULL the string (make it non-existent)
     *
     * \param[in] str String pointer
     */
    void Set(char* str) {
        str_ptr_ = str;
    }


    /**
     * Get string pointer
     *
     * \return String pointer which could be NULL
     */
    const char* Get() const {
        return str_ptr_;
    }


    /**
     * Make string pointer NULL
     */
    void Null() {
        str_ptr_ = NULL;
    }


    /**
     * This pointer accessor.  Due to operator overload use this to get just instance this pointer
     *
     * \return HTTP string (this)
     */
    HTTPSubstring* ThisPtr() {
        return this;
    }


    /**
     * Address operator, return string pointer
     *
     * \return String pointer which could be NULL
     */
    const char* operator&() const {
        return Get();
    }


    /**
     * Equals comparison operator, check string against given subject
     *
     * \param[in] rhs ASCII subject string
     * \retval true strings match
     * \retval false strings differ
     */
    bool operator==(const char* rhs) const {
        // strlen(rhs)+1 to include '\0' NULL so partial match via == isn't possible
        return StrNCmp(rhs, false, strlen(rhs)+1) == 0;
    }


    /**
     * Not equals comparison operator, check string against given subject
     *
     * \param[in] rhs ASCII subject string
     * \retval true when session id's differ
     * \retval false when session id's match
     */
    bool operator!=(const char* rhs) const {
        return !(*this == rhs);
    }


    /**
     * Post increment operator to move internal string pointer forward
     *
     * \return Reference this
     */
    HTTPSubstring& operator++() {
        str_ptr_++;
        return *this;
    }


    /**
     * Post decrement operator to move internal string pointer backward
     *
     * \return Reference this
     */
    HTTPSubstring& operator--() {
        if (str_ptr_) {
            str_ptr_--;
        }
        return *this;
    }


    /**
     * Self increment operator to move internal string pointer forward by n
     *
     * \param[in] rhs Increment, +n
     * \return Reference this
     */
    HTTPSubstring& operator+=(const uint16_t rhs) {
        str_ptr_ += rhs;
        return *this;
    }


    /**
     * Self decrement operator to move internal string pointer backward by n
     *
     * \param[in] rhs Decrement, +n
     * \return Reference this
     */
    HTTPSubstring& operator-=(const uint16_t rhs) {
        str_ptr_ -= rhs;
        return *this;
    }


    /**
     * String compare with controllable properties like length and case sensitivity.  Uses \ref StringHelper::StrNCmp
     *
     * \param[in] s2 ASCII subject string
     * \param[in] case_sensitive Flag default false
     * \param[in] n Comparison length N characters, default 0 => strlen(s2)
     * \return String difference at index
     * \retval 0 s1[0..n-1] == s2[0..n-1]
     * \retval +1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     * \retval -1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     */
    int16_t StrNCmp(const char* s2, const bool case_sensitive=false, uint16_t n=0) const {
        return StringHelper::StrNCmp(str_ptr_, s2, n, case_sensitive);
    }


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Equals comparison operator, check string against given subject
     *
     * \param[in] rhs ASCII subject string (flash based)
     * \retval true strings match
     * \retval false strings differ
     */
    bool operator==(PGM_VOID_P rhs) const {
        // strlen(rhs)+1 to include '\0' NULL so partial match via == isn't possible
        return StrNCmp(rhs, false, PGM_STRLEN(rhs)+1) == 0;
    }


    /**
     * Not equals comparison operator, check string against given subject
     *
     * \param[in] rhs ASCII subject string (flash based)
     * \retval true when session id's differ
     * \retval false when session id's match
     */
    bool operator!=(PGM_VOID_P rhs) const {
        return !(*this == rhs);
    }


    /**
     * String compare with controllable properties like length and case sensitivity.  Uses \ref StringHelper::StrNCmp
     *
     * \param[in] s2 ASCII subject string (flash based)
     * \param[in] case_sensitive Flag default false
     * \param[in] n Comparison length N characters, default 0 => strlen(s2)
     * \return String difference at index
     * \retval 0 s1[0..n-1] == s2[0..n-1]
     * \retval +1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     * \retval -1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     */
    int16_t StrNCmp(PGM_VOID_P s2, const bool case_sensitive=false, uint16_t n=0) const {
        return StringHelper::StrNCmp(str_ptr_, s2, n, case_sensitive);
    }
#endif // defined(ARDUINO_ARCH_AVR)


    /**
     * Trim quotes, either single or double.  May replace NULL terminator and move internal string pointer forward
     *
     * \retval true Trimmed. Quotes removed
     * \retval false Didn't require
     */
    bool TrimQuotes() {
        bool d = false;

        if (str_ptr_) {
            uint16_t l = strlen(str_ptr_);

            if (l>1) {
                if ('\''==str_ptr_[l-1] || '\"'==str_ptr_[l-1]) {
                    str_ptr_[l-1] = '\0';
                    d = true;
                }
            }
            if (l) {
                if ('\''==str_ptr_[0] || '\"'==str_ptr_[0]) {
                    str_ptr_++;
                    d = true;
                }
            }
        }

        return d;
    }


    /**
     * Trim white space from both ends.  White space is soft space or tab characters.  May replace NULL terminator and move internal string pointer forward
     *
     * \retval true Trimmed
     * \retval false Didn't require
     */
    bool Trim() {
        return TrimLeft() | TrimRight();
    }


    /**
     * Trim white space from left hand side (start).  White space is soft space or tab characters.  May move internal string pointer forward
     *
     * \retval true Trimmed
     * \retval false Didn't require
     */
    bool TrimLeft() {
        bool d = false;

        if (str_ptr_) {
            uint16_t l = strlen(str_ptr_), ll;

            for(ll = 0; ll<l; ll++) {
                if (' '==str_ptr_[ll] || '\t'==str_ptr_[ll]) {
                    str_ptr_++;
                    d = true;
                }else {
                    break;
                }
            }
        }

        return d;
    }


    /**
     * Trim white space from right hand side (end).  White space is soft space or tab characters.  May replace NULL terminator 
     *
     * \retval true Trimmed
     * \retval false Didn't require
     */
    bool TrimRight() {
        bool d = false;

        if (str_ptr_) {
            uint16_t l = strlen(str_ptr_), ll;

            if (l) {
                for(ll = l-1; ll>0; ll--) {
                    if (' '==str_ptr_[ll-1] || '\t'==str_ptr_[ll-1]) {
                        str_ptr_[ll-1] = '\0';
                        d = true;
                    }else {
                        break;
                    }
                }
            }
        }

        return d;
    } // TrimRight()


protected:
    char*        str_ptr_;                          ///< Internal string pointer
}; // class HTTPSubstring


}; // namespace ghttp

#endif // GHTTP_HTTPSUBSTRING_H
