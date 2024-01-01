/**
 * \file
 * Embedded HTTP request/response handling framework - Attr/value generic string set
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPATTRVALUESTRINGSETGENERIC_H
#define GHTTP_HTTPATTRVALUESTRINGSETGENERIC_H

namespace ghttp {

/**
 * A container class for an array of N class T where user specifies the storage class T
 *
 * \todo Support iterator
 *
 * \tparam N Array size
 * \tparam T Class of array container
 */
template<uint8_t N, class T>
class HTTPAttrValueStringSetGeneric {
public:
    uint8_t     count;                              ///< Array count

public:
    /**
     * Default constructor, make stable instance.  Strings are NULL and count is 0
     */
    HTTPAttrValueStringSetGeneric() : count(0) { }


    /**
     * Get at index use to access array elements N of instance T
     *
     * param[in] index Array 0>= index < N
     * return T pointer or NULL
     */
    T* At(const uint8_t index) { // for const objects: can only be used for access
        if (index>count) {
            return NULL;
        }

        return static_cast<T*>(&str_[index]);
    }


    /**
     * Index operator by string index (key), use to access array elements N of instance T
     *
     * param[in] index Array 0>= index < N
     * return T pointer or NULL
     */
    T* operator[] (const uint8_t i) { // for const objects: can only be used for access
        return At(i);
    }


    /**
     * Index operator by character string (key), use to access array elements N of instance T
     *
     * param[in] str String pointer to index string name to match
     * return T pointer or NULL
     */
    T* operator[] (const char* str) { // for const objects: can only be used for access
        int16_t index = Find(str, false, true);

        if (index<0) {
            return NULL;
        }

        return static_cast<T*>(&str_[index]);
    }


    /**
     * Search array for elements.  RFC 7230 (HTTP/1.1):  Each header field consists of a case-insensitive field name followed by a colon (":"), 
     * optional leading whitespace, the field value, and optional trailing whitespace.  So the default compare is case-insensitive as is 
     * then == and != operators
     *
     * param[in] str String pointer, subject
     * param[in] by_value Flag (default false => attribute)
     * param[in] case_sensitive Flag (default false => not case sensitive)
     * param[in] n String compare length (default 0 => strlen(str))
     * return Array index
     * retval -1 Not found
     * retval +n Array index.  Use index operator to access element
     */
    int16_t Find(const char* str, const bool by_value=false, const bool case_sensitive=false, const uint16_t n=0) {
        int16_t idx = -1;

        for(uint32_t i=0; i<count; i++) {
            if (by_value) {
                if (!str_[i].value.StrNCmp(str, case_sensitive, n)) {
                    idx = i;
                    break;
                }
            }else {
                if (!str_[i].StrNCmp(str, case_sensitive, n)) {
                    idx = i;
                    break;
                }
            }
        }

        return idx;
    }


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Index operator by character string (key), use to access array elements N of instance T
     *
     * param[in] str String pointer to index string name to match (flash based)
     * return T pointer or NULL
     */
    T* operator[] (PGM_VOID_P str) { // for const objects: can only be used for access
        int16_t index = Find(str, false, true);

        if (index<0) {
            return NULL;
        }

        return static_cast<T*>(&str_[index]);
    }


    /**
     * Search array for elements.  RFC 7230 (HTTP/1.1):  Each header field consists of a case-insensitive field name followed by a colon (":"), 
     * optional leading whitespace, the field value, and optional trailing whitespace.  So the default compare is case-insensitive as is 
     * then == and != operators
     *
     * param[in] str String pointer, subject (flash based)
     * param[in] by_value Flag (default false => attribute)
     * param[in] case_sensitive Flag (default false => not case sensitive)
     * param[in] n String compare length (default 0 => strlen(str))
     * return Array index
     * retval -1 Not found
     * retval +n Array index.  Use index operator to access element
     */
    int16_t Find(PGM_VOID_P str, const bool by_value=false, const bool case_sensitive=false, const uint16_t n=0) {
        int16_t idx = -1;

        for(uint32_t i=0; i<count; i++) {
            if (by_value) {
                if (!str_[i].value.StrNCmp(str, case_sensitive, n)) {
                    idx = i;
                    break;
                }
            }else {
                if (!str_[i].StrNCmp(str, case_sensitive, n)) {
                    idx = i;
                    break;
                }
            }
        }

        return idx;
    }
#endif // defined(ARDUINO_ARCH_AVR)

protected:
    T        str_[N];                               ///< String of T
}; // class HTTPAttrValueStringSetGeneric

}; // namespace ghttp

#endif // GHTTP_HTTPATTRVALUESTRINGSETGENERIC_H
