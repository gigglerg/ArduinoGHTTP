# GHTTP

**GHTTP** is a HTTP request/response C++ library offering a collection of C++ classes written for Ardunio but is portable and can be used in bare metal environments.  GHTTP is data centric and doesn't concern itself with TCP/IP or and inner works of transport layer; working instead with data provided by your integration offering great flexibility through a relatively well defined interface.

If you've ever worked with PHP, you'll know how quick and easy it is to handle HTTP request specifics, headers, cookies, URL processing, sessions and response building using an API which you'd probably consider to be boiler plate functionality that you'd expect from a well developed script language.  Jump on Arduino or for that matter anything embedded and you'll find yourself developing a lot of code that should already exist as you work with various Ethernet and IoT hardware bolt on shields.  It is often far from easy and the coding options can look pretty messy, nothing like what PHP or Python with requests offers.

Use GHTTP to simplify your embedded IoT/web enabled device functionality.  After 15mins playing with the examples you'll see how the simple framework offered looks and feels like PHP and hopefully you'll feel very comfortable using the API where you should be able to concentrate on your application rather than how something is implemented.

[License: GPL2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

I can't force you, but if your work is part of a commerical project or product, please consider a proportional donation to [Abingdon Fridge](https://chuffed.org/project/105084-running-costs) or [The Oxford Foodbank](http://oxfordfoodbank.org/support-us/make-a-donation/)


## Using the library

For now as this is a library, Doxygen documentation is available as follows:

* [All variant documentation](https://gigglerg.github.io/ArduinoGHTTP/release_100/avr8_html/index.htm)

To rebuild simply use make from within the "./docs" folder.  Use "make clean" to tidy.  Doxygen 1.8.15 is required without DOT.


## Requirements

* Supports
  * Arduino ([AVR8 Mega single flash bank](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)) devices
    * [Arduino Uno/Nano](https://store.arduino.cc/arduino-uno-rev3)

  * Arduino ([Due AT91SAM](https://store.arduino.cc/due))
    
  * [STM32 Cortex M series](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)

  * Bare metal MCu development, Arduino isn't required.  You'll have to define some preprocessor directives before including the library to replace the few simple hooks that exist into Arduino.  You should not need to change library code.
  
  * There is no dynamic memory allocation

* Restrictions

  * Requests up to 63K
  
  * Initially ISO 8859-1 supported.  UTF8 to be added

  * HTTP response "Continue" not presently suppoted
  
* Features

  * Request decoding to RFC1738 with access to
    * URL
    * Method
    * Username/password
    * Protocol/protocol version
    * Content
    * Query string
    * Post variables (RFC 7578), including optionally files (RFC 1867)
      * Framwwork support for content types "application/x-www-form-urlencoded", "multipart/form-data"

  * Headers for both request and response

  * Cookies, RFC 2109 and RFC 6265
  
  * Sessions RFC 6265

    * Can be related to both IP4 and MAC for added security

  * URI processing to RFC 3986
  
  * Mime, base 64 encoding/decoding option, RFC 2045
  
  * Simple route manager and OOD method of handling different requests where you define Routes
  
  * Hardware independent


## Library Installation

GHTTP is available for installation in the [Arduino Library Manager].  Alternatively it can be downloaded from GitHub using the following:

1. Download the ZIP file (pick a release branch) from https://github.com/gigglerg/ArduinoGHTTP.git
2. Rename the downloaded file to `GHTTP.zip`
3. From the Arduino IDE: Sketch -> Include Library... -> Add .ZIP Library...
4. Restart the Arduino IDE to see "GHTTP" library with examples

See the comments in the example sketches for coding use details.


### Include library

```cpp
// Include all headers
#include "ghttp.h"

using namespace ghttp;
```

The above includes everything but if you want to be specific, look at the source and include individual headers as desired.  Look at the examples for ideas and further information.

The main include has default macros which you can overide by defining them before including.  These include but not limited to:

* Macros
  * GHTTP_SESSIONID_LENGTH                  - Session string length
  * GHTTP_SESSION_EXPIRE_TIME                - Session timeout (Seconds)
  * GHTTP_SESSIONMGR_INCLUDE_MAC            - Include MAC in session handling
  * GHTTP_REQUEST_MAX_HEADERS                - Request header maximum
  * GHTTP_REQUEST_MAX_VARS                    - GET/POST variables maximum
  * GHTTP_REQUEST_MAX_CONTENT_TYPE            - Content type variables list maximum
  * GHTTP_REQUEST_PROCESS_POST_VARS            - Flag, control post variable processing
  * GHTTP_REQUEST_BAD_POST_DATA_IGNORE        - Flag, ignore post data errors
  * GHTTP_REQUEST_PROCESS_POST_FILE_VARS    - Flag, post file variables used
  * GHTTP_REQUEST_MAX_POST_FILE                - Maximum post file variables
  * GHTTP_REQUEST_PROCESS_COOKIES            - Flag, cookies processed
  * GHTTP_SERVER_USE_SESSIONS                - Flag, sessions used
  * GHTTP_REQUEST_MAX_COOKIE                - Maximum cookies
  * GHTTP_SERVER_STRING_SESSION_COOKIE        - Session cookie name
  * GHTTP_USE_HELPERS                        - Flag, use helpers

* Macros (platform specific which require ports for bare metal)
  * GHTTP_STOPWATCH_MS_TICKER_READ            - Read system ticker (Milliseconds)
  * GHTTP_SESSIONRND_GENERATE                - Source for session string generation
  * GHTTP_SESSIONRND_ENTROPHY_SRC1            - Session entrophy sources
  * GHTTP_SESSIONRND_ENTROPHY_SRC2


## Implementation Topology

Create a class to contain application specific session data (assuming sessions are used).  There are some required methods which intentionally have not been made virtual.

```cpp
class SessionData {
public:
    /* Add anything you want your application to process here... */

public:
    SessionData() { }

    /* Invoked by framework to clear your globals */
    void Clear(const uint8_t session_idx) { }

    /* Invoked by framework when session started using these globals */
    void Start(const uint8_t session_idx) { }

    /* Invoked by framework when no longer used */
    void Stop(const uint8_t session_idx) { }

    /* Invoked by framework when reused */
    void Continue(const uint8_t session_idx) { }
};
```

Create a class that is shared among your Routes, offering access to framework.  This will generally resembles PHP's super globals giving access to request, response and other characteristics

```cpp
class Shared {
public:
    SessionManager<10, Session<SessionData, SessionId<GHTTP_SESSIONID_LENGTH>, GHTTP_SESSION_EXPIRE_TIME>, uint32_t> session_manager;
    HTTPRequest<GHTTP_REQUEST_MAX_HEADERS, GHTTP_REQUEST_MAX_VARS, GHTTP_REQUEST_MAX_CONTENT_TYPE, GHTTP_REQUEST_MAX_POST_FILE, GHTTP_REQUEST_MAX_COOKIE> request;
    HTTPResponse<HTTPResponseStatus> response;
    struct {
        IP4_t    ip4;
        MAC_t    mac;
    }client;
    struct {
        IP4_t    ip4;
    }server;
}; // class Shared
```

Define one or more routes

```cpp
class RouteMsg : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!="/msg" || s.request.method!="get" || s.request.protocol!="http") {
            return false;
        }

        // Start session and get session variables
        auto &session = s.session_manager.StartSession(s.client.ip4, s.request.session.Get());
        auto &session_vars = session.user_data; // This will be your Global class instance which is per session

        // Access your session variables via "session_vars.xxxx"

        // Build response
        s.response.HeaderComplete();
        s.response.Append("<html><body><p>Other Msg...</body></html>");
        s.response.Complete();

        return true;
    }
};


class RouteOther : public Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!="/other" || s.request.method!="get" || s.request.protocol!="http") {
            return false;
        }
        
        // I don't want sessions for this route

        // Build response
        s.response.HeaderComplete();
        s.response.Append("<html><body><p>Other route...</body></html>");
        s.response.Complete();

        return true;
    }
};

```


Handle request/response, normally a main loop or FRTOS task

```cpp
uint8_t buffer[2048];

Shared sd;
RouteManager<5, Route<Shared>, Shared>  rm(sd);    // Maximum of 5 routes in this example

void Setup() {
    sd.client.ip4 = "10.0.0.10";    // This would have been setup before request processing
    sd.server.ip4 = "10.0.0.2";        // This would have been setup before request processing

    sd.response.SetMaximum(sizeof(buffer)-200);

    // Create our routes handled by this app.  These would be instanglobal not on stack
    RouteMsg route_msg;
    RouteOther route_other;

    // Register routes (done once)
    rm.Register(&route_msg);
    rm.Register(&route_other);
}

// Caller should send buffer to returned length in response to request
uint16_t Received_Request(char *buffer, uint16_t length, uint16_t max_length) {
    // Parse our request...
    uint16_t ci = sd.request.Parse(buffer, length, max_length);
    if (sd.request.ERR_NONE != sd.request.GetLastError()) {
        // Error
    }else {
        rm.Service();
        sd.session_manager.Service();
    }
    
    // Here is your response to send...
    // sd.response.GetBuffer();
    
    return sd.response.GetLength();
}
```


## TODOs

* 1).
HTTP binary standard
* 2).
Multiple forms where the name includes the form name following name="form[field]"
* 3).
UTF8


## Gotchas

None yet.


## Ethercard library corrections for STM32

Maple Mini has been my choice of development platform due mainly to Cortex M3 resources over the original ATMega328P devices used by the first Arduino boards.  The 
library is part of Arduino_STM32 and can be found here:

https://github.com/rogerclarkmelbourne/Arduino_STM32/tree/master/STM32F1/libraries/Serasidis_EtherCard_STM

However the ported to STM32 is now really stale and contains numerous bugs that have subsequently been fixed in Ethercard.   Latest ATMega library:

https://github.com/njh/EtherCard.git

I have included a patch in the examples which should be applied to the head of the latest library and that was for me:

commit 7aaf71cb034bc4b6436273edc1e06ea187d34d28 (HEAD -> master, origin/master, origin/HEAD)
Author: Walter Stanish <505366+globalcitizen@users.noreply.github.com>
Date:   Fri Jan 10 10:27:32 2020 +1100

    Update UIPEthernet repo URL (#380)


Without patching, most of the examples I've written will work but anything requiring multiple packets to receive and send data will likely fail.  So take 
Serasidis_EtherCard_STM and apply the patch 'enc28j60.cpp.patch' found in the root of GHTTP library.


## Contributing

Contributions to the code are welcome using the normal Github pull request workflow.

Please remember the small memory footprint of most micro controllers like Arduino, so try and be efficient when adding new features.


### Code Style

When making contributions, please follow [Google's code style](https://google.github.io/styleguide/cppguide.html) where possible to keep the codebase consistent.

* Indent *4 spaces*, No hard tabs
* EOL Unix (Library code)
* The rest is online!
