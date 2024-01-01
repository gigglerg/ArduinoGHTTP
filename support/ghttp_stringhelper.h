/**
 * \file
 * Embedded HTTP request/response handling framework - String helper
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_STRINGHELPER_H
#define GHTTP_STRINGHELPER_H

#if defined(ARDUINO_ARCH_AVR)
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif


namespace ghttp {

#if defined(ARDUINO_ARCH_AVR)
/**
 * POD type for flash based pointers.  Atmel AVR cores are based around Harvard architecture so buses for data and instruction(flash) are separate and as such both potentially 
 * start at 0 with no obvious indication in address numeric that some resource is in internal SRAM or flash.   What we can do is decide how to access the data by type so here 
 * we create a type specifically for flash based storage and retrieval
 */
typedef const void* PFLASH_t;
#endif


/**
 * Support class for string manipulation containing static helpers for reuse
 */
class StringHelper {
public:
    /**
     * Alternative from strncmpi and strncmp.  Some standard library implementations don't include
     *
     * \note There appear to be a few implementations so this is the one duplicated here:
     * https://www.techonthenet.com/c_language/standard_library_functions/string_h/strncmp.php
     *
     * \param[in] s1 String, NULL terminated
     * \param[in] s2 String, NULL terminated
     * \param[in] s2_max Characters in s2 string (length excluding NULL).  Use it to reduce search length within s1 string
     * \param[in] case_sensitive Case sensitive compare flag (default true)
     * \return String difference at index
     * \retval 0 s1[0..n-1] == s2[0..n-1]
     * \retval +1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     * \retval -1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     */
    static int16_t StrNCmp(const char* s1, const char* s2, const uint16_t s2_max=0, const bool case_sensitive=true) {
        uint16_t n = s2_max, i = 0;
        int16_t r = 0;

        if (s1 && s2) {
            if (n == 0) {
                n = strlen(s2);
            }
            for(; i<n; i++) {
                if ((case_sensitive && s1[i] != s2[i]) || (!case_sensitive && (s1[i] & 223) != (s2[i] & 223))) {
/*                    if (s1[i] < s2[i]) {
                        r = -1;
                    }else {
                        r = 1;
                    }*/
                    r = s1[i] - s2[i];    // +n if s1>s2 else -n
                    break;
                }
            }
        }else {
            r = 1;    // Doesn't matter its different
        }

/*        if (0 == r) {
            // Is s1 complete or is there more (i.e. strlen(s1) != strlen(s2))
            if ('\0' != s1[i] && '\0' == s2[i]) {
                r = 1;
            }
        }
*/
        return r;
    } // StrNCmp(...)


    /**
     * Search a string for a substring
     *
     * \note You can find the next substring within s1 by moving forward size of s2 string and passing this pointer in as s1
     *
     * \param[in] s1 Pointer to string1; the source or search string
     * \param[in] s2 Pointer to string2; the subject or substring to search for
     * \param[in] s1_start_i Start index for find into string s1
     * \param[in] case_sensitive Flag to control case sensitive searches (default true)
     * \param[in] s1_max s1 string maximum length (characters).  Default 0 which means strlen(s1) is used
     * \param[in] s2_max s2 string maximum length (characters).  Default 0 which means strlen(s2) is used
     * \return Starting index within s1 string where s2 substring found.  If larger-equal than strlen(s1) => not found
     */
    static uint16_t FindSubString(const char* s1, const char* s2, const uint16_t s1_start_i = 0, const bool case_sensitive = true, \
                                    const uint16_t s1_max = 0, const uint16_t s2_max=0) {
        uint16_t o, e;
        uint16_t ei = s1_max;
        uint16_t i = s1_start_i;

        if (s1 && s2 && !s1_max) {
            ei = strlen(s1);
            ei--; // Last index not length required
        }

        do {
            if (i >= ei) {
                i = 0xffffU; // Invalid
                break;
            }

            o = 0;
            e = i;
            while(1) {
                if (s2_max && o == s2_max) {
                    break;
                }else if ('\0' == s2[o]) {
                    break;
                }else if (e > ei) {
                    o = 0;
                    break;
                }else if (case_sensitive && s1[e] != s2[o]) {
                    o = 0;
                    break;
                }else if (!case_sensitive && (s1[e] & 223) != (s2[o] & 223)) {
                    o = 0;
                    break;
                }
                o++;
                e++;
            }
            if (o) {
                break; // Found
            }
            i++;
        }while(1);

        return i;
    } // FindSubstring(...)


    /**
     * Find terminator character using characters from given terminator string.  Can search only for characters that are included in terminator string
     *
     * \note Set parameter invert_speical_only to search for characters that only exist in this string
     *
     * \param[in] i Starting index into src string
     * \param[in] src Source string
     * \param[in] include_special Terminator string.  Contains characters you either want to find or must include
     * \param[in] invert_special_only Flag controlling termination where search ends when character exists in 
     * terminator string(false) or must exist in terminator string(true).
     * \param[in] src_max Source string maximum length or search length (characters).  Default 0 which means strlen(src)
     * \return Index where terminator character found(invert_special_only=false) or not found (invert_special_only=true).
     * If larger-equal than strlen(src) => not found
     */
    static uint16_t FindTerminator(uint16_t i, const char* src, const char* include_special, bool invert_special_only=false, \
                                    uint16_t src_max=0) {
        uint16_t u;

        if (!src_max) {
            src_max = strlen(src);
        }

        if (!include_special) {
            i = src_max;
        }

        while(1) {
            if (i>=src_max || (!invert_special_only && !src[i])) {
                break;
            }else {
                for(u=0; include_special; u++) {
                    if (include_special[u] == '\0') {
                        if (invert_special_only) {
                            // If invert_special_only means we must have characters only in special, if we are here with flag set then we didn't so exit
                            goto FindTerminator_Exit;
                        }
                        break;
                    }else if (!invert_special_only && (include_special[u] == src[i])) {
                        goto FindTerminator_Exit;

                    }else if (invert_special_only && (include_special[u] == src[i])) {
                        break; // We continue because character found is in special
                    }
                }
                i++;
            }
        }
        FindTerminator_Exit:

        return i;
    } // FindTerminator(...)


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Alternative from strncmpi and strncmp.  Some standard library implementations don't include
     *
     * \note There appear to be a few implementations so this is the one duplicated here:
     * https://www.techonthenet.com/c_language/standard_library_functions/string_h/strncmp.php
     *
     * \param[in] s1 String, NULL terminated
     * \param[in] s2 String, NULL terminated (flashed based)
     * \param[in] s2_max Characters in s2 string (length excluding NULL).  Use it to reduce search length within s1 string
     * \param[in] case_sensitive Case sensitive compare flag (default true)
     * \return String difference at index
     * \retval 0 s1[0..n-1] == s2[0..n-1]
     * \retval +1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     * \retval -1 => s1[n] - s2[n] where n is index of s1[n] != s2[n]
     */
    static int16_t StrNCmp(const char* s1, PGM_VOID_P s2, const uint16_t s2_max=0, const bool case_sensitive=true) {
        char s2c;
        uint16_t n = s2_max, i = 0;
        int16_t r = 0;
        char *ss2 = static_cast<char*>(const_cast<void*>(s2));

        if (s1 && s2) {
            if (n == 0) {
                if (!s2_max) {
                    n = PGM_STRLEN(s2);
                }else {
                    n = PGM_STRNLEN(s2, s2_max);
                }
            }
            for(; i<n; i++) {
                s2c = PGM_CHAR(ss2+i);

                if ((case_sensitive && s1[i] != s2c) || (!case_sensitive && (s1[i] & 223) != (s2c & 223))) {
/*                    if (s1[i] < s2c) {
                        r = -1;
                    }else {
                        r = 1;
                    }*/
                    r = s1[i] - s2c;    // +n if s1>s2 else -n
                    break;
                }
            }
        }else {
            r = 1;    // Doesn't matter its different
        }

/*
        if (0 == r) {
            s2c = PGM_CHAR(ss2+i);

            // Is s1 complete or is there more (i.e. strlen(s1) != strlen(s2))
            if ('\0' != s1[i] && '\0' == s2c) {
                r = 1;
            }
        }*/

        return r;
    } // StrNCmp(...)


    /**
     * Search a string for a substring
     *
     * \note You can find the next substring within s1 by moving forward size of s2 string and passing this pointer in as s1
     *
     * \param[in] s1 Pointer to string1; the source or search string
     * \param[in] s2 Pointer to string2 (flash based); the subject or substring to search for
     * \param[in] s1_start_i Start index for find into string s1
     * \param[in] case_sensitive Flag to control case sensitive searches (default true)
     * \param[in] s1_max s1 string maximum length (characters).  Default 0 which means strlen(s1) is used
     * \param[in] s2_max s2 string maximum length (characters).  Default 0 which means strlen(s2) is used
     * \return Starting index within s1 string where s2 substring found.  If larger-equal than strlen(s1) => not found
     */
    static uint16_t FindSubString(const char* s1, PGM_VOID_P s2, const uint16_t s1_start_i = 0, const bool case_sensitive = true, \
                                    const uint16_t s1_max = 0, const uint16_t s2_max=0) {
        uint16_t o, e;
        uint16_t ei = s1_max;
        uint16_t i = s1_start_i;
        char s2c;
        char *ss2 = static_cast<char*>(const_cast<void*>(s2));

        if (s1 && s2 && !s1_max) {
            ei = strlen(s1);
            ei--; // Last index not length required
        }

        do {
            if (i >= ei) {
                i = 0xffffU; // Invalid
                break;
            }

            o = 0;
            e = i;
            while(1) {
                s2c = PGM_CHAR(ss2+o);

                if (s2_max && o == s2_max) {
                    break;
                }else if ('\0' == s2c) {
                    break;
                }else if (e > ei) {
                    o = 0;
                    break;
                }else if (case_sensitive && s1[e] != s2c) {
                    o = 0;
                    break;
                }else if (!case_sensitive && (s1[e] & 223) != (s2c & 223)) {
                    o = 0;
                    break;
                }
                o++;
                e++;
            }
            if (o) {
                break; // Found
            }
            i++;
        }while(1);

        return i;
    } // FindSubstring(...)


    /**
     * Find terminator character using characters from given terminator string.  Can search only for characters that are included in terminator string
     *
     * \note Set parameter invert_speical_only to search for characters that only exist in this string
     *
     * \param[in] i Starting index into src string
     * \param[in] src Source string
     * \param[in] include_special Terminator string (flash based).  Contains characters you either want to find or must include
     * \param[in] invert_special_only Flag controlling termination where search ends when character exists in 
     * terminator string(false) or must exist in terminator string(true).
     * \param[in] src_max Source string maximum length or search length (characters).  Default 0 which means strlen(src)
     * \return Index where terminator character found(invert_special_only=false) or not found (invert_special_only=true).
     * If larger-equal than strlen(src) => not found
     */
    static uint16_t FindTerminator(uint16_t i, const char* src, PGM_VOID_P include_special, bool invert_special_only=false, \
                                    uint16_t src_max=0) {
        uint16_t u;
        char isc;
        char *include_special2 = static_cast<char*>(const_cast<void*>(include_special));

        if (!src_max) {
            src_max = strlen(src);
        }

        if (!include_special) {
            i = src_max;
        }

        while(1) {
            if (i>=src_max || (!invert_special_only && !src[i])) {
                break;
            }else {
                for(u=0; include_special2; u++) {
                    isc = PGM_CHAR(include_special2+u);

                    if (isc == '\0') {
                        if (invert_special_only) {
                            // If invert_special_only means we must have characters only in special, if we are here with flag set then we didn't so exit
                            goto FindTerminator_Exit;
                        }
                        break;
                    }else if (!invert_special_only && (isc == src[i])) {
                        goto FindTerminator_Exit;

                    }else if (invert_special_only && (isc == src[i])) {
                        break; // We continue because character found is in special
                    }
                }
                i++;
            }
        }
        FindTerminator_Exit:

        return i;
    } // FindTerminator(...)


    /**
     * Find terminator character using characters from given terminator string.  Can search only for characters that are included in terminator string
     *
     * \note Set parameter invert_speical_only to search for characters that only exist in this string
     *
     * \param[in] i Starting index into src string
     * \param[in] src Source string (flash based)
     * \param[in] include_special Terminator string (flash based).  Contains characters you either want to find or must include
     * \param[in] invert_special_only Flag controlling termination where search ends when character exists in 
     * terminator string(false) or must exist in terminator string(true).
     * \param[in] src_max Source string maximum length or search length (characters).  Default 0 which means strlen(src)
     * \return Index where terminator character found(invert_special_only=false) or not found (invert_special_only=true).
     * If larger-equal than strlen(src) => not found
     */
    static uint16_t FindTerminator(uint16_t i, PGM_VOID_P src, PGM_VOID_P include_special, bool invert_special_only=false, \
                                    uint16_t src_max=0) {
        uint16_t u;
        char isc, isr;
        char *include_special2 = static_cast<char*>(const_cast<void*>(include_special));
        char *src2 = static_cast<char*>(const_cast<void*>(src));

        if (!src_max) {
            src_max = PGM_STRLEN(src);
        }

        if (!include_special) {
            i = src_max;
        }

        while(1) {
            isr = PGM_CHAR(src2+i);
            if (i>=src_max || (!invert_special_only && !isr)) {
                break;
            }else {
                for(u=0; include_special2; u++) {
                    isc = PGM_CHAR(include_special2+u);

                    if (isc == '\0') {
                        if (invert_special_only) {
                            // If invert_special_only means we must have characters only in special, if we are here with flag set then we didn't so exit
                            goto FindTerminator_Exit;
                        }
                        break;
                    }else if (!invert_special_only && (isc == isr)) {
                        goto FindTerminator_Exit;

                    }else if (invert_special_only && (isc == isr)) {
                        break; // We continue because character found is in special
                    }
                }
                i++;
            }
        }
        FindTerminator_Exit:

        return i;
    } // FindTerminator(...)
#endif // defined(ARDUINO_ARCH_AVR)


    /**
     * Reverse string
     *
     * \param[in] str Pointer to string to reverse
     * \param[in,out] length Length in characters of reverse operation (default 0 => strlen(str))
     * \return str Pointer
     */
    static char *Reverse(char* str, uint8_t length=0) {
        uint8_t hl;
        
        if (!length) {
            length = strlen(str);
        }
        
        hl = length>>1;

        if (length<=1) {
            return str;
        }
        length--;    // Last character always -1
        for(uint8_t i=0; i<hl; i++) {
            // EOR swap character[i] with character[length-i]
            str[i] = str[i] ^ str[length-i];
            str[length-i] = str[length-i] ^ str[i];
            str[i] = str[i] ^ str[length-i];
        }

        return str;
    }


    /**
     * Hex digit from index
     *
     * \param[in] i >=0 Index <=15
     * \return Character from alphabet
     */
    static char HexDigitFromIndex(const uint8_t i) {
#if defined(ARDUINO_ARCH_AVR)
        PGM_VOID_P ascii_ = T("0123456789abcdef");

        return PGM_CHAR(ascii_+(i & 15));
#else
        const char ascii_[] = "0123456789abcdef";

        return ascii_[i & 15];
#endif
    }


    /**
     * String from signed integer type T.  itoa like function
     * Source: http://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux
     * Internally makes -n +n which means you must have a data type twice as large as desired conversion
     *
     * \attention Only tested base 10 and 16!
     *
     * \tparam T Base signed integer used for conversion.  Usable range is TOP/2 for any given type
     * \param[in] number Number to convert
     * \param[out] str String pointer to buffer for conversion
     * \param[in] base Number base/radix, default 10 (denary)
     * \return Length in characters (not including NULL)
     */
    template<typename T>
    static uint8_t FromInt(T number, char* str, const uint8_t base=10) {
        uint8_t i, sign = 0;

        // Record sign
        if (number<0) {
            number = -number;        // abs(n)
            sign = 1;
        }

        // Generate digits in reverse order
        i = 0;
        do {
            str[i++] = StringHelper::HexDigitFromIndex((number % base) & 15);   // Get next digit
        }while ((number /= base) > 0);    // Delete it
        if (sign) {
            str[i++] = '-';
        }
        str[i] = '\0';    // null terminator

        StringHelper::Reverse(str, i);    // Reverse string to complete

        return i;
    }


    /**
     * String from unsigned integer type T.  itoa like function
     * Source: http://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux
     * Internally makes -n +n which means you must have a data type twice as large as desired conversion
     *
     * \attention Only tested base 10 and 16!
     *
     * \tparam T Base unsigned integer used for conversion
     * \param[in] number Number to convert
     * \param[out] str String pointer to buffer for conversion
     * \param[in] base Number base/radix, default 10 (denary)
     * \return Length in characters (not including NULL)
     */
    template<typename T>
    static uint8_t FromUInt(T number, char* str, const uint8_t base=10) {
        uint8_t i;

        // Generate digits in reverse order
        i = 0;
        do {
            str[i++] = StringHelper::HexDigitFromIndex((number % base) & 15);   // Get next digit
        }while ((number /= base) > 0);    // Delete it
        str[i] = '\0';    // null terminator

        StringHelper::Reverse(str, i);    // Reverse string to complete

        return i;
    }


    /**
     * String from real type T.  Like sprintf(str, "%f")
     * Source: printFloat for arduno in core/maple/print.cpp
     * Unfortunately for maple mini its a private method and within a class that would be bloated for my purposes here, hence the 
     * copy and changes.
     *
     * \tparam T Base real type for conversion (float or double)
     * \param[in] number Number to convert
     * \param[out] str String pointer to buffer for conversion
     * \param[in] digits Digits below the decimal point, default 2
     * \return Length in characters (not including NULL)
     */
    template<typename T>
    static uint8_t FromDecimal(T number, char* str, uint8_t digits=2) {
        uint8_t length=0;

        // Ignore dealing with super large values - we know they wont be in our working system

        // Handle negative numbers
        if (number < 0.0) {
            str[length++]='-';
            number = -number;    // abs(...)
        }
        str[length]='\0';

        // Simplistic rounding strategy so that e.g. print(1.999, 2) - prints as "2.00"
        T rounding = 0.5;
        for (uint8_t i = 0; i < digits; i++) {
            rounding /= 10.0;
        }
        number += rounding;

        // Extract the integer part of the number and print it
        uint32_t int_part = (uint32_t)number;
        T remainder = number - int_part;
        StringHelper::FromUInt(int_part, &str[length], 10);
        length=strlen(str);

        // Print the decimal point, but only if there are digits beyond
        if (digits > 0) {
            str[length++]='.';
            str[length]='\0';
        }

        // Extract digits from the remainder one at a time
        while(digits-- > 0) {
            remainder *= 10.0;
            uint32_t to_print = (uint32_t)remainder;

            StringHelper::FromUInt(to_print, &str[length], 10);
            length=strlen(str);

            remainder -= to_print;
        }

        return length;
    } // FromDecimal(...)
}; // class StringHelper

}; // namespace ghttp

#endif // GHTTP_STRINGHELPER_H
