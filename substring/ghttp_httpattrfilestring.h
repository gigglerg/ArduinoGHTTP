/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP substring with specifics for a HTTP file
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPATTRFILESTRING_H
#define GHTTP_HTTPATTRFILESTRING_H

namespace ghttp {

/**
 * A class representing attribute/value field for HTTP file input types processed during post request handling.  Accessors include filename, mimetype and filesize
 */
class HTTPAttrFileString : public HTTPAttrValueString {
public:
    HTTPSubstring   filename;                       ///< Filename of data upload
    HTTPSubstring   mimetype;                       ///< Data content mime type for upload
    uint16_t        filesize;                       ///< Filesize (Bytes) of data upload
}; // class HTTPAttrFileString

}; // namespace ghttp

#endif // GHTTP_HTTPATTRFILESTRING_H
