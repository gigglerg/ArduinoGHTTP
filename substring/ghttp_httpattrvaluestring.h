/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP attribute/value string
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPATTRVALUESTRING_H
#define GHTTP_HTTPATTRVALUESTRING_H

namespace ghttp {

/**
 * A class representing HTTP attribute/value pair substrings employed by both request and response processors.   Primarily used in header decoding by request where header 
 * strings contain attribute/value pair substrings
 */
class HTTPAttrValueString : public HTTPSubstring {
public:
    HTTPSubstring    value;                         ///< Value substring

public:
    /**
     * Default constructor, make stable instance.  String pointers are NULL
     */
    HTTPAttrValueString() : value() { }


    /**
     * Constructor, make stable instance.  Sets value substring to another substring instance
     */
    HTTPAttrValueString(HTTPSubstring &v) : value(v) { }


    /**
     * Constructor, make stable instance.  Sets attribute string from buffer, start index and length with value a NULL pointer
     */
    HTTPAttrValueString(char* buffer, const uint16_t start, const uint16_t length=0) : value() {
        Set(buffer, start, length);
    }


    /**
     * Address operator, return attribute substring
     *
     * \return Substring pointer
     */
    HTTPAttrValueString* operator&() const { return const_cast<HTTPAttrValueString*>(this); }


    /**
     * Equals comparison operator, check value string against given subject
     *
     * \param[in] rhs ASCII subject string
     * \retval true strings match
     * \retval false strings differ
     */
    bool operator==(const char* rhs) const {
        return value == rhs;
    }


    /**
     * Not equals comparison operator, check value string against given subject
     *
     * \param[in] rhs ASCII subject string
     * \retval true when session id's differ
     * \retval false when session id's match
     */
    bool operator!=(const char* rhs) const {
        return !(value == rhs);
    }

}; // class HTTPAttrValueString

}; // namespace ghttp

#endif // GHTTP_HTTPATTRVALUESTRING_H
