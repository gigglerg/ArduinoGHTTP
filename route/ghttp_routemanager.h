/**
 * \file
 * Embedded HTTP request/response handling framework - Route manager
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_ROUTEMANAGER_H
#define GHTTP_ROUTEMANAGER_H

namespace ghttp {

/**
 * Routemanager class used to guide processing of requests and handling failures along with servicing request/response framework
 *
 * \note The last register route should be your unhandled route handler if you want to deal with this situation
 *
 * \tparam N Route
 * \tparam R Class of route managed by this instance
 * \tparam T Class of your application specific shared data
 */
template<uint8_t N, class R, class T>
class RouteManager {

public:
    /**
     * Constructor, make stable instance.  Route count 0, no pre-route or any other routes
     */
    RouteManager(T* s=NULL) : shared_(s) {
        Clear();
    }

    
    /**
     * Register shared instance passed to routes when being handled
     *
     * \note Use this method if you don't specify by constructor invoke
     *
     * \param[in] s Pointer to shared instance
     */
    void RegisterShared(T* s) {
        shared_ = s;
    }


    /**
     * Register route.  The new route can be a normal route, the last normal route could accept any route to handle the condition where 
     * no other route accepted request and finally it could be a pre-route handler.   A pre-route handler makes sure semantics of the request 
     * are generally what your application accepts and can setup other globals to aid your other requests.
     *
     * \param[in] r Route pointer  Make sure your routes are global, don't create them on stack
     * \param[in] pre_route_check Flag indicating this route is the pre-route check, default false => normal route
     * \return State
     * \retval +n Route count after addition
     * \retval -1 Failed.  Make sure N is larger enough for your application
     */
    int16_t Register(R *r, const bool pre_route_check=false) {
        bool done = false;
        
        if (r) {
            if (pre_route_check) {
                pre_ = r;
                done = true;
            }else {
                if (count_<N) {
                    route_[count_++] = r;
                    done = true;
                }
            }
        }
        
        return static_cast<int16_t>(done?count_-1:-1);
    }


    /**
     * Route count.  Query registered routes with Route manager
     *
     * \return Routes 0>= n <N
     */
    uint8_t RouteCount() const {
        return count_;
    }


    /**
     * Service routes.   Invoke after new requests received by TCP/IP or whatever lower layer you use to populate the request buffer
     *
     * \note Only invoke when complete request ready for processing so do not invoke on partial receives
     * \todo Refactor for partial receives so HTTP status Continue handled correctly
     *
     * \return Route index of handler that accepted
     * \retval +n Route
     * \retval -1 No route handled data
     */
    int16_t Service() {
        uint8_t i;
        bool handled = false;

        if (shared_) {
            if (pre_) {
                handled = pre_->Handler(*shared_);
            }

            if (!handled) {
                for(i=0; i<count_; i++) {
                    if (route_[i]) {
                        handled = route_[i]->Handler(*shared_);
                        if (handled) {
                            break;
                        }
                    }
                }
            }
        }

        return static_cast<int16_t>(handled?i:-1);
    }
    

    /**
     * Clear routes.  Invoke if you dynamically change registered routes which is application specific
     *
     * \attention Most applications do not want to this behaviour and will not invoke this method
     */
    void Clear() {
        pre_ = NULL;
        for(uint8_t i=0; i<N; i++) {
            route_[i] = NULL;
        }
        count_ = 0;
    }

protected:
    uint8_t   count_;                               ///< Route count
    R*        pre_;                                 ///< Pre-route pointer
    R*        route_[N];                            ///< Array of route pointers
    T*        shared_;                              ///< Pointer to shared instance passed to routes during request/response processing
}; // class RouteManager

}; // namespace ghttp

#endif // GHTTP_ROUTEMANAGER_H
