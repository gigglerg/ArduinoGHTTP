/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP request/response buffer
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPBUFFER_H
#define GHTTP_HTTPBUFFER_H

namespace ghttp {

/**
 * A class for character buffering with an interface for string manipulation used during both request and response processing
 */
class HTTPBuffer {
public:
    /**
     * Default constructor, make stable instance.  Buffer characteristics imply empty with no actual buffer
     */
    HTTPBuffer() : buffer_(NULL), length_(0), max_length_(0), offset_(0) { }


    /**
     * Constructor, make stable instance.  Setup buffer, maximum length, length and any offset.  The offset is primarily used 
     * to allow parts of the buffer to remain while other parts filled; useful when processing a response that still requires 
     * request data that would otherwise be overwritten.
     *
     * \param[in] buffer Character string/buffer pointer
     * \param[in] max_length Maximum length (characters)
     * \param[in] length Characters of user data present in buffer
     * \param[in] offset Data offset.  Processing starts at buffer[offset].  Revert to buffer[0] by \ref PrepareZeroOffsetData
     */
    HTTPBuffer(char* buffer, uint16_t max_length, uint16_t length=0, uint16_t offset=0) : max_length_(max_length) {
        SetBuffer(buffer, length, offset);
    }


#if defined(_MSC_VER)
    void Dump(char* s, uint16_t i, uint16_t l) const {
        bool zero = false;

        printf("%s (%u, %u) ptr 0x%p: \n000 ", s, length_, strlen(buffer_), buffer_);
        for(uint16_t c=0, u=0;u<l; u++, c++) {
            if (c==16) {
                printf("\n%03X ", u);
                c=0;
            }
            if (!zero) {
                if (buffer_[i+u]>=32 && buffer_[i+u]<127) {
                    printf("%C  ", buffer_[i+u]);
                }else {
                    printf("%02X ", buffer_[i+u]);
                }
            }else {
                printf("%02x ", buffer_[i+u]);
            }
            if (!buffer_[i+u]) {
                zero = true;
            }
        }
        printf("\n");
    }
#endif // defined(_MSC_VER)


    /**
     * Clear buffer, includes characteristics like length and offset
     *
     * \param[in] zero_clear Flag indicating entire buffer should be zero cleared (default false)
     */
    void Clear(const bool zero_clear=false) {
        if (length_ && offset_) {
            PrepareZeroOffsetData();
        }
        length_ = 0;
        offset_ = 0;
        if (zero_clear && max_length_) {
            memset(buffer_, 0, max_length_);
        }
    }


    /**
     * At accessor
     *
     * \param[in] i 0>= Index < \ref GetMaximum
     * \return Character or '\0' (index out of range)
     */
    char At(const uint16_t i) const {
        char c = '\0';

        if (buffer_ && i<max_length_) {
            c = buffer_[i];
        }

        return c;
    }


    /**
     * At accessor setter
     *
     * \attention Length is not changed, writing above \ref GetLength will set data if also less than \ref GetMaximum
     *
     * \param[in] i 0>= Index < \ref GetMaximum
     * \param[in] c Character
     * \retval true Written
     * \retval false No written (index out of range)
     */
    bool At(const uint16_t i, char c) {
        bool f = false;

        if (buffer_ && i<max_length_) {
            buffer_[i] = c;
            f = true;
        }

        return f;
    }


    bool operator+=(const HTTPSubstring& rhs) {
        return Append(rhs.Get());
    }


    bool operator+=(const HTTPSubstring* rhs) {
        return Append(rhs->Get());
    }


    bool operator+=(const char* rhs) {
        return Append(rhs);
    }

    uint16_t operator+=(const uint16_t& rhs) {
        SetLength(length_+rhs, true);

        return length_;
    }


    uint16_t operator-=(const uint16_t& rhs) {
        if (rhs>length_) {
            SetLength(0);
        }else {
            SetLength(length_-rhs);
        }

        return length_;
    }


    /**
     * Index operator, use to access characters as alternative to \ref At
     *
     * \param[in] i 0>= Index < \ref GetMaximum
     * return T pointer or NULL
     */
    char operator[] (const uint16_t i) {
        return At(i);
    }


    /**
     * Get buffer
     *
     * \return Buffer pointer
     * \retval NULL if buffer not setup
     */
    char* GetBuffer() const {
        return buffer_;
    }


    /**
     * Set buffer
     * Because Request and Responses may share the same buffer and some Request information maybe required for building a Response, use offset to keep required Request information buffered and usable
     * while Response being built.  At some point further in processing the offset can be removed with \ref PrepareZeroOffsetData moving back the Response data, overwriting the Request, ready to use.
     * \note Prerequisite max_length_ should be setup, if not invoke /ref SetMaximum before 
     *
     * \note Use \ref SetMaximum prior to setting buffer characteristics
     *
     * \param[in] buffer Character string/buffer pointer
     * \param[in] length Characters of user data present in buffer
     * \param[in] offset Data offset.  Processing starts at buffer[offset].  Revert to buffer[0] by \ref PrepareZeroOffsetData
     */
    void SetBuffer(char* buffer, uint16_t length=0, uint16_t offset=0) {
        if (length < max_length_ && (offset+length) < max_length_) {
            buffer_ = &buffer[offset];
            length_ = length;
            buffer_[length] = '\0';    // Initial response string null.  Might not mean much if octet stream
            offset_ = offset;
        }
    }


    /**
     * Get offset
     *
     * \note To set use SetBuffer
     *
     * \return Offset A non-zero indicates that \ref PrepareZeroOffsetData requires invoke at some point
     */
    uint16_t GetOffset() const {
        return offset_;
    }


    /**
     * Prepare zero offset data.  Internally shifts data so that anything contained within buffer[offset gets moved to 
     * buffer[0] and at the same time the offset is cleared.  If offset is required again then use \ref SetBuffer
     *
     * \note Use SetBuffer to reset a new offset if buffer reused
     */
    void PrepareZeroOffsetData() {
        if (offset_) {
            memmove( buffer_ - offset_, buffer_, max_length_ );
            buffer_-= offset_;
            offset_ = 0;
        }
    }


    /**
     * Get length.  This is the filled length or length of valid data held within buffer
     *
     * \return Characters
     */
    uint16_t GetLength() const {
        return length_;
    }


    /**
     * Set length.  This is the filled length or length of valid data held within buffer.  You may use this after using \ref At for actual data
     * setting
     *
     * \param[in] length Characters of user data present in buffer
     * \param[in] ignore_maximum Flag allowing length to be set larger than actual buffer size (default false)
     */
    void SetLength(const uint16_t length, const bool ignore_maximum=false) {
        if (ignore_maximum) {
            if (buffer_) {
                length_ = length;
            }
        }else {
            if (buffer_ && length < max_length_) {
                length_ = length;
            }
        }
    }

    
    /**
     * Get buffer maximum
     *
     * \return Characters.  0 indicates nothing can be held
     */
    uint16_t GetMaximum() const {
        return max_length_;
    }

    
    /**
     * Set buffer maximum
     *
     * \param[in] max_length Maximum length (characters)
     */
    void SetMaximum(const uint16_t max_length) {
        max_length_ = max_length;
    }


    /**
     * Append character string
     *
     * \param[in] str String pointer (NULL terminated)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Append(const char* str, uint16_t length=0) {
        return Insert(length_, str, length);
    }


    /**
     * Append unsigned character array
     *
     * \param[in] data Data pointer
     * \param[in] length Characters
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Append(const uint8_t* data, uint16_t length) {
        return Insert(length_, reinterpret_cast<const char*>(const_cast<uint8_t*>(data)), length);
    }


    /**
     * Append character
     *
     * \param[in] ch Character
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Append(char ch) {
        char cs[2] = { ch, '\0' };

        return Insert(length_, cs, 1);
    }


    /**
     * Append signed integer type T with given base
     *
     * \attention Conversion from numeric to string performed at end of character buffer.  This may fail if there isn't sufficient space
     *
     * \tparam T signed integer type
     * \param[in] v Type T integer value
     * \param[in] base Number base/radix, default 10
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    template<typename T>
    bool AppendInt(T v, uint8_t base=10) {
        bool done = false;
        uint8_t l;
        char *str;

        // Length in digits is an issue, guess maximum of 16 including NULL, obviously overkill for int8/int16 etc.
        if ((length_+16) < max_length_) {
            str = &buffer_[max_length_-16];
            l = StringHelper::FromInt<T>(v, str, base);
            HTTPBuffer::Append(str, l);
        }

        return done;
    }


    /**
     * Append unsigned integer type T with given base
     *
     * \attention Conversion from numeric to string performed at end of character buffer.  This may fail if there isn't sufficient space
     *
     * \tparam T unsigned integer type
     * \param[in] v Type T integer value
     * \param[in] base Number base/radix, default 10
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    template<typename T>
    bool AppendUInt(T v, uint8_t base=10) {
        bool done = false;
        uint8_t l;
        char *str;

        // Length in digits is an issue, guess maximum of 16 including NULL, obviously overkill for int8/int16 etc.
        if ((length_+16) < max_length_) {
            str = &buffer_[max_length_-16];
            l = StringHelper::FromUInt<T>(v, str, base);
            HTTPBuffer::Append(str, l);
        }

        return done;
    }


    /**
     * Append decimal type T with specific digits below decimal point
     *
     * \attention Conversion from numeric to string performed at end of character buffer.  This may fail if there isn't sufficient space
     *
     * \tparam T float/double type
     * \param[in] v Type T decimal value
     * \param[in] digits_below_dp Digits below decimal point, default 3
     * \param[in] str_allocation String allocation at end of buffer for conversion, Characters, default 16.  Increase as required for conversion digits
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    template<typename T>
    bool AppendDecimal(T v, uint8_t digits_below_dp=3, uint8_t str_allocation=16) {
        bool done = false;
        uint8_t l;
        char *str;

        if ((length_+str_allocation) < max_length_) {
            str = &buffer_[max_length_-str_allocation];
            l = StringHelper::FromDecimal(v, str, digits_below_dp);
            HTTPBuffer::Append(str, l);
        }

        return done;
    }


    /**
     * Insert character string at specific index
     *
     * \param[in] i Index >=0 n <= \ref GetLength (latter for append operation)
     * \param[in] str String pointer (NULL terminated)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Insert(uint16_t i, const char* str, uint16_t length=0) {
        bool done = false;
        
        if (!length) {
            length = strlen(str); // Doesn't include NULL
        }
        if ((length_ + length + 1) < max_length_) {
            if (i<length_) {
                memmove(&buffer_[i + length], &buffer_[i], length_ - i);
            }
            memcpy(&buffer_[i], str, length);
            length_ += length;
            buffer_[length_] = '\0';
            done = true;
        }
        
        return done;
    }


    /**
     * Insert padding character (a repeated character) at specific index to given length
     *
     * \param[in] i Index >=0 n <= \ref GetLength (latter for append operation)
     * \param[in] c Character
     * \param[in] length Characters N to insert
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool InsertPadding(uint16_t i, char c, uint16_t length) {
        bool done = false;

        if (length && (length_ + length) < max_length_) {
            /* Normally moving to <length_ would be it but since mostly it is string data and the null terminator isn't included in length, he if inserting at the end we assume a null exists and move it
             * this shouldn't be an issue because length_ is still not changed to include the null
             */
            if (i<=length_) {
                memmove(&buffer_[i + length], &buffer_[i], 1 + length_ - i);
            }
            for(uint32_t u=0; u<length; u++) {
                buffer_[i + u] = c;
            }
            length_ += length;
            done = true;
        }
        
        return done;
    }

    
    /**
     * Remove character(s) at specific start index to end index
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] e Index >=0 n < \ref GetLength
     * \return Remove state
     * \retval true When successful
     * \retval false When failed
     */
    bool Remove(uint16_t i, uint16_t e) {
        bool done = false;
        uint16_t l;

        if (i<=e && (e<max_length_-1) && e<length_) {
            l = length_ - e;
            memmove(&buffer_[i], &buffer_[e+1], l);
            length_ -= (1+e-i);
            done = true;
        }
        
        return done;
    }
    
    
    /**
     * Replace character(s) at specific start index to end index with given character string
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] e Index >=0 n < \ref GetLength
     * \param[in] str String pointer (NULL terminated)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Replace state
     * \retval true When successful
     * \retval false When failed
     */
    bool Replace(uint16_t i, uint16_t e, const char* str, uint16_t length=0) {
        bool done = false;
        uint16_t cl = e-i;

        if (i<=length_ && i<e) {
            if (!length) {
                length = strlen(str); // Doesn't include NULL
            }
            if (((length_ - cl) + length) < max_length_) {
                if (cl < length) { // Pad required?
                    done =! InsertPadding(i, '.', length - cl -1);
                }else if (cl > length) { // Remove required?
                    done =! Remove(i, i + cl - length);
                }
                if (!done) {
                    memcpy(&buffer_[i], str, length);
                    done = true;
                }else {
                    // Pad failed??
                    done = false;
                }
            }
        }

        return done;
    }


#if defined(ARDUINO_ARCH_AVR)
    bool operator+=(PGM_VOID_P rhs) {
        return Append(rhs);
    }


    /**
     * Append character string
     *
     * \param[in] str String pointer (NULL terminated, flash based)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Append(PGM_VOID_P str, uint16_t length=0) {
        return Insert(length_, str, length);
    }


    /**
     * Insert character string at specific index
     *
     * \param[in] i Index >=0 n <= \ref GetLength (latter for append operation)
     * \param[in] str String pointer (NULL terminated, flash based)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Append state
     * \retval true When successful
     * \retval false When failed
     */
    bool Insert(uint16_t i, PGM_VOID_P str, uint16_t length=0) {
        bool done = false;
        
        if (!length) {
            length = PGM_STRLEN(str); // Doesn't include NULL
        }
        if ((length_ + length + 1) < max_length_) {
            if (i<length_) {
                memmove(&buffer_[i + length], &buffer_[i], length_ - i);
            }
            PGM_MEMCPY(&buffer_[i], str, length);
            length_ += length;
            buffer_[length_] = '\0';
            done = true;
        }
        
        return done;
    }


    /**
     * Replace character(s) at specific start index to end index with given character string
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] e Index >=0 n < \ref GetLength
     * \param[in] str String pointer (NULL terminated, flash based)
     * \param[in] length Characters from start of string to append.  Default 0 => strlen(str)
     * \return Replace state
     * \retval true When successful
     * \retval false When failed
     */
    bool Replace(uint16_t i, uint16_t e, PGM_VOID_P str, uint16_t length=0) {
        bool done = false;
        uint16_t cl = e-i;

        if (i<=length_ && i<e) {
            if (!length) {
                length = PGM_STRLEN(str); // Doesn't include NULL
            }
            if (((length_ - cl) + length) < max_length_) {
                if (cl < length) { // Pad required?
                    done =! InsertPadding(i, '.', length - cl -1);
                }else if (cl > length) { // Remove required?
                    done =! Remove(i, i + cl - length);
                }
                if (!done) {
                    PGM_MEMCPY(&buffer_[i], str, length);
                    done = true;
                }else {
                    // Pad failed??
                    done = false;
                }
            }
        }

        return done;
    }
#endif // !defined(ARDUINO_ARCH_AVR)


protected:
    /**
     * Find end of line
     *
     * \note An EOL is LF+CR in that order and the return index will be of CR
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] ei Index >=0 n < \ref GetLength.  Maximum expected line length between i and ei
     * \return Index of CR (EOL found)
     * \retval ei+1 Not found
     */
    uint16_t FindEOL(uint16_t i, uint16_t ei) const {
        uint8_t f = 0;

        do {
            if (i >= ei) {
                break;
            }
            if (!f) {
                if ('\r' == buffer_[i]) {
                    f++;
                }
            }else {
                if ('\n' == buffer_[i]) {
                    f++;
                    break;
                }
                f = 0;
            }
            i++;
        }while(1);

        if (2 != f) {
            i = ei+1;    // Invalid, outside range
        }

        return i;
    } // FindEOL(...)


    /**
     * Search for a substring, case sensitive
     *
     * \note You can find the next substring within s1 by moving forward size of s2 string and passing this pointer in as s1
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] ei Index >=0 n < \ref GetLength.  Maximum search length between i and ei
     * \param[in] str Pointer to subject or substring to search for
     * \param[in] str_length_max Sub string maximum length (characters).  Default 0 => strlen(str)
     * \return Starting index within buffer where subject string found.  If larger-equal than ei => not found
     */
    uint16_t FindSubString(uint16_t i, uint16_t ei, const char* str, uint16_t str_length_max=0) const {
        return StringHelper::FindSubString(buffer_, str, i, true, ei, str_length_max);
    }


    /**
     * Find terminator characters.  Can search only for characters that are included in terminator string
     *
     * \note Set parameter invert_speical_only to search for characters that only exist in this string
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] include_special Terminator string.  Contains characters you either want to find or must include(invert_speical_only=true)
     * \param[in] invert_special_only Flag controlling termination where search ends when character exists in terminator string(false) or 
     * must exist in terminator string(true).
     * \param[in] i_max Maximum index where search should stop, default 0 => strlen(GetBuffer())
     * \return Index where terminator character found(invert_special_only=false) or not found (invert_special_only=true)
     * \retval i_max+1 Than no terminator found
     */
    uint16_t FindTerminator(uint16_t i, const char* include_special, bool invert_special_only=false, uint16_t i_max=0) const {
        if (!i_max) {
            i_max = length_;
        }

        return StringHelper::FindTerminator(i, buffer_, include_special, invert_special_only, i_max);
    }


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Search for a substring, case sensitive
     *
     * \note You can find the next substring within s1 by moving forward size of s2 string and passing this pointer in as s1
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] ei Index >=0 n < \ref GetLength.  Maximum search length between i and ei
     * \param[in] str Pointer to subject or substring to search for (flash base)
     * \param[in] str_length_max Sub string maximum length (characters).  Default 0 => strlen(str)
     * \return Starting index within buffer where subject string found.  If larger-equal than ei => not found
     */
    uint16_t FindSubString(uint16_t i, uint16_t ei, PGM_VOID_P str, uint16_t str_length_max=0) const {
        return StringHelper::FindSubString(buffer_, str, i, true, ei, str_length_max);
    }


    /**
     * Find terminator characters.  Can search only for characters that are included in terminator string
     *
     * \note Set parameter invert_speical_only to search for characters that only exist in this string
     *
     * \param[in] i Index >=0 n < \ref GetLength
     * \param[in] include_special Terminator string (flash based).  Contains characters you either want to find or must include(invert_speical_only=true)
     * \param[in] invert_special_only Flag controlling termination where search ends when character exists in terminator string(false) or 
     * must exist in terminator string(true).
     * \param[in] i_max Maximum index where search should stop, default 0 => strlen(GetBuffer())
     * \return Index where terminator character found(invert_special_only=false) or not found (invert_special_only=true)
     * \retval i_max+1 Than no terminator found
     */
    uint16_t FindTerminator(uint16_t i, PGM_VOID_P include_special, bool invert_special_only=false, uint16_t i_max=0) const {
        if (!i_max) {
            i_max = length_;
        }

        return StringHelper::FindTerminator(i, buffer_, include_special, invert_special_only, i_max);
    }
#endif // !defined(ARDUINO_ARCH_AVR)

protected:
    uint16_t    length_;                            ///< Data in buffer, length in characters
    uint16_t    max_length_;                        ///< Maximum data in buffer in characters
    uint16_t    offset_;                            ///< Offset (characters) of unused head of buffer
    char*        buffer_;                           ///< Character buffer pointer
}; // class HTTPBuffer

}; // namespace ghttp

#endif // GHTTP_HTTPBUFFER_H
