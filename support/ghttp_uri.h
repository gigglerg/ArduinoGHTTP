/**
 * \file
 * Embedded HTTP request/response handling framework - URI
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_URI_H
#define GHTTP_URI_H

namespace ghttp {

/**
 * A class representing URI encode/decode implementation loosely covering RFC 3986
 */
class URI {
public:
    /**
     * Decode URI string.  Decoding may make a string smaller but not larger
     *
     * \param[in,out] str Encoded ASCII string pointer (NULL terminated)
     * \param[in] l String str length in characters (not including NULL).  Default 0 => strlen(str)
     * \param[in] include_plus_space Flag indicating ASCII plus '+' translates to ' ' (soft space) when true (default)
     * \return String pointer of str
     */
    static char* Decode(char* str, uint16_t l=0, const bool include_plus_space=true) {
        uint8_t h;
        uint16_t i = 0;
        char* o_str = str;
        char* w_str = str;
    
        if (0 == l) {
            l = strlen(str);
        }
        if (0 == l) {
            return str;
        }
    
        while(1) {
            if (!*str) {
                break;
            }
            if (include_plus_space && '+'==*str) {
                *w_str = ' ';
            }else {
                if ('%' == str[0] && str[1]>='0' && ((str[1] & 0x223)<='F') && str[2]>='0' && ((str[2] & 0x223)<='F')) {
                    // Upper nibble 0-9 or a-f
                    if (str[1]<='9') {
                        h = (str[1] - '0')<<4;
                    }else {
                        h = (((str[1] & 223) - 'A') + 10) <<4;
                    }

                    // Lower nibble 0-9 or a-f
                    if (str[2]<='9') {
                        h |= str[2] - '0';
                    }else {
                        h |= ((str[2] & 223) - 'A') + 10;
                    }
                
                    *w_str = (char)h;
                    str+=2;    // 2+1=3 => %xx=3
                }else {
                    if (w_str != str) {
                        *w_str = *str;
                    }
                }
            }
            str++;
            w_str++;
        }
        // Altered?  install null
        if (w_str != str) {
            *w_str = '\0';
        }
    
        return o_str;
    } // Decode(...)


    /**
     * Get decode URI string length (characters) includes NULL
     * 
     * \param[in] str Encoded ASCII string pointer (NULL terminated)
     * \param[in] l String str length in characters (not including NULL).  Default 0 => strlen(str)
     * \return Length in Bytes/ASCII characters
     */
    static uint16_t GetDecodeLength(char* str, uint16_t l=0) {
        uint16_t dl = 1;
    
        if (0 == l) {
            l = strlen(str);
        }
        if (0 < l) {
            while(1) {
                if (!*str) {
                    break;
                }
                if ('%' == str[0] && str[1]>='0' && ((str[1] & 0x223)<='F') && str[2]>='0' && ((str[2] & 0x223)<='F')) {
                    str+=2;    // 2+1=3 => %xx=3
                }
                dl++;
                str++;
            }
        }

        return dl;
    } // GetDecodeLength(...)


    /**
     * Encode URI string.  Encoding may make a string larger
     *
     * \param[out] dst ASCII string pointer
     * \param[in] dst_l Destination string buffer length (characters)
     * \param[in] src Encoded string pointer (NULL terminated)
     * \param[in] src_l Encoded string length in Bytes (not including NULL).  Default 0 => strlen(src)
     * \param[in] plus_space_no_twiddle Flag indicating '+' => ' ' and no '~' accepted (default true)
     * \return String pointer dst
     */
    static char* Encode(char* dst, uint16_t dst_l, const char* src, uint16_t src_l=0, const bool plus_space_no_twiddle=true) {
        char a = '\0',b = '\0';
        char* dst_end_ptr = &dst[dst_l];
        char* dst_o = dst;
        if (!src_l) {
            src_l = strlen(src);
        }
        for(uint16_t i=0; i<src_l; i++) {
            // Single characters?
            if (plus_space_no_twiddle && src[i]==' ') {
                // +
            }else if (!plus_space_no_twiddle && src[i]=='~') {
                // ~
            }else if (src[i]=='-' || src[i]=='.' || src[i]=='_') {
                // '-', '.', '_'
            }else if (src[i]>='0' && src[i]<='9') {
                // '0' ... '9'
            }else if ((src[i] & 223)>='A' && (src[i] & 223)<='Z') {
                // 'A' ... 'Z', 'a' ... 'z'
            }else {
                // Anything else is %xx encode
                a = (src[i]>>4);
                a+=(a>=10)?'A'-10:'0';
                b = src[i] & 0xf;
                b+=(b>=10)?'A'-10:'0';
            }
            if (a | b) {
                if ((dst+3)>=dst_end_ptr) {
                    break;
                }
                dst[0] = '%';
                dst[1] = a;
                dst[2] = b;
                dst+=3;
                a = b = '\0';
            }else {
                if ((dst+1)>=dst_end_ptr) {
                    break;
                }
                dst[0] = src[i];
                dst++;
            }
        } // switch(src[i])
        if ((dst+1)>=dst_end_ptr) {
            *(dst_end_ptr-1) = '\0';
        }else {
            dst[0]='\0';
        }

        return dst_o;
    } // Encode(...)


    /**
     * Get encode URI string length (Bytes/characters) includes NULL
     * 
     * \param[in] src ASCII string pointer (NULL terminated)
     * \param[in] src_l String src length (not including NULL).  Default 0 => strlen(src)
     * \param[in] plus_space_no_twiddle Flag indicating '+' => ' ' and no '~' accepted (default true)
     * \return Length in Bytes/characters
     */
    static uint16_t GetEncodeLength(const char* src, uint16_t src_l=0, const bool plus_space_no_twiddle=true) {
        uint16_t d_len = 0;
        if (!src_l) {
            src_l = strlen(src);
        }
        for(uint16_t i=0; i<src_l; i++) {
            // Single characters?
            if (plus_space_no_twiddle && src[i]==' ') {
                // +
            }else if (!plus_space_no_twiddle && src[i]=='~') {
                // ~
            }else if (src[i]=='-' || src[i]=='.' || src[i]=='_') {
                // '-', '.', '_'
            }else if (src[i]>='0' && src[i]<='9') {
                // '0' ... '9'
            }else if ((src[i] & 223)>='A' && (src[i] & 223)<='Z') {
                // 'A' ... 'Z', 'a' ... 'z'
            }else {
                // Anything else is %xx encode
                d_len+=2;
            }
            d_len++;
        } // switch(src[i])

        return d_len+1;    // Include NULL
    } // GetEncodeLength(...)

}; // class URI

}; // namespace ghttp

#endif // GHTTP_URI_H
