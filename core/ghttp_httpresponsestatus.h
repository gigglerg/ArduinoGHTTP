/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP response status
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPRESPONSESTATUS
#define GHTTP_HTTPRESPONSESTATUS

namespace ghttp {

#if defined(ARDUINO_ARCH_AVR)

/**
 * Helpe strcat for this module.  AVR string y is flash based
 *
 * \param[in] x String pointer (SRAM based)
 * \param[in] y String pointer (flash based)
 * \return x String pointer
 */
#define HRS_STRCAT(x,y)            PGM_STRCAT(x,y)

#else

/**
 * Helper strcat for this module
 *
 * \param[in] x String pointer
 * \param[in] y String pointer
 * \return x String pointer
 */
#define HRS_STRCAT(x,y)            strcat(x,y)

#endif // !defined(ARDUINO_ARCH_AVR)


/**
 * A class covering HTTP/1.x status codes, implementing in part RFC 7231
 *
 * \note Write your own replacement class via protected inheritance if you intend to add new response status codes or other functionality
 */
class HTTPResponseStatus {
public:
    /**
     * Enum of possible status codes
     */
    typedef enum {
        STATUS_CONTINUE                     = 100,        ///< No content
        STATUS_OK                           = 200,
        STATUS_CREATED                      = 201,
        STATUS_ACCEPTED                     = 202,
        STATUS_NO_CONTENT                   = 204,        ///< No content
        STATUS_RESET_CONTENT                = 205,
        STATUS_NOT_MODIFIED                 = 304,        ///< No content
        STATUS_MOVED_PERMANENTLY            = 301,        ///< Includes Location header
        STATUS_FOUND                        = 302,        ///< Includes Location header
        STATUS_SEE_OTHER                    = 303,        ///< Includes Location header
        STATUS_TEMPORARY_REDIRECT           = 307,        ///< Includes Location header
        STATUS_PERMANENT_REDIRECT           = 308,        ///< Includes Location header
        STATUS_BAD_REQUEST                  = 400,        ///< Request not repeated by client
        STATUS_UNAUTHORIZED                 = 401,        ///< Response must include WWW-Authenticate challenge
        STATUS_FORBIDDEN                    = 403,        ///< Should include content with description otherwise use STATUS_NOT_FOUND
        STATUS_NOT_FOUND                    = 404,
        STATUS_LENGTH_REQUIRED              = 411,
        STATUS_PAYLOAD_TOO_LARGE            = 413,
        STATUS_URI_TOO_LONG                 = 414,
        STATUS_IM_A_TEAPOT                  = 418,
        STATUS_REQUEST_TIMEOUT              = 408,
        STATUS_CONFLICT                     = 409,        ///< If server accepted request there would be some kind of resource conflict
        STATUS_TOO_EARLY                    = 425,
        STATUS_REQUEST_HEADER_TOO_LARGE     = 431,
        STATUS_INTERNAL_SERVER_ERROR        = 500,        ///< Should include content with description
        STATUS_NOT_IMPLEMENTED              = 501,        ///< Should include content with description
        STATUS_SERVICE_UNAVAILABLE          = 503,        ///< Should include content with description
        STATUS_HTTP_VERSION_NOT_SUPPORTED   = 505,        ///< Should include content with description
        STATUS_INSUFFICIENT_STORAGE         = 507,        ///< Should include content with description
        STATUS_LOOP_DETECTED                = 508         ///< Should include content with description
    }STATUS;


public:    
    /**
     * Default constructor, make stable instance with status OK (200)
     */
    HTTPResponseStatus() : status_(STATUS_OK) { }


    /**
     * Set status code
     *
     * \param[in] status Status code numeric
     */
    void Set(const uint16_t status) {
        status_ = status;
    }

    
    /**
     * Get status code
     *
     * \return Status code numeric
     */
    uint16_t Get() const {
        return status_;
    }


    /**
     * Get length of status code (if) converted into ASCII string
     *
     * \return String length, characters (excluding NULL)
     */
    uint8_t GetStringLength() const {
        uint8_t length;

        HTTPResponseStatus::GetStatusString(status_, NULL, &length);

        return length;
    }


    /**
     * Get string of status code converted into ASCII string
     *
     * \param[out] write_buffer String pointer with enough space to store status code converted into string
     * \return String pointer (write_buffer)
     */
    char* GetString(char *write_buffer) const {
        return HTTPResponseStatus::GetStatusString(status_, write_buffer);
    }


    /**
     * Get length of given status code OR get string of given status code
     *
     * \param[in] status Status code
     * \param[out] write_buffer String pointer.  Maybe NULL if length only required
     * \param[out] length ASCII string length (exluding NULL).  Default is NULL (length not required)
     * \return String pointer (write_buffer)
     */
    static char* GetStatusString(uint16_t status, char* write_buffer, uint8_t *length=NULL) {
        if (length) {
            *length=4;    // Status 3 decimal digits + space
        }
        if (write_buffer) {
            sprintf(write_buffer, "%u ", status);
        }
        switch(status) {
            case STATUS_CONTINUE :
                if (write_buffer) {
                    //                          12345678
                    HRS_STRCAT(write_buffer, T("Continue"));
                }
                if (length) {
                    *length+=0x08;
                }
                break;
            case STATUS_OK :
                if (write_buffer) {
                    //                          12
                    HRS_STRCAT(write_buffer, T("OK"));
                }
                if (length) {
                    *length+=0x02;
                }
                break;
            case STATUS_CREATED :
                if (write_buffer) {
                    //                          1234567
                    HRS_STRCAT(write_buffer, T("Created"));
                }
                if (length) {
                    *length+=0x07;
                }
                break;
            case STATUS_ACCEPTED :
                if (write_buffer) {
                    //                          12345678
                    HRS_STRCAT(write_buffer, T("Accepted"));
                }
                if (length) {
                    *length+=0x08;
                }
                break;
            case STATUS_NO_CONTENT :
                if (write_buffer) {
                    //                          123456789a
                    HRS_STRCAT(write_buffer, T("No Content"));
                }
                if (length) {
                    *length+=0x0a;
                }
                break;
            case STATUS_RESET_CONTENT :
                if (write_buffer) {
                    //                          123456789abcd
                    HRS_STRCAT(write_buffer, T("Reset Content"));
                }
                if (length) {
                    *length+=0x0d;
                }
                break;
            case STATUS_FOUND :
                if (write_buffer) {
                    //                          12345
                    HRS_STRCAT(write_buffer, T("Found"));
                }
                if (length) {
                    *length+=0x05;
                }
                break;
            case STATUS_SEE_OTHER :
                if (write_buffer) {
                    //                          123456789
                    HRS_STRCAT(write_buffer, T("See Other"));
                }
                if (length) {
                    *length+=0x09;
                }
                break;
            case STATUS_NOT_MODIFIED :
                if (write_buffer) {
                    //                          123456789abc
                    HRS_STRCAT(write_buffer, T("Not Modified"));
                }
                if (length) {
                    *length+=0x0c;
                }
                break;
            case STATUS_MOVED_PERMANENTLY :
                if (write_buffer) {
                    //                          123456789abcdef01
                    HRS_STRCAT(write_buffer, T("Moved Permanently"));
                }
                if (length) {
                    *length+=0x11;
                }
                break;
            case STATUS_TEMPORARY_REDIRECT :
                if (write_buffer) {
                    //                          123456789abcdef012
                    HRS_STRCAT(write_buffer, T("Temporary Redirect"));
                }
                if (length) {
                    *length+=0x12;
                }
                break;
            case STATUS_PERMANENT_REDIRECT :
                if (write_buffer) {
                    //                          123456789abcdef012
                    HRS_STRCAT(write_buffer, T("Permanent Redirect"));
                }
                if (length) {
                    *length+=0x12;
                }
                break;
            case STATUS_BAD_REQUEST :
                if (write_buffer) {
                    //                          123456789ab
                    HRS_STRCAT(write_buffer, T("Bad Request"));
                }
                if (length) {
                    *length+=0x0b;
                }
                break;
            case STATUS_UNAUTHORIZED :
                if (write_buffer) {
                    //                          123456789abc
                    HRS_STRCAT(write_buffer, T("Unauthorized"));
                }
                if (length) {
                    *length+=0x0c;
                }
                break;
            case STATUS_FORBIDDEN :
                if (write_buffer) {
                    //                          123456789
                    HRS_STRCAT(write_buffer, T("Forbidden"));
                }
                if (length) {
                    *length+=0x09;
                }
                break;
            case STATUS_NOT_FOUND :
                if (write_buffer) {
                    //                          123456789
                    HRS_STRCAT(write_buffer, T("Not Found"));
                }
                if (length) {
                    *length+=0x09;
                }
                break;
            case STATUS_REQUEST_TIMEOUT :
                if (write_buffer) {
                    //                          123456789abcdef
                    HRS_STRCAT(write_buffer, T("Request Timeout"));
                }
                if (length) {
                    *length+=0x0f;
                }
                break;
            case STATUS_CONFLICT :
                if (write_buffer) {
                    //                          12345678
                    HRS_STRCAT(write_buffer, T("Conflict"));
                }
                if (length) {
                    *length+=0x08;
                }
                break;
            case STATUS_LENGTH_REQUIRED :
                if (write_buffer) {
                    //                          123456789abcdef
                    HRS_STRCAT(write_buffer, T("Length Required"));
                }
                if (length) {
                    *length+=0x0f;
                }
                break;
            case STATUS_PAYLOAD_TOO_LARGE :
                if (write_buffer) {
                    //                          123456789abcdef01
                    HRS_STRCAT(write_buffer, T("Payload Too Large"));
                }
                if (length) {
                    *length+=0x11;
                }
                break;
            case STATUS_URI_TOO_LONG :
                if (write_buffer) {
                    //                          123456789abc
                    HRS_STRCAT(write_buffer, T("URI Too Long"));
                }
                if (length) {
                    *length+=0x0c;
                }
                break;
            case STATUS_IM_A_TEAPOT :
                if (write_buffer) {
                    //                          123456789abc
                    HRS_STRCAT(write_buffer, T("I'm a teapot"));
                }
                if (length) {
                    *length+=0x0c;
                }
                break;
            case STATUS_TOO_EARLY :
                if (write_buffer) {
                    //                         123456789
                    HRS_STRCAT(write_buffer, ("Too Early"));
                }
                if (length) {
                    *length+=0x09;
                }
                break;
            case STATUS_REQUEST_HEADER_TOO_LARGE :
                if (write_buffer) {
                    //                          123456789abcdef0123456789abcdef
                    HRS_STRCAT(write_buffer, T("Request Header Fields Too Large"));
                }
                if (length) {
                    *length+=0x1f;
                }
                break;
            case STATUS_INTERNAL_SERVER_ERROR :
                if (write_buffer) {
                    //                          123456789abcdef012345
                    HRS_STRCAT(write_buffer, T("Internal Server Error"));
                }
                if (length) {
                    *length+=0x15;
                }
                break;
            case STATUS_NOT_IMPLEMENTED :
                if (write_buffer) {
                    //                          123456789abcdef
                    HRS_STRCAT(write_buffer, T("Not Implemented"));
                }
                if (length) {
                    *length+=0x0f;
                }
                break;
            case STATUS_SERVICE_UNAVAILABLE :
                if (write_buffer) {
                    //                          123456789abcdef0123
                    HRS_STRCAT(write_buffer, T("Service Unavailable"));
                }
                if (length) {
                    *length+=0x13;
                }
                break;
            case STATUS_HTTP_VERSION_NOT_SUPPORTED :
                if (write_buffer) {
                    //                          123456789abcdef0123456789a
                    HRS_STRCAT(write_buffer, T("HTTP Version Not Supported"));
                }
                if (length) {
                    *length+=0x1a;
                }
                break;
            case STATUS_INSUFFICIENT_STORAGE :
                if (write_buffer) {
                    //                          123456789abcdef01234
                    HRS_STRCAT(write_buffer, T("Insufficient Storage"));
                }
                if (length) {
                    *length+=0x14;
                }
                break;
            case STATUS_LOOP_DETECTED :
                if (write_buffer) {
                    //                          123456789abcd
                    HRS_STRCAT(write_buffer, T("Loop Detected"));
                }
                if (length) {
                    *length+=0xd;
                }
                break;
        } // switch(status)
        
        return write_buffer;
    } // GetStatusString(...)
    
protected:
    uint16_t status_;            ///< Status code
}; // class HTTPResponseStatus

}; // namespace ghttp

#endif // GHTTP_HTTPRESPONSESTATUS
