/**
 * \file
 * Embedded HTTP request/response handling framework - HTTP request string set cookie jar
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPREQUESTCOOKIEJAR_H
#define GHTTP_HTTPREQUESTCOOKIEJAR_H

namespace ghttp {

/**
 * A container class representing a cookie jar, use during request processing
 *
 * \tparam CN Array size
 */
template<uint8_t CN>
class HTTPRequestCookieJar : public HTTPAttrValueStringSet<CN> { };

}; // namespace ghttp

#endif // GHTTP_HTTPREQUESTCOOKIEJAR_H
