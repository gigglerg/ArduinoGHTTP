/**
 * \file
 * Embedded HTTP request/response handling framework - IP4 structure
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_IP4_H
#define GHTTP_IP4_H

namespace ghttp {

/**
 * A structure represent an IP4 address with various accessors and helpers
 */
struct IP4_t {
    /**
     * Default constructor, make stable instance, 0.0.0.0 IP
     */
    IP4_t() : u32(0) { }


    /**
     * Clear IP to 0.0.0.0
     */
    void Clear() {
        u32 = 0;
    }


    /**
     * Equality operator for comparisons against another IP4_t
     *
     * \param[in] rhs IP4 reference for comparison
     * \retval true when rhs identical
     * \retval false when rhs differs
     */
    bool operator==(const IP4_t& rhs) const {
        return u32 == rhs.u32;
    }
    

    /**
     * Inequality operator for comparisons against another IP4_t
     *
     * \param[in] rhs IP4 reference for comparison
     * \retval true when rhs differs
     * \retval false when rhs identical
     */
    bool operator!=(const IP4_t& rhs) const {
        return !(*this == rhs);
    }


    /**
     * Equality operator for comparisons against another IP4_t
     *
     * \param[in] rhs IP4 pointer for comparison
     * \retval true when rhs identical
     * \retval false when rhs differs
     */
    bool operator==(const IP4_t* rhs) const {
        return u32 == rhs->u32;
    }
    

    /**
     * Inequality operator for comparisons against another IP4_t
     *
     * \param[in] rhs IP4 pointer for comparison
     * \retval true when rhs differs
     * \retval false when rhs identical
     */
    bool operator!=(const IP4_t* rhs) const {
        return !(*this == rhs);
    }


    /**
     * Equal operator for assignment of IP from ASCII string of the form "X.X.X.X" where 0>= X <=255
     *
     * \param[in] s ASCII string pointer (NULL terminated)
     * \return This reference
     */
    IP4_t& operator=(const char* s) {
        uint8_t n=1, i=0, l=strlen(s);

        Clear();
        // Attempt to string parse "s" in the form xxx.xxx.xxx.xxx (xxx base 10 numeric)
        u8[0] = static_cast<uint8_t>(atoi(s));
        for(; i<l; i++) {
            if (s[i]=='.') {
                u8[n++] = static_cast<uint8_t>(atoi(s+1+i));
                if (n==4) {
                    break;
                }
            }
        }

        return *this;
    }


    /**
     * Convert IP4 address into string as "X.X.X.X" (X is 1..3 digit decimal)
     *
     * \param[out] ip_str String pointer.  Maximum length is 16 characters including NULL terminator
     * \return Length in characters (not including NULL)
     */
    uint8_t String(char *ip_str) {
        uint8_t l = StringHelper::FromUInt<uint8_t>(u8[0], ip_str);
        ip_str[l++] = '.';
        l += StringHelper::FromUInt<uint8_t>(u8[1], &ip_str[l]);
        ip_str[l++] = '.';
        l += StringHelper::FromUInt<uint8_t>(u8[2], &ip_str[l]);
        ip_str[l++] = '.';
        l += StringHelper::FromUInt<uint8_t>(u8[3], &ip_str[l]);

        return l;
    }
    

    /**
     * IP4 address storage, various integer types
     */
    union {
        uint8_t     u8[4];
        uint16_t    u16[2];
        uint32_t    u32;
    };
}; // struct IP4_t

}; // namespace ghttp

#endif // GHTTP_IP4_H
