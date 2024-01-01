/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP request
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPREQUEST_H
#define GHTTP_HTTPREQUEST_H

namespace ghttp {

#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1

#if GHTTP_REQUEST_PROCESS_COOKIES == 1
template<uint8_t N, uint8_t V, uint8_t CT, uint8_t FV, uint8_t CN>
#else // GHTTP_REQUEST_PROCESS_COOKIES != 1
template<uint8_t N, uint8_t V, uint8_t CT, uint8_t FV>
#endif

#else // GHTTP_REQUEST_PROCESS_POST_FILE_VARS != 1

#if GHTTP_REQUEST_PROCESS_COOKIES == 1
template<uint8_t N, uint8_t V, uint8_t CT, uint8_t CN>
#else // GHTTP_REQUEST_PROCESS_COOKIES != 1
template<uint8_t N, uint8_t V, uint8_t CT>
#endif

#endif
/**
 * HTTP Request class with helpers to assist in the parsing of valid HTTP request messages, likely received over TCP/IP
 *
 * Refer: RFC 1738, RFC 7578 & RFC 1867, RFC 6265, RFC 1735
 *
 * \tparam N Maximum parsed headers
 * \tparam V Maximum parsed POST/GET variables
 * \tparam CT Maximum content type strings.  Content type header broken down into strings
 * \tparam FV Maximum parsed file POST variables.  Exists if GHTTP_REQUEST_PROCESS_POST_FILE_VARS=1
 * \tparam CN Maximum cookies in Cookie jar.  Exists if GHTTP_REQUEST_PROCESS_COOKIES=1
 */
class HTTPRequest : public HTTPBuffer {
public:
    /**
     * Enum of possible request parse errors
     */
    typedef enum {
        ERR_NONE,                                   ///<  0 - No error
        ERR_SEGMENTATION,                           ///<  1 - Parse information.  More data expected
        ERR_PARSE_EOF,                              ///<  2 - Parse error. End of File => to much data sent from host
        ERR_PARSE_BAD_FIRST_LINE_SOL,               ///<  3 - Parse error. First line contained unexpected data at SOL
        ERR_PARSE_BAD_METHOD,                       ///<  4 - Parse error. Bad method string
        ERR_PARSE_BAD_URL,                          ///<  5 - Parse error. Bad URL string
        ERR_PARSE_BAD_GET_PARAM_NAME,               ///<  6 - Parse error. Bad GET parameter name
        ERR_PARSE_BAD_GET_PARAM_VALUE,              ///<  7 - Parse error. Bad GET parameter value
        ERR_PARSE_BAD_GET_PARAM_TO_MANY,            ///<  8 - Parse error. To many GET parameters
        ERR_PARSE_BAD_PROTOCOL,                     ///<  9 - Parse error. Bad protocol string
        ERR_PARSE_BAD_PROTOCOL_VERSION,             ///< 10 - Parse error. Bad protocol version string
        ERR_PARSE_BAD_FIRST_LINE_NO_EOL,            ///< 11 - Parse error. Bad first line, termination of
        ERR_PARSE_BAD_HEADER_LINE_SOL,              ///< 12 - Parse error. Bad header line string, unexpect data at SOL
        ERR_PARSE_BAD_HEADER_LINE_NAME,             ///< 13 - Parse error. Bad header line string at attribute name
        ERR_PARSE_BAD_HEADER_LINE_VALUE,            ///< 14 - Parse error. Bad header line string at value
        ERR_PARSE_HEADER_LIMIT,                     ///< 15 - Parse error. Header string limit reached when appending data
        ERR_PARSE_BAD_HEADER_LINE_NO_EOL,           ///< 16 - Parse error. Bad header line, termination of
        ERR_PARSE_UNEXPECTED_DATA,                  ///< 17 - Parse error. To much data in request buffer
        ERR_PARSE_CONTENT_LENGTH_MISMATCH,          ///< 18 - Parse error. Content length header string differs from actual (read) content length
        ERR_PARSE_BAD_COOKIE_NAME,                  ///< 19 - Parse error. Processing header, bad cookie name found
        ERR_PARSE_BAD_COOKIE_VALUE,                 ///< 20 - Parse error. Processing header, bad cookie value found
        ERR_PARSE_BAD_COOKIE_TO_MANY,               ///< 21 - Parse error. Processing header, to many cookies in set cannot add more
        ERR_PARSE_BAD_POST_PARAM_NAME,              ///< 22 - Parse error. Bad POST parameter name
        ERR_PARSE_BAD_POST_PARAM_VALUE,             ///< 23 - Parse error. Bad POST parameter value
        ERR_PARSE_BAD_POST_PARAM_TO_MANY,           ///< 24 - Parse error. To many POST parameters, cannot add more
        ERR_PARSE_BAD_POST_FILE_PARAM_TO_MANY,      ///< 25 - Parse error. To many POST FILE parameters, cannot add more
        ERR_PARSE_BAD_CTYPE_NAME,                   ///< 26 - Parse error. Bad content type parameter name
        ERR_PARSE_BAD_CTYPE_VALUE,                  ///< 27 - Parse error. Bad content type parameter value
        ERR_PARSE_BAD_CTYPE_TO_MANY,                ///< 28 - Parse error. To many content type parameters, cannot add more
        ERR_PARSE_MAX,
    }ERR;


    HTTPSubstring    url_raw;                       ///< RFC 1738, Not URL decoded.  Does not include username or password if specified
    HTTPSubstring    method;                        ///< RFC 1738, URL method
    HTTPSubstring    url;                           ///< RFC 1738, Decoded URL, i.e. route like /msg or / or /request etc.
    HTTPSubstring    username;                      ///< RFC 1738, Not from session but URL before @
    HTTPSubstring    password;                      ///< RFC 1738, Not from session but URL before @
    HTTPSubstring    protocol;                      ///< RFC 1738, protocol like HTTP
    HTTPSubstring    protocol_version;              ///< RFC 1738, protocol version like 1.0, 1.1 etc.
    HTTPSubstring    content;                       ///< Content after headers.  May only contain first string if POST decoded

#if GHTTP_SERVER_ACCEPT_AUTHORIZATION == 1 && GHTTP_SERVER_AUTHORIZATION_REUSE_VARS == 0
    HTTPSubstring    authorization_username;        ///< RFC 1735, basic authorization username

    HTTPSubstring    authorization_password;        ///< RFC 1735, basic authorization password
#endif // GHTTP_SERVER_ACCEPT_AUTHORIZATION == 1

    HTTPAttrValueStringSet<CT> content_type;        ///< Content string set
    HTTPAttrValueStringSet<V> query_variable;       ///< Query string set

#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
    HTTPAttrValueStringSet<V> post_variable;        ///< Post string set
#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
    HTTPRequestFileStringSet<FV> post_file_variable;    ///< Post file string set
#endif
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1

    HTTPAttrValueStringSet<N> header;               ///< Header strings

#if GHTTP_REQUEST_PROCESS_COOKIES == 1
    HTTPRequestCookieJar<CN> cookie;                ///< Cookie strings
#endif

#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
    HTTPSubstring    session;                       ///< Session string (from session cookie if present).  Saves you looking at cookies.  Exists only when GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
#endif

public:
    /**
     * Default constructor, make stable instance.  Defaults NULL strings, port 80 and no error
     */
    HTTPRequest() : port_(80), last_error_(ERR_NONE), eoh_index_(0) {
    }


    /**
     * Get last error
     *
     * \return Error numeric, \ref ERR
     */
    ERR GetLastError() const {
        return last_error_;
    }


    /**
     * Request parse.  Invoke \ref GetLastError post parse, upon successful parse the error will be ERR::ERR_NONE.  A special case for error reporting
     * where ERR::ERR_SEGMENTATION is given when there isn't enough data which includes the case where content length is specified as a header and the 
     * entire request doesn't meet that length.
     *
     * \attention If segmentation used.  Before the start of receiving a new request or at the end of processing a response using the same buffer, caller 
     * should invoke \ref HTTPBuffer::Clear() in order to indicate the start of a new request otherwise data will be assumed segmented and part of the 
     * same request
     *
     * \note Flag parse_content has no affect if GHTTP_REQUEST_PROCESS_POST_VARS isn't defined.  It will be set to false internally if HTTP version < 1.0
     * as only GET requests with no content length are supported
     *
     * \note Using segmentation means ERR::ERR_PARSE_CONTENT_LENGTH_MISMATCH only occurs when there is to much data received and content length header 
     * present for HTTP version >=1.0 otherwise ERR::ERR_SEGMENTATION is given
     *
     * \todo Refactor as statemachine
     *
     * \param[in] request_buffer Request buffer pointer (Maybe NULL terminated)
     * \param[in] length Request buffer length (characters, not including NULL)
     * \param[in] max_length Request buffer maximum length (characters, including NULL)
     * \param[in] parse_content Flag indicating content should also be parsed.  Default true => POST variables are parsed if enabled through macros
     * \param[in] segmentation Flag indicating the request maybe be segmented and a caller is expecting feedback from Parser to decide what to do 
     * with network activity (default true).  Set to false if you expect a single invoke per request and the entire request should exist in buffer.
     * \return Last position parsed within buffer (character index).  Upon error condition where \ref GetLastError indicates failure the index may help
     */
    uint16_t Parse(char* request_buffer, uint16_t length, uint16_t max_length, const bool parse_content=true, const bool segmentation=true) {
        uint16_t e,i=0;

        last_error_ = ERR_NONE;

        if (!segmentation) {
            Clear();
        }

        // First segment?
        if (!GetLength()) {
            eoh_index_ = 0;
            buffer_ = request_buffer;
            max_length_ = max_length;

            // Strings we expect to populate
            method.Null();
            url_raw.Null();
            url.Null();
            username.Null();
            password.Null();
            protocol.Null();
            protocol_version.Null();
            content.Null();

        // Conditionals
#if GHTTP_REQUEST_PROCESS_COOKIES == 1 && GHTTP_SERVER_USE_SESSIONS == 1
            session.Null();
#endif
#if GHTTP_REQUEST_PROCESS_COOKIES == 1
            cookie.count = 0;
#endif
#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
            post_variable.count = 0;
#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
            post_file_variable.count = 0;
#endif
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1

            tmp_.count = 0;
            header.count = 0;
            query_variable.count = 0;
            port_ = 80;                // Default
            content_length_ = 0;    // Default
            content_type.count = 0;
        }
        e = length_;            // Current length before parse
        length_ = length;        // Required for FindTerminator(...)

        if (!eoh_index_ && segmentation) {
            // Make sure EOH present in buffer
            eoh_index_ = FindSubString(0, length, T("\r\n\r\n"), max_length);
            // End of first line
            i = FindSubString(0, length, T("\r\n"), max_length);
            if (eoh_index_>=length) {
                // No EOH.  First line?
                if (url.Get() || i>=length) {
                    // No first line.  Wait for more data
                    last_error_ = ERR_SEGMENTATION;
                }

                i = eoh_index_ = 0;
            }else {
                // Was there no length before parse? i.e. this is first segment and includes everything?  if so we need to parse first line so i=0
                if (!e) {
                    i = 0;
                }else {
                    i+=2; // Skip over EOFL
                }
            }
        }else if (eoh_index_ && eoh_index_<=length) {
            // Start parsing content.  It should start on end of header "\r\n" hence +2 not +4
            i = eoh_index_ + 2;
        }

        if (last_error_ == ERR_NONE && length_) {
            do {
                if (i>=length_) {
                    // More data expected, parse not finished
                    Error(ERR_PARSE_EOF);
                    break;
                }
                // First line parse?
                if (i==0) {
                    if (buffer_[i]<=' ') {
                        // Bad first line header
                        Error(ERR_PARSE_BAD_FIRST_LINE_SOL);
                        break;
                    }

                    // Method
                    e = FindTerminator(i," ");
                    if (e==i || e==length_) {
                        // Error bad method
                        Error(ERR_PARSE_BAD_METHOD);
                        break;
                    }else {
                        method.Set(buffer_, i,e-i);
                    }
                    i = e + 1; // Skip NULL

                    // Username
                    e = FindTerminator(i,T("@ "));
                    if (e==i || e==length_ || (e<length_ && '@'!=buffer_[e])) {
                        // Not specified
                    }else {
                        uint16_t ate = e;

                        // Password
                        e = FindTerminator(i,T(":@"));
                        if (e==i || e==length_ || ate==e || (e<length_ && ':'!=buffer_[e])) {
                            // Not specified
                            username.Set(buffer_, i,ate-i);
                        }else {
                            username.Set(buffer_, i,e-i);
                            password.Set(buffer_, e+1, ate-(1+e));    // Password is +1 as is NULL for username and len n-1 => ate-(1+e)
                        }

                        i = ate + 1; // Skip NULL
                    }

                    // Setup RAW URL.  Find end 'e'.  We only copy and setup if there is room
                    e = FindTerminator(i," ");
                    if ((length_+ 1 + e-i) < max_length_) {
                        // Originally requests were not segmented since any parser has to interaction with lower protocol layers of your network stack
                        // this raw URL cannot be put at length_ because that changes with new segments.
                        // Behaviour now is:
                        // If not segmenting assume length_ entire request so continue to place at end of request otherwise place at end of buffer - string length
                        // and move it to length_ after parsing complete.
                        if (!segmentation) {
                            // Place at end of complete request due to lack of segmentation
                            memcpy(&buffer_[length_+1], &buffer_[i], e-i);
                            buffer_[length_ + 1 + e-i] = '\0';
                            url_raw.Set(buffer_, length_ + 1, e-i);
                        }else {
                            uint16_t i_raw_url = max_length_ - (1 + e-i);

                            // Place at end of buffer complete request due to lack of segmentation
                            memcpy(&buffer_[i_raw_url], &buffer_[i], e-i);
                            buffer_[max_length_-1] = '\0';
                            url_raw.Set(buffer_, i_raw_url, e-i);
                        }
                    }

                    // URL
                    e = FindTerminator(i, T("? "));
                    if (e==i || e==length_) {
                        // Error bad URL
                        Error(ERR_PARSE_BAD_URL);
                        break;
                    }else {
                        char q = buffer_[e];
                        url.Set(buffer_, i, e-i);

                        // Decode it.  A decode will always be <= encoded string
                        URI::Decode(const_cast<char*>(url.Get()));
                        i = e + 1; // Skip NULL

                        // Expect GET parameters?
                        if ('?'==q) {
                            i = BuildStringSetParseVariables(i, query_variable, ERR_PARSE_BAD_GET_PARAM_NAME);
                            if (ERR_NONE != GetLastError()) {
                                break;
                            }
                        }
                    }

                    // Protocol
                    e = FindTerminator(i, "/");
                    if (e==i || e==length_) {
                        // Error bad protocol
                        Error(ERR_PARSE_BAD_PROTOCOL);
                        break;
                    }else {
                        protocol.Set(buffer_, i,e-i);
                    }
                    i = e + 1; // Skip NULL

                    // Protocol version
                    e = FindTerminator(i, "\r");
                    if (e==i || e==length_) {
                        // Error bad protocol version
                        Error(ERR_PARSE_BAD_PROTOCOL_VERSION);
                        break;
                    }else {
                        protocol_version.Set(buffer_, i,e-i);
                    }
                    i = e + 1; // Skip NULL
                    
                    // EOL?
                    if ('\n' != buffer_[i]) {
                        // Error bad line 0
                        Error(ERR_PARSE_BAD_FIRST_LINE_NO_EOL);
                        break;
                    }
                    i++;

                    // Do we parse content?
                    // Source: https://www.w3.org/Protocols/HTTP/AsImplemented.html
                    // HTTP/0.9 supported GET with no content (body)
                    if (protocol == T("http") && protocol_version == T("0.9")) {
                        if (method != T("get")) {
                            // HTTP/0.9 only supports GET
                            Error(ERR_PARSE_BAD_METHOD);
                        }
                        if (i != length_) {
                            Error(ERR_PARSE_UNEXPECTED_DATA);
                        }
                        break; // Done.  v0.9 has no content or headers
                    }else {
                        // We've parsed first line but did do we have the headers to go on and parse?
                        if (!eoh_index_ && segmentation) {
                            // No
                            Error(ERR_SEGMENTATION);
                            break;
                        }
                    }

                }else { // else (l!=0)
                    // Process header strings, i.e. "name: value" pairs

                    // Start of content? (is header string just CR+LF)
                    if (buffer_[i]=='\r' && (i+1)<length_ && buffer_[i+1]=='\n') {
                        if ((i+2)<=length_) {
                            uint16_t al;

                            // Content exists
                            i+=2;
                            al = length_ - i;
                            content.Set(buffer_, i, al);
                            if (content_length_ != al) {
                                // Error content length differ
#if defined(_MSC_VER)
                                printf("set %d, calc length = %d\n", content_length_, (length_ - i));
#endif
                                // If segmentation, are we still waiting for content?
                                if (segmentation &&  al < content_length_) {
                                    Error(ERR_SEGMENTATION);
                                }else {
                                    Error(ERR_PARSE_CONTENT_LENGTH_MISMATCH);
                                }
                            }else if (content_length_>=2) {
#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
                                if (parse_content) {
                                    /* RFC 7578 & RFC 1867
                                     * These are processed if
                                     * header Content-Type = multipart/form-data or application/x-www-form-urlencoded
                                     * method = "post"
                                     */
                                    if (!method.StrNCmp(T("post")) && content_type.count>0) {
                                        if (content_type.Find(T("application/x-www-form-urlencoded"))>=0) {
#if GHTTP_REQUEST_BAD_POST_DATA_IGNORE == 1
                                            i = BuildStringSetParseVariables(i, post_variable);
#else
                                            i = BuildStringSetParseVariables(i, post_variable, ERR_PARSE_BAD_POST_PARAM_NAME);
                                            if (ERR_NONE != GetLastError()) {
                                                break;
                                            }
#endif
                                        }else if (content_type.Find(T("multipart/form-data"))>=0) {

                                            // Bounary string in content type must exist
                                            int16_t bi = content_type.Find(T("boundary"));
                                            if (bi>=0) {
                                                i = ProcessContent(i, content_type.At(static_cast<uint8_t>(bi))->value.Get());
                                                if (ERR_NONE != GetLastError()) {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                } // if (parse_content)
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1
                            } // else if (content_length_>=2)
                        } // if ((i+2)<length_)

                        // We're done
                        break;
                    } // if (buffer_[i]=='\r' && (i+1)<length_ && buffer_[i+1]=='\n')

                    if (buffer_[i]<=' ') {
                        // Bad header string, expected SOL
                        Error(ERR_PARSE_BAD_HEADER_LINE_SOL);
                        break;
                    }

                    // Attribute/name
                    e = FindTerminator(i, ":");
                    if (e==i || e==length_) {
                        // Error bad header string attribute/name
                        Error(ERR_PARSE_BAD_HEADER_LINE_NAME);
                        break;
                    }else {
                        if (header.count == N) {
                            // Error header string limit reached
                            Error(ERR_PARSE_HEADER_LIMIT);
                            break;
                        }

                        header.At(header.count)->Set(buffer_, i, e-i);
                    }

                    // Find start of value string
                    i = FindTerminator(e + 1/* Skip NULL */, T(": "), true);

                    // Specific string handling
                    if (port_==80 && !header.At(header.count)->StrNCmp(T("host"))) {
                        // Found host header string, deal with here

                        // Value
                        e = FindTerminator(i, ":\r");
                        if (e==i || e==length_) {
                            // Error bad header string value
                            Error(ERR_PARSE_BAD_HEADER_LINE_VALUE);
                            break;
                        }else {
                            header.At(header.count)->value.Set(buffer_, i, e-i);
                        }
                        i = e + 1; // Skip NULL

                        // EOL?
                        if ('\n' != buffer_[i]) {
                            // No so we can assume port specified by host

                            // Port
                            e = FindTerminator(i, "\r");
                            if (e==i || e==length_) {
                                // Error bad header string value
                                Error(ERR_PARSE_BAD_HEADER_LINE_VALUE);
                                break;
                            }else {
                                // We can use content string because it shouldnt be setup yet
                                content.Set(buffer_, i, e-i);

                                port_ = atoi(&content);

#if defined(HTTPSERVER_HOST_HEADER_INCLUDE_PORT)
                                // Re-install colon so host includes port again
                                buffer_[i] = ':';
#endif
                                i = e + 1; // Skip NULL.  Maybe 2?
                            }
                        }

#if GHTTP_SERVER_ACCEPT_AUTHORIZATION == 1
                    }else if (!header.At(header.count)->StrNCmp(T("authorization"))) { // Authorization
                        // RFC 1735, authorization

                        // Value
                        e = FindTerminator(i, "\r");
                        if (e==i || e==length_) {
                            // Error bad header string value
                            Error(ERR_PARSE_BAD_HEADER_LINE_VALUE);
                            break;
                        }else {
                            header.At(header.count)->value.Set(buffer_, i, e-i);
                        }
                        i = e + 1; // Skip NULL

                        // Is basic authorization?
                        if (!StringHelper::StrNCmp(header.At(header.count)->value.Get(), T("basic "), 6, false)) {
                            header.At(header.count)->value+=6;    // Move over basic so value pointers to username/password data pair

                            // Base64 decode
                            char *b64_pstr = const_cast<char*>(header.At(header.count)->value.Get());
                            MIMEBase64::Decode(b64_pstr, b64_pstr, strlen(b64_pstr), strlen(b64_pstr));

                            // Find password start, a colon comes before it
                            uint16_t psi = StringHelper::FindTerminator(0, b64_pstr, ":");
                            if (psi>0) {
                                b64_pstr[psi] = '\0'; // NULL username

                                // Setup username and password.  We could re-use the username and password from URI (RFC 1738) if short of memory
#if GHTTP_SERVER_AUTHORIZATION_REUSE_VARS == 1
                                username.Set(b64_pstr);
                                password.Set(&b64_pstr[psi+1]);
#else // GHTTP_SERVER_AUTHORIZATION_REUSE_VARS != 1
                                authorization_username.Set(b64_pstr);
                                authorization_password.Set(&b64_pstr[psi+1]);
#endif
                                // Type basic and we appear to have decoded successfull so drop this header otherwise leave as a header string application
                                header.count--;
                            }
                        }
#endif // GHTTP_SERVER_ACCEPT_AUTHORIZATION == 1
                    }else if (!header.At(header.count)->StrNCmp(T("content-type"))) { // Content type

                        // Process
                        i = BuildStringSet(i, content_type, ERR_PARSE_BAD_CTYPE_NAME);
                        if (ERR_NONE != GetLastError()) {
                            break;
                        }
                        header.count--;    // Drop this header as we've parsed into own list

#if GHTTP_REQUEST_PROCESS_COOKIES == 1
                    }else if (!header.At(header.count)->StrNCmp(T("cookie"))) { // Cookies?

                        // Process
                        i = BuildStringSet(i, cookie, ERR_PARSE_BAD_COOKIE_NAME);
                        if (ERR_NONE != GetLastError()) {
                            break;
                        }else if (cookie.count) { // No error and we have cookies
#if GHTTP_SERVER_USE_SESSIONS == 1 && defined(GHTTP_SERVER_STRING_SESSION_COOKIE)
                            // Look for session cookie
                            int16_t ci = cookie.Find(GHTTP_SERVER_STRING_SESSION_COOKIE);

                            // Session cookie exists so set easy access session string
                            if (ci >= 0) {
                                session.Set(const_cast<char*>(cookie.At(static_cast<uint8_t>(ci))->value.Get()));
                            }
#endif // GHTTP_SERVER_USE_SESSIONS == 1 && defined(GHTTP_SERVER_STRING_SESSION_COOKIE)
                        }
                        header.count--;    // Drop this header as we've parsed into own list
#endif // GHTTP_REQUEST_PROCESS_COOKIES == 1

                    }else {
                        // Default behaviour for any property value that isn't expected to include other information

                        // Value
                        e = FindTerminator(i,"\r");
                        if (e==i || e==length_) {
                            // Error bad header string value
                            Error(ERR_PARSE_BAD_HEADER_LINE_VALUE);
                            break;
                        }else {
                            header.At(header.count)->value.Set(buffer_, i,e-i);
                        }
                        i = e + 1; // Skip NULL

                        // Is integer content length 0 and is parameter content length, if yes then convert value to integer and populate
                        if (!content_length_ && !header.At(header.count)->StrNCmp(T("content-length"))) {
                            content_length_ = atoi(&header.At(header.count)->value); // TODO stringhelper::uint
                        }
                    }

                    // EOL?
                    if ('\n' != buffer_[i]) {
                        // Error bad line 0
                        Error(ERR_PARSE_BAD_HEADER_LINE_NO_EOL);
                        break;
                    }
                    i++;

                    header.count++;
                } // else (l!=0)
            }while(1);
        } // if (last_error_ == ERR_NONE && length_)

        // Parse complete with no error and raw url setup with segmentation then we need to move string
        if (!last_error_ && url_raw.Get() && segmentation) {
            // Move raw url from end of buffer to end of request so response can use end of buffer without worrying about overwriting raw url if offset correctly setup when building
            memcpy(&buffer_[length_+1], url_raw.Get(), 1+strlen(url_raw.Get()));
            url_raw.Set(&buffer_[length_+1]);
        }

        return i;
    } // Parse(...)


    /**
     * Get content length
     *
     * \note Only valid if \ref Parse invoked prior
     *
     * \return Characters/Octets
     */
    uint16_t GetContentLength() const {
        return content_length_;
    }

protected:
    /**
     * Set error to be queried later by \ref GetLastError
     *
     * \param[in] e Error from \ref ERR
     */
    void Error(const ERR e) {
        last_error_ = e;
    }


#if GHTTP_REQUEST_PROCESS_POST_VARS == 1
    /**
     * Build string set for post variables from content when type "multipart/form-data"
     *
     * \param[in] i Start index of content into buffer
     * \param[in] boundary_str Boundary string taken from content type header.  This is data only not entire header
     * \return Last position processed within buffer (character index).  Upon error condition where \ref GetLastError indicates failure the index may help
     */
    uint16_t ProcessContent(uint16_t i, const char* boundary_str) {
        uint8_t bsl = strlen(boundary_str);
        uint16_t e, ls = 0, o, t, j;

        do {
            e = FindSubString(i, length_, boundary_str, bsl);
            if (e>2 && e < length_/* && o*/) {
                if ('-' == buffer_[e-1] && '-' == buffer_[e-2]) {
                    e-=2;
                }else {
                    break;
                }

                if (ls) {
                    /* Process field data between characrters last start (ls) and end (e) character indexes
                     * Fields look like:
                     * Content-Disposition: form-data; name="user"
                     *
                     * Files look like: 
                     * Content-Disposition: form-data; name="thefile"; filename=""
                     * Content-Type: application/octet-stream
                     */
                    // Look for first line \r\n
                    j = FindEOL(ls, e);
                    if (j < (e+1)) {
                        // form-data; name="thefile"; filename
                        if (FindSubString(ls, e, T("form-data")) < e) {
                            if (FindSubString(ls, e, T("name")) < e) {
                                // Is it a file or field?
                                if (FindSubString(ls, e, T("filename")) >= e) {
                                    t = FindSubString(ls, e, T("name"));
                                    o = FindTerminator(t, "\"", false, j) +1;
                                    if (o<j) {
                                        t = FindTerminator(o+1, "\"", false, j);
                                        if (t<j) {
#if GHTTP_REQUEST_BAD_POST_DATA_IGNORE == 1
                                        if (V == post_variable.count) {
                                            post_variable.count--;    // Over-write
                                        }
#else // GHTTP_REQUEST_BAD_POST_DATA_IGNORE != 1
                                        if (V == post_variable.count) {
                                            Error(ERR_PARSE_BAD_POST_PARAM_TO_MANY);
                                            return i;
                                        }
#endif // GHTTP_REQUEST_BAD_POST_DATA_IGNORE != 1
                                            // Attr name = "abcd", o='a' and t='"'
                                            post_variable.At(post_variable.count)->Set(buffer_, o, t-o);

                                            // Find value which will be after double \r\n\r\n within ls - e
                                            do {
                                                o = FindEOL(o, e);
                                                
                                                if (o<e && (o+3)<e && buffer_[o]=='\n' && buffer_[o+1]=='\r' && buffer_[o+2]=='\n') {
                                                    o+=3; // +CRLFCR
                                                    // value[x] where o>= x <= e-2 (length => e-(o+6))
                                                    post_variable.At(post_variable.count++)->value.Set(buffer_, o, e-(o+2));
                                                    break;
                                                }else if (o==e) {
                                                    break;
                                                }
                                                o++;
                                            }while(1);
                                        }
                                    }
                                }else { // if (!t) => filename exists
                                    // File

#if GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
#if GHTTP_REQUEST_BAD_POST_DATA_IGNORE == 1
                                    if (FV == post_file_variable.count) {
                                        post_file_variable.count--;    // Over-write
                                    }
#else // GHTTP_REQUEST_BAD_POST_DATA_IGNORE != 1
                                    if (FV == post_file_variable.count) {
                                        Error(ERR_PARSE_BAD_POST_FILE_PARAM_TO_MANY);
                                        return i;
                                    }
#endif // GHTTP_REQUEST_BAD_POST_DATA_IGNORE != 1

                                    // We look through each post header until we reach post data.  Along the way we selectively populate file objects
                                    do {
                                        j = FindEOL(ls, e);
                                        if (j >= e) {
                                            // Error no more, oh
                                            break;
                                        }
                                        if (j == (1+ls)) {
                                            // End of post headers, start of data
                                            break;
                                        }

                                        // Find header "name: "
                                        t = FindTerminator(ls, ": \r", false, j);
                                        if (t>=j || (t<j && ':'!=buffer_[t])) {
                                            // Skip to end of line.  Shouldn't really get here??
                                            ls = j + 1;
                                            continue;
                                        }
                                        buffer_[t] = '\0';
                                        tmp_.count = 0;
                                        t = FindTerminator(t+1, " ", true, j);

                                        i = BuildStringSet(t, tmp_, 0, j);
                                        if (tmp_.count) {
                                            if (0 == StringHelper::StrNCmp(&buffer_[ls], T("content-type"), 12, false)) {
                                                HTTPSubstring* vs = tmp_.At(0)->ThisPtr();

                                                vs->TrimQuotes();
                                                post_file_variable.At(post_file_variable.count)->mimetype.Set(const_cast<char*>(vs->Get()));
                                            }else if (0 == StringHelper::StrNCmp(&buffer_[ls], T("content-disposition"), 19, false)) {
                                                int16_t si;
                                                
                                                if ((si = tmp_.Find(T("name")))>=0) {
                                                    HTTPSubstring* vs = tmp_.At(static_cast<uint8_t>(si))->value.ThisPtr();
                                                    vs->TrimQuotes();
                                                    post_file_variable.At(post_file_variable.count)->Set(const_cast<char*>(vs->Get()));

                                                }
                                                if ((si = tmp_.Find(T("filename")))>=0) {
                                                    HTTPSubstring* vs = tmp_.At(static_cast<uint8_t>(si))->value.ThisPtr();
                                                    vs->TrimQuotes();
                                                    post_file_variable.At(post_file_variable.count)->filename.Set(const_cast<char*>(vs->Get()));
                                                }
                                            }
                                        }
                                        ls = i+1;
                                    }while(1);
                                    // CR+LF<data>CR+LF (so -4) as 'e' points to boundary string start which is terminator for this file post and ls the last CR+LF before data
                                    post_file_variable.At(post_file_variable.count)->filesize = e - (ls + 4);

                                    // Name is really the only important field
                                    if (post_file_variable.At(post_file_variable.count)->Get() && strlen(post_file_variable.At(post_file_variable.count)->Get())) {
                                        // Set file data and we're done
                                        buffer_[e-2] = '\0';
                                        post_file_variable.At(post_file_variable.count)->value.Set(&buffer_[ls+2]);
                                        post_file_variable.count++;
                                    }
#endif // GHTTP_REQUEST_PROCESS_POST_FILE_VARS == 1
                                } // if (FindSubString(ls, e, "filename") >= e) => filename exists
                            } // if (FindSubString(ls, e, "name") < e) => name exists
                        } // if (FindSubString(ls, e, "form-data") < e) => form-data exists
                    } // if (j < (e+1)) => CR+LF end of header line
                } // if (ls) => 2nd or more boundary
                ls = e + bsl + 4; // '-' '-'  + 'CR' 'LF'

                i = ls;
            }else {
                i = e + 1;
                break;
            }
        }while(1);

        return i;
    } // ProcessContent(...)
#endif // GHTTP_REQUEST_PROCESS_POST_VARS == 1


    /**
     * Build string set from a header where format expected "Header: param1[=value]; param2[=value]; param3[=value]..." and result string set is header data
     *
     * \note set.count isn't zeroed so can accumulate with multiple invokes of this function, useful when making multiple headers accessible from the same set
     *
     * \tparam SSN String set maximum
     * \param[in] i Start index of header string.  If skip_header_name false it should after colon otherwise will internally find and skip
     * \param[in] set Reference to string set to build/append to
     * \param[in] base Error code base for reporting correctly the errors found for different sets, default 0 (errors not reported via \ref GetLastError)
     * \param[in] max_i Maximum index where set building should terminate, default 0 => max_i = HTTPBuffer::GetLength
     * \param[in] skip_header_name Flag indicating when true that index i points to entire header and that the name should be skipped, default false
     * \return Last position processed within buffer (character index).  Upon error condition where \ref GetLastError indicates failure the index may help
     */
    template<uint8_t SSN>
    uint16_t BuildStringSet(uint16_t i, HTTPAttrValueStringSet<SSN> &set, uint8_t base=0, uint32_t max_i=0, const bool skip_header_name=false) {
        uint16_t e, el, eat;

        if (!max_i) {
            max_i = length_;
        }

        if (skip_header_name) {
            // Find end of header name
            e = FindTerminator(i, T(": \r"), false, max_i);
            if (e<max_i && ':' == buffer_[e]) {
                // Skip over search string characters to start of data
                i = FindTerminator(e, ": ", true, max_i);
            }
        }

        // Work until EOL
        el = FindEOL(i, max_i);
        do {
            // EOL?  Theoretically this should finish on '\r'
            if (i >= el) {
                // We're done
                break;
            }

            if (set.count == SSN) {
                if (base) {
                    // Error to many parameters
                    Error(static_cast<ERR>(base+2));
                }
                break;
            }

            // Find end of current attribute+value string
            eat = FindTerminator(i, T(";\r"), false, max_i);
            if (eat==i || eat>=max_i) {
                // Error bad name, well actually its a completely bad attribute
                if (base) {
                    Error(static_cast<ERR>(base+0));
                }
                break;
            }
            // So between i - eat we have something like "multipart/form-data;" or "boundary=----WebKitFormBoundarycbLpHx9jR5uFEQxZ" etc.

            // For attrib/name
            e = FindTerminator(i,"=", false, eat);
            if (e==i) {
                // Error bad name, can't have = and not move i
                if (base) {
                    Error(static_cast<ERR>(base+0));
                }
                break;
            }
            if (e<eat) {
                // name=value exists

                // Set name
                i = FindTerminator(i," ", true, eat); // Skip over any white space
                if (i>eat) {
                    // Error bad name, can't have = and not move i
                    if (base) {
                        Error(static_cast<ERR>(base+0));
                    }
                    break;
                }
                set.At(set.count)->Set(buffer_, i, e-i);
                i = e + 1; // Skip NULL


                // For value
                i = FindTerminator(i," ", true, eat); // Skip over any white space
                if (i>eat) {
                    // Error bad value
                    if (base) {
                        Error(static_cast<ERR>(base+1));
                    }
                    break;
                }
                set.At(set.count)->value.Set(buffer_, i, eat-i);
                set.count++;
                i = eat + 1; // Skip NULL
            }else {
                // Set name only, no =
                i = FindTerminator(i," ", true, eat); // Skip over any white space
                if (i>eat) {
                    // Error bad name, can't have = and not move i
                    if (base) {
                        Error(static_cast<ERR>(base+0));
                    }
                    break;
                }
                set.At(set.count)->Set(buffer_, i, eat-i);
                set.At(set.count)->value.Set(buffer_, eat, 0);
                set.count++;
                i = eat + 1; // Skip NULL
            }

        }while(1);

        return i;
    } // BuildStringSet(...)


    /**
     * Build string set from GET/POST variables, latter when content type is "application/x-www-form-urlencoded".  Format of URL encoded data is 
     * paramName1=paramValue1[&paramName2[=paramValue2]...]  (= or =<value> is optional)
     *
     * \note set.count isn't zeroed so can accumulate with multiple invokes
     *
     * \tparam SSN String set maximum
     * \param[in] i Start index of header string.  If skip_header_name false it should after colon otherwise will internally find and skip
     * \param[in] v Reference to string set to build/append to
     * \param[in] base Error code base for reporting correctly the errors found for different sets, default 0 (errors not reported via \ref GetLastError)
     * \return Last position processed within buffer (character index).  Upon error condition where \ref GetLastError indicates failure the index may help
     */
    template<uint8_t SSN>
    uint16_t BuildStringSetParseVariables(uint16_t i, HTTPAttrValueStringSet<SSN> &v, uint8_t base=0) {
        uint8_t d = 0;
        uint16_t e;

        do {
            e = FindTerminator(i, T("& \r"));
            if (e<length_ && '&'!=buffer_[e]) {
                d = 1;
            }
            if (!d && (e==i || e>length_)) {
                if (base) {
                    // Error bad parameter name
                    Error(static_cast<ERR>(base+0));
                }
                break;
            }
            if (v.count == SSN) {
                if (base) {
                    // Error to many parameters
                    Error(static_cast<ERR>(base+2));
                }
                break;
            }
            v.At(v.count)->Set(buffer_, i, e-i);

            i = FindTerminator(i, T("= &\r"), false, e);
            if (i>e || i>length_) {
                if (base) {
                    // Error bad parameter value
                    Error(static_cast<ERR>(base+1));
                    break;
                }
            }
            if ('=' == buffer_[i]) {
                buffer_[i] = '\0'; // Write over equals to seperate parameter/value
                v.At(v.count)->value.Set(buffer_, i+1, e-(i+1));
            }else {
                // There is no value
                buffer_[i] = '\0'; // Write over whatever

                v.At(v.count)->value.Set(buffer_, i, 0);    // No value
            }

            // Decode parameter + value
            URI::Decode(const_cast<char*>(v.At(v.count)->Get()));
            URI::Decode(const_cast<char*>(v.At(v.count)->value.Get()));

            v.count++;

            i = e + 1; // Skip NULL

            // No more parameters
            if (d || i>=length_) {
                break;
            }
        }while(1);

        return i;
    } // BuildStringSetParseVariables(...)

protected:
    uint16_t    port_;                              ///< Received port number
    uint16_t    content_length_;                    ///< Content length (Bytes/ASCII characters)
    uint16_t    eoh_index_;                         ///< End of headers index, setup by parser re-used in segmented parse operations
    ERR            last_error_;                     ///< Last error
    HTTPAttrValueStringSet<6> tmp_;                 ///< Temporary use string set
}; // class HTTPRequest

}; // namespace ghttp

#endif // GHTTP_HTTPREQUEST_H
