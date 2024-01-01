/**
 * \file
 * Embedded HTTP request/response handling framework - A route
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_ROUTE_H
#define GHTTP_ROUTE_H

namespace ghttp {

/**
 * A class pure virtual class representing a route used by \ref RouteManager during processing request/responses
 *
 * \tparam T Class of your application specific shared data
 */
template<class T>
class Route {
public:
    /**
     * Route handler.  Route manager will invoke as new requests come in to system and you decide if a particular request 
     * should be handled or not.  Routes can exist that all look similar, for example they may all accept the same URL but only accept
     * specific HTTP method like GET, HEAD, POST, PUT, DELETE etc.
     *
     * \param[in] s Reference to shared data.  This is application specific and normally includes everything required for processing routes
     *
     * \return Handled state
     * \retval true Route handled
     * \retval false Route not handled
     */
    virtual bool Handler(T &s) = 0;
}; // class Route

}; // namespace ghttp

#endif // GHTTP_ROUTE_H
