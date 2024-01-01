/**
 * \file
 * Embedded HTTP request/response handling framework - MAC address structure
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_MAC_H
#define GHTTP_MAC_H

namespace ghttp {

/**
 * A structure represent a MAC address (12 Hex digit) with various accessors and helpers
 */
struct MAC_t {
    /**
     * Default constructor, make stable instance, 0x00 0x00 0x00 0x00 0x00 0x00 0x00 MAC
     */
    MAC_t() {
        Clear();
    }


    /**
     * Clear MAC to 0x00 0x00 0x00 0x00 0x00 0x00 0x00
     */
    void Clear() {
        u16[0] = 0U;
        u16[1] = 0U;
        u16[2] = 0U;
    }


    /**
     * Equality operator for comparisons against another MAC_t
     *
     * \param[in] rhs MAC reference for comparison
     * \retval true when rhs identical
     * \retval false when rhs differs
     */
    bool operator==(const MAC_t& rhs) const {
        return u16[0] == rhs.u16[0] && u16[1] == rhs.u16[1] && u16[2] == rhs.u16[2];
    }
    

    /**
     * Inequality operator for comparisons against another MAC_t
     *
     * \param[in] rhs MAC reference for comparison
     * \retval true when rhs differs
     * \retval false when rhs identical
     */
    bool operator!=(const MAC_t& rhs) const {
        return !(*this == rhs);
    }


    /**
     * Equal operator for assignment of MAC from ASCII string of the form "XXXXXXXXXXXX" where X is a hex (base 16) digit
     *
     * \param[in] s ASCII string pointer (NULL terminated)
     * \return This reference
     */
    MAC_t& operator=(const char* s) {
        uint8_t n, i, c, l=strlen(s);

        Clear();

        // We expect 12 hex digits
        if (12 == l) {
            for(i=0; i<l; i++) {
                c = s[i];
                if ((c>='0' && c<='9') || ((c=(c & 223))>='A' && c<='F')) {
                    if (c<='9') {
                        c-= '0';
                    }else {
                        c-= 'A' - 10;
                    }
                    // 0>= c <= 15 (a nibble)
                    if (!(i % 2)) {
                        n = c << 4;
                    }else {
                        n |= c;

                        // process
                        u8[i>>1] = n;
                    }
                }else {
                    break;    // Bad hex digit
                }
            }
        }

        return *this;
    }


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Equal operator for assignment of MAC from ASCII string of the form "XXXXXXXXXXXX" where X is a hex (base 16) digit
     *
     * \param[in] s ASCII string pointer (NULL terminated, flash based)
     * \return This reference
     */
    MAC_t& operator=(PGM_VOID_P s) {
        uint8_t n, i, c, l=PGM_STRLEN(s);

        Clear();

        // We expect 12 hex digits
        if (12 == l) {
            for(i=0; i<l; i++) {
                c = PGM_CHAR(s+i);

                if ((c>='0' && c<='9') || ((c=(c & 223))>='A' && c<='F')) {
                    if (c<='9') {
                        c-= '0';
                    }else {
                        c-= 'A' - 10;
                    }
                    // 0>= c <= 15 (a nibble)
                    if (!(i % 2)) {
                        n = c << 4;
                    }else {
                        n |= c;

                        // process
                        u8[i>>1] = n;
                    }
                }else {
                    break;    // Bad hex digit
                }
            }
        }

        return *this;
    }
#endif // defined(ARDUINO_ARCH_AVR)


    /**
     * Convert MAC address into string of the form "XXXXXXXXXXXX" where X is a hex (base 16) digit
     *
     * \param[out] mac_str String pointer.  Maximum length is 13 characters including NULL terminator
     * \return Length in characters (not including NULL)
     */
    uint8_t String(char *mac_str) {
        uint8_t l = 0;
        
        for(uint8_t u=0; u<6; u++) {
            if (u8[u]<0x10) {
                mac_str[l++] = '0';
            }
            l += StringHelper::FromUInt<uint8_t>(u8[u], &mac_str[l], 16);
        }

        return l;
    }


    /**
     * MAC storage, various integer types
     */
    union {
        uint8_t        u8[6];
        uint16_t    u16[3];
    };
}; // struct MAC_t

}; // namespace ghttp

#endif // GHTTP_MAC_H
