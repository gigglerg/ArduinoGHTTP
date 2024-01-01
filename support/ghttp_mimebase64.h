/**
 * \file
 * Embedded HTTP request/response handling framework - Mime, Base64
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_MIMEBASE64_H
#define GHTTP_MIMEBASE64_H

namespace ghttp {

/**
 * A class representing a Mime Base64 implementation loosely covering RFC 2045.  Refer: https://en.wikipedia.org/wiki/Base64
 */
class MIMEBase64 {
protected:
    /**
     * ADT used to seperate and build Base64 characters against CPU sized opaque (POD) type
     */
    struct bitstream_t {
        union {
            uint32_t u32;
            uint8_t u8[4];
            struct {
                uint32_t d:6;
                uint32_t c:6;
                uint32_t b:6;
                uint32_t a:6;
                uint32_t reserved:8;
            }f;
        };
    };

protected:
    /**
     * Translate an ASCII character to Base64 alphabet used for decode operations
     *
     * \param[in] c ASCII character
     * \return Base64 character code/index
     */
    static uint8_t IndexFromASCII(const char c) {
        uint8_t a = 0;     // assume padding

        if (c>='A' && c<='Z') {
            // index 0..25
            a = static_cast<uint8_t>(c-'A');
        }else if (c>='a' && c<='z') {
            // index 26..51
            a = static_cast<uint8_t>(c- ('a' - 26));
        }else if (c>='0' && c<='9') {
            // index 52..61
            a = static_cast<uint8_t>(c + 4);
        }else if (c=='+') {
            a = 62;
        }else if (c=='/') {
            a = 63;
        }
    
        return a;
    }


    /**
     * Translate a Base64 alphabet index into an ASCII character used for encode operations
     *
     * \param[in] a Base64 character code/index
     * \return ASCII character
     */
    static char ASCIIFromIndex(const uint8_t a) {
        char c = '=';    // assume padding

        if (a>=0 && a<=25) {
            // char A..Z
            c = static_cast<char>(a+'A');
        }else if (a>=26 && a<=51) {
            // char a..z-
            c = static_cast<char>((a-26)+'a');
        }else if (a>=52 && a<=61) {
            // char 0..9
            c = static_cast<char>((a-52)+'0');
        }else if (a==62) {
            c = '+';
        }else if (a==63) {
            c = '/';
        }

        return c;
    }

public:
    /**
     * ASCII string encode into Base64.  Example encode would be "Hell0 Wor1d" => "SGVsbDAgV29yMWQ="
     *
     * \note Encoded length will always be larger than decoded length.  For pre-allocation \ref GetEncodeLength
     *
     * \param[in] s Source ASCII string pointer (NULL terminated)
     * \param[out] d Destination Base64 string pointer (will be NULL terminated)
     * \param[in] d_len Length of destination string buffer (Bytes/Characters)
     * \param[in] s_len Length of source ASCII string, characters (not including NULL), default 0 => strlen(s)
     * \return Destination string pointer
     */
    static char* Encode(const char* s, char* d, const uint16_t d_len, const uint16_t s_len=0) {
        uint16_t sl = s_len, dl = 0;
        struct bitstream_t b;

        if (!sl) {
            sl = strlen(s);
        }
        if (d_len) {
            d[0] = '\0';
        }
        if (sl && d_len) {
            for(uint16_t i=0; i<sl; i+=3) {
                b.u32 = 0;
                switch(sl-i) {
                    case 0 :
                        // Nothing
                        break;
                    case 1 :
                        b.u8[2] = s[i+0];
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.a);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.b);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(65);    // Padding
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(65);    // Padding
                        }
                        break;
                    case 2 :
                        b.u8[2] = s[i+0];
                        b.u8[1] = s[i+1];
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.a);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.b);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.c);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(65);    // Padding
                        }
                        break;
                    default :
                        b.u8[2] = s[i+0];
                        b.u8[1] = s[i+1];
                        b.u8[0] = s[i+2];
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.a);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.b);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.c);
                        }
                        if (dl<d_len) {
                            d[dl++] = MIMEBase64::ASCIIFromIndex(b.f.d);
                        }
                } // switch(sl-3)
                if (dl==d_len) {
                    break;
                }
            }
            // Install null
            d[ (dl==d_len) ? dl-1 : dl ] = '\0';
        } // if (sl & d_len)
    
        return d;
    } // Encode(...)

    
    /**
     * Get ASCII to Base64 encode length.  Example encode "Hell0 Wor1d" length would be 16
     *
     * \param[in] s Source ASCII string pointer (NULL terminated)
     * \param[in] s_len Length of source ASCII string, characters (not including NULL), default 0 => strlen(s)
     * \return Base64 character count in Bytes.  Doesn't include a NULL if left as string
     */
    static uint16_t GetEncodeLength(const char* s, const uint16_t s_len=0) {
        uint16_t sl = s_len;
        if (!s_len) {
            sl = strlen(s);
        }

        return sl ? ( (sl % 3) ? (sl/3+1)<<2 : (sl/3)<<2 ): 0;
    }


    /**
     * Base64 decode into ASCII string.  Example encode would be "SGVsbDAgV29yMWQ=" => "Hell0 Wor1d"
     *
     * \note Decoded length will always be smaller than encoded length.  For pre-allocation \ref GetDecodeLength
     *
     * \param[in] s Source Base64 string pointer (NULL terminated)
     * \param[out] d Destination ASCII string pointer (will be NULL terminated)
     * \param[in] d_len Length of destination string buffer (Bytes/Characters)
     * \param[in] s_len Length of source string (Bytes, not including NULL), default 0 => strlen(s)
     * \return Destination string pointer
     */
    static char* Decode(const char* s, char* d, const uint16_t d_len, const uint16_t s_len=0) {
        uint16_t sl = s_len, i, dl = 0;
        struct bitstream_t b;
        if (!s_len) {
            sl = strlen(s);
        }
        if (sl) {
            for(i=0; i<sl; i+=4) {
                b.f.a = IndexFromASCII(s[i+0]);
                b.f.b = IndexFromASCII(s[i+1]);
                b.f.c = IndexFromASCII(s[i+2]);
                b.f.d = IndexFromASCII(s[i+3]);
                d[dl++] = b.u8[2];
                d[dl++] = b.u8[1];
                d[dl++] = b.u8[0];
            }
        }
        d[dl]='\0';

        return d;
    } // Decode(...)


    /**
     * Get Base64 to ASCII decode length.  Example encode "SGVsbDAgV29yMWQ=" length would be 11
     *
     * \param[in] d Base64 string pointer (NULL terminated)
     * \param[in] d_len Length of source (Bytes) (not including NULL), default 0 => strlen(s)
     * \return ASCII character count.  Doesn't include a NULL if left as string
     */
    static uint16_t GetDecodeLength(const char* d, const uint16_t d_len=0) {
        uint16_t dl = d_len, l = 0;
        if (!d_len) {
            dl = strlen(d);
        }
        if (dl) {
            l = (dl>>2)*3;
            // Any padding?
            if (d[dl-1]=='=') {
                l--;
                if (d[dl-2]=='=') {
                    l--;
                }
            }
        }

        return l;
    }

}; // class MIMEBase64

}; // namespace ghttp

#endif // GHTTP_MIMEBASE64_H
