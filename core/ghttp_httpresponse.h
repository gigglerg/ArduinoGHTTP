/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP response
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPRESPONSE_H
#define GHTTP_HTTPRESPONSE_H

namespace ghttp {

/**
 * HTTP Response class with helpers to assist in the construction of valid HTTP response messages.  The resulting data in HTTPBuffer should be sent over TCP/IP.  It is 
 * expected that this will all be after handling a HTTP request
 *
 * \tparam S Status code class used in response
 */
template<class S>
class HTTPResponse : public HTTPBuffer {
public:
    S            status;            ///< Status code instance

public:
    /**
     * Default constructor, make stable instance.  Buffer setup with 0 length and NULL pointer
     *
     * \note Use \ref SetBuffer to setup the response buffer characteristics
     */
    HTTPResponse() : HTTPBuffer() { }


    /**
     * Constructor, make stable instance, includes response buffer characteristics
     *
     * \param[in] buffer Buffer String pointer to buffer first character
     * \param[in] max_length Buffer maximum length (including NULL)
     * \param[in] length Buffer current length (if data exists already), default 0 (characters)
     * \param[in] offset Offset (characters) from buffer[0] to allow request header and other request data access during construction of response.  Default 0
     */
    HTTPResponse(char* buffer, uint16_t max_length, uint16_t length=0, uint16_t offset=0) : HTTPBuffer(buffer, max_length, length, offset) { }


    /**
     * Calculate and return content length - headers.  It is assumed response headers are complete when content exists
     *
     * \return Content length (characters)
     */
    uint16_t GetContentLength() const {
        uint16_t l = GetEndOfHeaders();

        if (l) {
            l = length_ - l;
        }

        return l;
    }


    /**
     * Response header string, append with optional replace
     *
     * \note Can be used to set status header (line).  When doing so pass protocol and version only like "HTTP/1.0" etc.
     * \note Besides the status header, all other headers must be complete.  No header should include newline or carriage return
     * \attention Use until \ref HeaderComplete has been invoked.  Also if there is any content length do not attempt to add headers
     *
     * \param[in] header_str Header string pointer (NULL terminated).  Header format is "<name>: <data>" unless setting status header
     * \param[in] replace Flag indicating replacement, default true.  When true all other like headers will be removed prior to new insert
     * \return State, true on success otherwise false
     */
    bool Header(const char* header_str, const bool replace=true) {
        bool set = false;

        if (header_str && strlen(header_str)>5) {
            // HTTP/1.0 => First line/status header?
            if (!StringHelper::StrNCmp("HTTP/", header_str, 5, false)) {
                set = HeaderForStatus(header_str);
            }else {
                set = (!GetEndOfHeaders() ? HeaderSet(header_str, replace) : false);
            }
        }

        return set;
    }


    /**
     * Header for status, caller should provide protocol and protocol version information for header string construction
     *
     * \todo Refactor with \ref HeaderRemove
     *
     * \param[in] protocol_str Protocol (including version) string pointer.  Format like "HTTP/1.0" etc.
     * \return State, true on success otherwise false
     */
    bool HeaderForStatus(const char* protocol_str) {
        bool set = false;
        uint16_t i_1st_eol = FindEOL(0, length_);
        uint8_t t;

        if (protocol_str) {
            uint16_t new_str_len;

            set = true;
            t = strlen(protocol_str);
            new_str_len = t;

            // Figure out how long new line string is
            t = status.GetStringLength();
            new_str_len += t + 3; // + 3 (space + CR + LF)

            // Protocol must have length
            if (t) {
                uint16_t diff;

                // Any first line header?
                if (i_1st_eol>5 && i_1st_eol<length_) {
                    // Header(s) exist
                    i_1st_eol++;

                    // We can't simply replace because we don't have a single replace string so pad as required
                    if (i_1st_eol < new_str_len) {
                        diff = new_str_len - i_1st_eol;
                        set = InsertPadding(0, '.', diff);
                    }else if (i_1st_eol > new_str_len) {
                        set = Remove(new_str_len, i_1st_eol-1);
                    }
                }else {
                    // No header set
                    length_ = new_str_len;
                    if ((length_ + 3) >= max_length_) {
                        set = false;
                    }
                }
                if (set && new_str_len>2) {
                    t = strlen(protocol_str);
                    strcpy(buffer_, protocol_str);
                    buffer_[t] = ' ';
                    buffer_[t+1] = '\0';
                    status.GetString(&buffer_[t+1]); // Concatenate status
                    buffer_[new_str_len-2] = '\r';
                    buffer_[new_str_len-1] = '\n';
                }
            }
        }

        return set;
    } // HeaderForStatus(...)


    /**
     * Header remove by given header name string.  Optionally removes all like headers
     *
     * \param[in] header_str String pointer to header name (NULL terminated).  This can be entire header or string before colon (the name).  The former case 
     * only the name is matched not the header data during removal.
     * \param[in] all Flag to remove all or first matching header, default true
     * \return State, true on success otherwise false
     */
    bool HeaderRemove(const char* header_str, const bool all=true) {
        return (!GetEndOfHeaders() ? HeaderRemoveForce(header_str, all) : false);
    }


    /**
     * Force remove any existing content
     *
     * \attention Does nothing if not content exists.  After content removed, header complete still remains
     * \return State, true on success otherwise false
     */
    bool NoContent() {
        bool done = false;
        uint16_t l = GetContentLength();

        if (l) {
            length_-=l;
            At(length_, '\0');
            done = true;
        }

        return done;
    }


    // session_id string from SessionId instance related to request
    // session_cookie_addtion SameSite=Strict; HttpOnly
    // /note adds session cookie if any
#if GHTTP_SERVER_USE_SESSIONS == 1
    /**
     * Issue header complete with optional session cookie id and any session cookie addition string
     *
     * \attention Invoke before \ref Complete
     *
     * \param[in] session_id String pointer, obtained from request session (if any).  Default NULL => no session cookie addition upon header completion
     * \param[in] session_cookie_addition String pointer.  Only used session_id string setup.  Default NULL => no addition.  Example might be "HttpOnly" etc.
     * \return State, true on success otherwise false
     */
    bool HeaderComplete(const char* session_id=NULL, const char* session_cookie_addition=NULL) {
#else // GHTTP_SERVER_USE_SESSIONS != 1
    /**
     * Issue header complete
     *
     * \attention Invoke before \ref Complete
     *
     * \return State, true on success otherwise false
     */
    bool HeaderComplete() {
#endif
        bool c = false;

        if (!GetEndOfHeaders() && GetEndOfHeaders(true)) {
#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1 && defined(GHTTP_SERVER_STRING_SESSION_COOKIE)
            // Add session cookie where set
            if (session_id) {
                c = Append(T("Set-Cookie: "));
                if (!c) {
                    goto HeaderComplete_Exit;
                }
                c = Append(GHTTP_SERVER_STRING_SESSION_COOKIE);
                if (!c) {
                    goto HeaderComplete_Exit;
                }
                c = Append("=");
                if (!c) {
                    goto HeaderComplete_Exit;
                }
                c = Append(session_id);
                if (!c) {
                    goto HeaderComplete_Exit;
                }
                if (session_cookie_addition && strlen(session_cookie_addition)) {
                    c = Append(T("; "));
                    if (!c) {
                        goto HeaderComplete_Exit;
                    }
                    c = Append(session_cookie_addition);
                    if (!c) {
                        goto HeaderComplete_Exit;
                    }
                }
                c = Append(T("\r\n"));
                if (!c) {
                    goto HeaderComplete_Exit;
                }
            }
#endif // GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1 && defined(GHTTP_SERVER_STRING_SESSION_COOKIE)
            c = Append(T("\r\n"));
        }
        HeaderComplete_Exit:

        return c;
    } // HeaderComplete(...)


    /**
     * Query are headers complete
     *
     * \retval true When final CR+LF included and content start present
     * \retval false When headers not complete
     */
    bool IsHeaderComplete() const {
        return !GetEndOfHeaders()? true : false;
    }


    /**
     * Complete, invoke to indicate content complete.
     *
     * \note Make sure you have appended all request data prior to invoke
     * \attention Invoke after \ref HeaderComplete
     * \attention HTTPBuffer::PrepareZeroOffsetData invoked internally when result yields true
     * \return State, true on success otherwise false.  In the latter case make sure headers are complete prior
     */
    bool Complete(const bool include_content_length=true) {
        bool c = true;
        char cl_str[24];

        if (length_ >= 4 && GetEndOfHeaders(true) > 0) {
            if (include_content_length) {
                strcpy(cl_str, "Content-Length: ");
                StringHelper::FromUInt<uint16_t>(GetContentLength(), &cl_str[16]);
                c = HeaderSet(cl_str);
            }
            // OK? then realign request data within buffer ready for use
            if (c) {
                PrepareZeroOffsetData();
            }
        }
        return c;
    }


    /**
     * Get end of headers (character index)
     *
     * \param[in] include_incomplete Flag indicating a length required even if headers are not complete.  Default false
     * \return Character index into HTTPBuffer where next append should be made
     */
    uint16_t GetEndOfHeaders(const bool include_incomplete=false) const {
        uint16_t i = 0;

        if (max_length_>=length_ && length_>4) {
            i = HTTPBuffer::FindSubString(0, length_, "\r\n\r\n");
            // Not found?
            if (i > length_) {
                i = 0;
                if (include_incomplete) {
                    uint16_t e;
                    
                    do {
                        e = HTTPBuffer::FindEOL(i, length_);
                        if (e>length_) {
                            // Take i
                            break;
                        }
                        i = e + 1;
                    }while(1);
                }
            }else {
                i+=4;    // Move over CR+LF+CR+LF
            }
        }

        return i;
    } // GetEndOfHeaders(...)


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Response header string, append with optional replace
     *
     * \note Can be used to set status header (line).  When doing so pass protocol and version only like "HTTP/1.0" etc.
     * \note Besides the status header, all other headers must be complete.  No header should include newline or carriage return
     * \attention Use until \ref HeaderComplete has been invoked.  Also if there is any content length do not attempt to add headers
     *
     * \param[in] header_str Header string pointer (NULL terminated, flash based).  Header format is "<name>: <data>" unless setting status header
     * \param[in] replace Flag indicating replacement, default true.  When true all other like headers will be removed prior to new insert
     * \return State, true on success otherwise false
     */
    bool Header(PGM_VOID_P header_str, const bool replace=true) {
        bool set = false;

        if (header_str && PGM_STRLEN(header_str)>5) {
            // HTTP/1.0 => First line/status header?
            if (!StringHelper::StrNCmp("HTTP/", header_str, 5, false)) {

                set = HeaderForStatus(header_str);
            }else {
                set = (!GetEndOfHeaders() ? HeaderSet(header_str, replace) : false);
            }
        }

        return set;
    }


    /**
     * Header for status, caller should provide protocol and protocol version information for header string construction
     *
     * \todo Refactor with \ref HeaderRemove
     *
     * \param[in] protocol_str Protocol (including version) string pointer, flash based.  Format like "HTTP/1.0" etc.
     * \return State, true on success otherwise false
     */
    bool HeaderForStatus(PGM_VOID_P protocol_str) {
        bool set = false;
        uint16_t i_1st_eol = FindEOL(0, length_);
        uint8_t t;

        if (protocol_str) {
            uint16_t new_str_len;

            set = true;
            t = PGM_STRLEN(protocol_str);
            new_str_len = t;

            // Figure out how long new line string is
            t = status.GetStringLength();
            new_str_len += t + 3; // + 3 (space + CR + LF)

            // Protocol must have length
            if (t) {
                uint16_t diff;

                // Any first line header?
                if (i_1st_eol>5 && i_1st_eol<length_) {
                    // Header(s) exist
                    i_1st_eol++;

                    // We can't simply replace because we don't have a single replace string so pad as required
                    if (i_1st_eol < new_str_len) {
                        diff = new_str_len - i_1st_eol;
                        set = InsertPadding(0, '.', diff);
                    }else if (i_1st_eol > new_str_len) {
                        set = Remove(new_str_len, i_1st_eol-1);
                    }
                }else {
                    // No header set
                    length_ = new_str_len;
                    if ((length_ + 3) >= max_length_) {
                        set = false;
                    }
                }
                if (set && new_str_len>2) {
                    t = PGM_STRLEN(protocol_str);
                    PGM_MEMCPY(buffer_, protocol_str, t);
//                    strcpy(buffer_, protocol_str);
                    buffer_[t] = ' ';
                    buffer_[t+1] = '\0';
                    status.GetString(&buffer_[t+1]); // Concatenate status
                    buffer_[new_str_len-2] = '\r';
                    buffer_[new_str_len-1] = '\n';
                }
            }
        }

        return set;
    } // HeaderForStatus(...)


    /**
     * Header remove by given header name string.  Optionally removes all like headers
     *
     * \param[in] header_str String pointer to header name (NULL terminated, flash based).  This can be entire header or string before colon (the name).  The former case 
     * only the name is matched not the header data during removal.
     * \param[in] all Flag to remove all or first matching header, default true
     * \return State, true on success otherwise false
     */
    bool HeaderRemove(PGM_VOID_P header_str, const bool all=true) {
        return (!GetEndOfHeaders() ? HeaderRemoveForce(header_str, all) : false);
    }

#endif // !defined(ARDUINO_ARCH_AVR)

protected:
    /**
     * Set response header string, append with optional replace.  Employed internally for header setting and has less restrictions than \ref Header
     *
     * \note May have partly set header or removed existing upon failure
     * \note Can be used to set status header (line).  When doing so pass protocol and version only like "HTTP/1.0" etc.
     * \note Besides the status header, all other headers must be complete.  No header should include newline or carriage return
     *
     * \param[in] header_str Header string pointer (NULL terminated).  Header format is "<name>: <data>" unless setting status header
     * \param[in] replace Flag indicating replacement, default true.  When true all other like headers will be removed prior to new insert
     * \return State, true on success otherwise false
     */
    bool HeaderSet(const char* header_str, const bool replace=true) {
        uint16_t header_length, i, e;
        bool set = false;

        // Header string?
        if (header_str) {
            // Find header string terminator
            header_length = strlen(header_str);

            // Check header string contains "[\w]+\:" before we continue
            i = StringHelper::FindTerminator(0, header_str, ":", false, header_length);
            if (i < header_length) {
                // Found.  No white space before
                e = StringHelper::FindTerminator(0, header_str, " \t", false, i);
                if (e < i) {
                    // Found, this is a problem, prevent further process and fail
                    header_length = 0;
                }
            }else {
                // No colon, prevent further process and fail
                header_length = 0;
            }

            if (header_length > 1 && (length_ + header_length + 4) < max_length_) { // +4 because EOH marker is another CR+LF which must come before content
                // Not control codes
                if (header_str[0] > 32) {
                    // Replace or append?
                    if (replace) {
                        set = HeaderRemoveForce(header_str, true);
                    }else {
                        set = true;
                    }

                    // Any remove OK?
                    if (set) {
                        // if (!replace) => append but we may have to insert depending where we are with content and header building

                        i = GetEndOfHeaders();
                        if (i) {
                            // Insert required as user headers complete but content length or others may still be required (ok essentially incomplete!)
                            i-=2; // move back CR+LF
                            set = Insert(i, header_str, header_length);
                            if (set) {
                                set = Insert(i + header_length, "\r\n", 2);
                            }
                        }else {
                            set = Append(header_str, header_length);
                            if (set) {
                                set = Append("\r\n", 2);
                            }
                        }
                    }
                }
            } // if (header_length > 1)
        }

        return set;
    } // HeaderSet(...)


    /**
     * Header remove by given header name string.  Optionally removes all like headers and has less restrictions than \ref HeaderRemove
     *
     * \note Can remove even after headers complete
     *
     * \param[in] header_str String pointer to header name (NULL terminated).  This can be entire header or string before colon (the name).  The former case 
     * only the name is matched not the header data during removal.
     * \param[in] all Flag to remove all or first matching header, default true
     * \return State, true on success otherwise false
     */
    bool HeaderRemoveForce(const char* header_str, const bool all=true) {
        uint16_t header_length, reduced_header_length, i, e;
        bool done = false;

        // Header string?
        if (header_str) {
            // Find header string terminator
            header_length = strlen(header_str);
            if (header_length > 1 && (length_ + header_length + 4) < max_length_) { // +4 because EOH marker is another CR+LF which must come before content
                // Not control codes
                if (header_str[0] > 32) {
                    // Find head name
                    reduced_header_length = StringHelper::FindTerminator(0, header_str, ": ", false, header_length);
                    if (reduced_header_length >= header_length) {
                        // Not found.  Assume entire header string just the header name
                        reduced_header_length = header_length;
                    }

                    if (length_>reduced_header_length) {
                        // There maybe multiple headers, remove all
                        while(1) {
                            i = FindSubString(0, length_, header_str, reduced_header_length);
                            if (i >= length_) {
                                done = true;
                                break; // Done...
                            }
                            if (':' != buffer_[i + reduced_header_length]) {
                                break;
                            }
                            e = FindEOL(i, length_);
                            if (e > length_) {
                                break;
                            }
                            if (!Remove(i, e)) {
                                break;
                            }
                            if (!all) {
                                done = true;
                                break;
                            }
                        } // while(1)
                    }else {
                        // Can't be present to remove
                        done = true;
                    }
                } // if (header_str[0] > 32)
            } // if (header_length > 1 && (length_ + header_length + 4) < max_length_)
        } // if (header_str)

        return done;
    } // HeaderRemoveForce(...)


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Set response header string, append with optional replace.  Employed internally for header setting and has less restrictions than \ref Header
     *
     * \note May have partly set header or removed existing upon failure
     * \note Can be used to set status header (line).  When doing so pass protocol and version only like "HTTP/1.0" etc.
     * \note Besides the status header, all other headers must be complete.  No header should include newline or carriage return
     *
     * \param[in] header_str Header string pointer (NULL terminated, flash based).  Header format is "<name>: <data>" unless setting status header
     * \param[in] replace Flag indicating replacement, default true.  When true all other like headers will be removed prior to new insert
     * \return State, true on success otherwise false
     */
    bool HeaderSet(PGM_VOID_P header_str, const bool replace=true) {
        uint16_t header_length, i, e;
        bool set = false;

        // Header string?
        if (header_str) {
            // Find header string terminator
            header_length = PGM_STRLEN(header_str);

            // Check header string contains "[\w]+\:" before we continue
            i = StringHelper::FindTerminator(0, header_str, ":", false, header_length);
            if (i < header_length) {
                // Found.  No white space before
                e = StringHelper::FindTerminator(0, header_str, " \t", false, i);
                if (e < i) {
                    // Found, this is a problem, prevent further process and fail
                    header_length = 0;
                }
            }else {
                // No colon, prevent further process and fail
                header_length = 0;
            }

            if (header_length > 1 && (length_ + header_length + 4) < max_length_) { // +4 because EOH marker is another CR+LF which must come before content
                // Not control codes
                if (PGM_CHAR(header_str) > 32) {
                    // Replace or append?
                    if (replace) {
                        set = HeaderRemoveForce(header_str, true);
                    }else {
                        set = true;
                    }

                    // Any remove OK?
                    if (set) {
                        // if (!replace) => append but we may have to insert depending where we are with content and header building

                        i = GetEndOfHeaders();
                        if (i) {
                            // Insert required as user headers complete but content length or others may still be required (ok essentially incomplete!)
                            i-=2; // move back CR+LF
                            set = Insert(i, header_str, header_length);
                            if (set) {
                                set = Insert(i + header_length, "\r\n", 2);
                            }
                        }else {
                            set = Append(header_str, header_length);
                            if (set) {
                                set = Append("\r\n", 2);
                            }
                        }
                    }
                }
            } // if (header_length > 1)
        }

        return set;
    } // HeaderSet(...)


    /**
     * Header remove by given header name string.  Optionally removes all like headers and has less restrictions than \ref HeaderRemove
     *
     * \note Can remove even after headers complete
     *
     * \param[in] header_str String pointer to header name (NULL terminated, flash based).  This can be entire header or string before colon (the name).  The former case 
     * only the name is matched not the header data during removal.
     * \param[in] all Flag to remove all or first matching header, default true
     * \return State, true on success otherwise false
     */
    bool HeaderRemoveForce(PGM_VOID_P header_str, const bool all=true) {
        uint16_t header_length, reduced_header_length, i, e;
        bool done = false;

        // Header string?
        if (header_str) {
            // Find header string terminator
            header_length = PGM_STRLEN(header_str);
            if (header_length > 1 && (length_ + header_length + 4) < max_length_) { // +4 because EOH marker is another CR+LF which must come before content
                // Not control codes
                if (PGM_CHAR(header_str) > 32) {
                    // Find head name
                    reduced_header_length = StringHelper::FindTerminator(0, header_str, ": ", false, header_length);
                    if (reduced_header_length >= header_length) {
                        // Not found.  Assume entire header string just the header name
                        reduced_header_length = header_length;
                    }

                    // There maybe multiple headers, remove all
                    while(1) {
                        i = FindSubString(0, length_, header_str, reduced_header_length);
                        if (i >= length_) {
                            done = true;
                            break; // Done...
                        }
                        if (':' != buffer_[i + reduced_header_length]) {
                            break;
                        }
                        e = FindEOL(i, length_);
                        if (e > length_) {
                            break;
                        }
                        if (!Remove(i, e)) {
                            break;
                        }
                        if (!all) {
                            done = true;
                            break;
                        }
                    } // while(1)
                } // if (header_str[0] > 32)
            } // if (header_length > 1 && (length_ + header_length + 4) < max_length_)
        } // if (header_str)

        return done;
    } // HeaderRemoveForce(...)
#endif // !defined(ARDUINO_ARCH_AVR)

}; // class HTTPResponse

}; // namespace ghttp

#endif // GHTTP_HTTPRESPONSE_H
