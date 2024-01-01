/**
 * \file
 * Embedded HTTP request/response handling framework - Attr/value string set
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_HTTPATTRVALUESTRINGSET_H
#define GHTTP_HTTPATTRVALUESTRINGSET_H

namespace ghttp {

/**
 * A container class for attribute/value made up of an array of N * class HTTPAttrValueString
 *
 * \tparam N Array size
 */
template<uint8_t N>
class HTTPAttrValueStringSet : public HTTPAttrValueStringSetGeneric<N, HTTPAttrValueString> { };

}; // namespace ghttp

#endif // GHTTP_HTTPATTRVALUESTRINGSET_H
