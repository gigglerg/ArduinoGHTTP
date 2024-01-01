/**
 * \file
 * Embedded HTTP request/response handling framework - Attr/value string set for files
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPREQUESTFILESTRINGSET_H
#define GHTTP_HTTPREQUESTFILESTRINGSET_H

namespace ghttp {

/**
 * A container class representing an array of files used during request processing of post variables
 *
 * \tparam N Array size
 */
template<uint8_t N>
class HTTPRequestFileStringSet : public HTTPAttrValueStringSetGeneric<N, HTTPAttrFileString> { };

}; // namespace ghttp

#endif // GHTTP_HTTPREQUESTFILESTRINGSET_H
