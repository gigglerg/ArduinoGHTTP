/**
 * \file
 * Embedded HTTP request/response handling framework - Stopwatch
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_STOPWATCH_H
#define GHTTP_STOPWATCH_H

namespace ghttp {

/**
 * Stopwatch class used for asynchronous timeout management
 *
 * \note Tick unit Milliseconds
 *
 * \tparam TT POD type used for timeout storage
 */
template<typename TT>
class StopWatch {
public:
    /**
     * Default constructor, make stable instance.  Timer stopped
     */
    StopWatch() : ticks_(0), running_(false) { }


    /**
     * Get time now from system ticker
     *
     * \return Milliseconds
     */
    TT Now() const {
        return GHTTP_STOPWATCH_MS_TICKER_READ;
    }


    /**
     * Query StopWatch running state
     *
     * \retval true Running
     * \retval false Not running
     */
    bool IsRunning() const {
        return running_;
    }


    /**
     * Start StopWatch running
     */
    void Start() {
        ticks_ = Now();
        running_ = true;
    }


    /**
     * Stop StopWatch if running.  Returns finish ticks
     *
     * \return Milliseconds when stopped (duration since \ref Start)
     */
    TT Stop() {
        if (running_) {
            ticks_ = Since();
            running_ = false;
        }

        return Since();
    }


    /**
     * Since yields an instantaneous StopWatch value since \ref Start if 
     * running.  If stopped result will be 0
     *
     * \todo Deal with overflow where now < ticks_
     *
     * \return Milliseconds
     */
    TT Since() const {
        TT diff = 0;

        if (running_) {
            diff = Now() - ticks_;
        }

        return diff;
    }

protected:
    bool	running_;                               ///< Running flag
    TT      ticks_;                                 ///< Ticks when started
}; // class StopWatch

}; // namespace ghttp

#endif // GHTTP_STOPWATCH_H
