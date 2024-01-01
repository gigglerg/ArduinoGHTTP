/**
 * \file
 * Embedded HTTP request/response handling framework
 * PROJECT: GHTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_H
#define GHTTP_H

#if defined(ARDUINO_ARCH_AVR)
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif


namespace ghttp {

/*
 * For AVR architecture
 */

#if !defined(T)
#if defined(ARDUINO_ARCH_AVR)
/**
 * Text marco.  For AVR places given string in flash memory via PROGMEM attribute in order to reduce SRAM use
 *
 * \note For parameters, use type PGM_VOID_P
 * \note Not all framework functions take flash memory pointers, check before use
 * \attention Define your own if you want some other functionality
 *
 * \param[in] x Character string
 * \return String pointer
 */
#define T(x)                                    (static_cast<PGM_VOID_P>(PSTR(x)))
#else // !defined(ARDUINO_ARCH_AVR)

/**
 * Text marco.  Unless AVR does nothing
 *
 * \attention Define your own if you want some other functionality
 *
 * \param[in] x Character string
 * \return String
 */
#define T(x)                                    (x)

#endif // !defined(ARDUINO_ARCH_AVR)
#endif // !defined(T)


#if defined(ARDUINO_ARCH_AVR)

#if !defined(PGM_CHAR)
/**
 * Read character from AVR flash memory
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with pgm_read_byte_far
 *
 * \param[in] x Program/flash memory address
 * \return Character
 */
#define PGM_CHAR(x)                             static_cast<char>(pgm_read_byte_near((uint16_t)x))
#endif


#if !defined(PGM_STRLEN)
/**
 * Get string length from AVR flash memory string
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with strlen_PF
 *
 * \param[in] x String pointer (flash based)
 * \return N Character count (excluding NULL)
 */
#define PGM_STRLEN(x)                           strlen_P(x)
#endif


#if !defined(PGM_STRNLEN)
/**
 * Get string length from AVR flash memory string with maximum length
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with strlen_PF
 *
 * \param[in] x String pointer (flash based)
 * \param[in] s Size, Characters
 * \return N Character count (excluding NULL)
 */
#define PGM_STRNLEN(x,s)                        strnlen_P(x,s)
#endif


#if !defined(PGM_STRCAT)
/**
 * String concatenate from AVR flash memory string
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with strcat_PF
 *
 * \param[in] x String pointer (SRAM based)
 * \param[in] y String pointer (flash based)
 * \return x String pointer
 */
#define PGM_STRCAT(x,y)                         strcat_P(x,y)
#endif


#if !defined(PGM_MEMCPY)
/**
 * Memory copy from AVR flash memory string
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with memcpy_PF
 *
 * \param[in] x Destination pointer (SRAM based)
 * \param[in] y Source pointer (flash based)
 * \param[in] s Size, Bytes
 * \return x Destination pointer
 */
#define PGM_MEMCPY(x,y,s)                       memcpy_P(x,y,s)
#endif


#if !defined(PGM_STRCPY)
/**
 * String copy from AVR flash memory string
 *
 * \attention Define your own as required.  If using larger AVR with multiple flash banks define with strcpy_PF
 *
 * \param[in] x Destination pointer (SRAM based)
 * \param[in] y Source pointer (flash based)
 * \param[in] s Size, Bytes
 * \return x Destination pointer
 */
#define PGM_STRCPY(x,y,s)                       strcpy_P(x,y,s)
#endif

#endif // !defined(ARDUINO_ARCH_AVR)

/*
 * For support
 */

#if !defined(GHTTP_STOPWATCH_MS_TICKER_READ)
/**
 * Macro defined to give StopWatch class a system ticker source
 *
 * \attention Define your own or use this default with Arduino framework
 *
 * \return Milliseconds ticker count.  This is the unit of StopWatch class
 */
#define GHTTP_STOPWATCH_MS_TICKER_READ          millis()
#endif


/*
 * For sessions
 */

#if !defined(GHTTP_SESSIONRND_GENERATE)
/**
 * Random session ID generation, string length
 *
 * \attention Define your own session generation macro if you want to replace default operation/class used
 *
 * \param[in] n Length in characters (including NULL) of generated session ID
 * \return String pointer
 */
#define GHTTP_SESSIONRND_GENERATE(n)            SessionRnd<n>::Generate()
#endif


#if !defined(GHTTP_SESSIONRND_ENTROPHY_SRC1)
/**
 * Random session entrophy source 1.  Under Arduino uses millis(), replace if using bare metal environment
 *
 * \attention Use this macro to define your own entrophy source
 *
 * \return Integer
 */
#define GHTTP_SESSIONRND_ENTROPHY_SRC1          millis()
#endif


#if !defined(GHTTP_SESSIONRND_ENTROPHY_SRC2)

#if defined(ARDUINO_MAPLE_MINI)
/**
 * Random session entrophy source 2.  Under Arduino for Maple Mini uses analogue read of input (adc8), replace if using bare metal environment or
 * with another floating input
 *
 * \attention Use this macro to define your own entrophy source
 *
 * \return Integer
 */
#define GHTTP_SESSIONRND_ENTROPHY_SRC2          analogRead(D3)
#endif

/**
 * Random session entrophy source 2.  Under Arduino for Mega/Due uses analogue read of input (adc7), replace if using bare metal environment or
 * with another floating input
 *
 * \attention Use this macro to define your own entrophy source
 *
 * \return Integer
 */
#define GHTTP_SESSIONRND_ENTROPHY_SRC2          analogRead(A7)

#endif // !defined(GHTTP_SESSIONRND_ENTROPHY_SRC2)


#if !defined(GHTTP_SESSIONID_LENGTH)
/**
 * Application specific session id string length, characters (including NULL)
 *
 * \attention Define your own or use this default.  You don't need to use this macro since it's a template parameter to ghttp::SessionId
 *
 * \return Characters (including NULL)
 */
#define GHTTP_SESSIONID_LENGTH                  33
#endif


#if !defined(GHTTP_SESSION_EXPIRE_TIME)
/**
 * Application specific macro defines a session instance timeout in Seconds
 *
 * \note This is per active session and relates to ghttp::Session::GetLastUse
 * \attention Define your own or use this default value.  You don't need to use this macro since it is a template parameter to ghttp::Session
 *
 * \return Seconds
 */
#define GHTTP_SESSION_EXPIRE_TIME               (60*30)    /// Seconds (30Minutes)
#endif


/**
 * Macro to include MAC address in Session management otherwise simplify to use IP.  If security an issue define as 1
 *
 * Sessions are related to clients to link them session variables via network addresses, commonly this is simply IP or IP:port but here we can 
 * include MAC.  It is harder to spoof a MAC on the same LAN where a node exists already with the same MAC controlling your device
 *
 * \attention Define as 1 to include client MAC address in session management
 */
#if !defined(GHTTP_SESSIONMGR_INCLUDE_MAC)
#define GHTTP_SESSIONMGR_INCLUDE_MAC            0
#endif


#if !defined(GHTTP_SESSIONMGR_MAX_SESSIONS)
/**
 * Application specific macro sets the maximum unique sessions handled by session manager (the HTTP server)
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_SESSIONMGR_MAX_SESSIONS           10
#endif


/*
 * For core
 */

#if !defined(GHTTP_REQUEST_MAX_HEADERS)
/**
 * Application specific macro to define upper limit on request header strings.  Passed to constructor of your request instance
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_REQUEST_MAX_HEADERS               25
#endif


#if !defined(GHTTP_REQUEST_MAX_VARS)
/**
 * Macro to define upper limit on request query and post strings (GET/POST variables).  Passed to constructor of your request instance
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_REQUEST_MAX_VARS                  10
#endif


#if !defined(GHTTP_REQUEST_MAX_CONTENT_TYPE)
/**
 * Macro to define upper limit on request content substrings.  During request processing content type header is broken down into
 * smaller accessible strings.  Some of these are useful to determine extra information about the request and this macro limits 
 * the number of substrings it can be broken into.  Passed to constructor of your request instance
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_REQUEST_MAX_CONTENT_TYPE          4
#endif


#if !defined(GHTTP_REQUEST_PROCESS_POST_VARS)
/**
 * Macro define to indicate that post variables should not be processed.   You can per request, override this with a flag on 
 * \ref HTTPRequest::Parse but this macro must still be defined as 1 to enable override
 *
 * \attention Define as 0 if you do not want to process post variables for any of your requests
 */
#define GHTTP_REQUEST_PROCESS_POST_VARS         1
#endif


#if !defined(GHTTP_REQUEST_BAD_POST_DATA_IGNORE)
/**
 * Macro define to ignore errors in post data while processing request post variables
 *
 * \note Doesn't do anything if \ref GHTTP_REQUEST_PROCESS_POST_VARS not 1
 *
 * \attention Define as 1 if you do care about field and data errors during processing of post variables
 */
#define GHTTP_REQUEST_BAD_POST_DATA_IGNORE      0
#endif


#if !defined(GHTTP_REQUEST_PROCESS_POST_FILE_VARS)
/**
 * Macro define to include file input types decoded during post variable processing
 *
 * \note Doesn't do anything if \ref GHTTP_REQUEST_PROCESS_POST_VARS not 1
 *
 * \attention Define as 0 if you do not want to process file post variables
 */
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS    1
#endif


#if !defined(GHTTP_REQUEST_MAX_POST_FILE)
/**
 * Application specific macro to define upper limit on request post files variables.  Most applications won't require file 
 * uploads
 *
 * \note Doesn't do anything if GHTTP_REQUEST_PROCESS_POST_FILE_VARS not 1
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_REQUEST_MAX_POST_FILE             3
#endif


#if !defined(GHTTP_REQUEST_PROCESS_COOKIES)
/**
 * Macro define to indicate cookie headers should be processed during request handling
 *
 * \attention Define as 0 if you do not want to process request cookies
 */
#define GHTTP_REQUEST_PROCESS_COOKIES           1
#endif


#if !defined(GHTTP_SERVER_USE_SESSIONS)
/**
 * Macro define to enable sessions in both request and response handling (the HTTP server)
 *
 * \attention Define as 0 if you do not want sessions or session data
 */
#define GHTTP_SERVER_USE_SESSIONS               1
#endif


#if !defined(GHTTP_REQUEST_MAX_COOKIE)
/**
 * Application specific macro sets the maximum cookies decoded by request
 *
 * \note Doesn't do anything if GHTTP_REQUEST_PROCESS_COOKIES not 1
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_REQUEST_MAX_COOKIE                15
#endif


#if !defined(GHTTP_SERVER_MAX_ROUTES)
/**
 * Application specific macro sets the maximum routes handled by route manager (the HTTP server)
 *
 * \attention Define your own or use this default.  This is application specific and not used by framework directly
 */
#define GHTTP_SERVER_MAX_ROUTES                 10
#endif


#if defined(GHTTP_REQUEST_PROCESS_COOKIES) && GHTTP_SERVER_USE_SESSIONS == 1 && !defined(GHTTP_SERVER_STRING_SESSION_COOKIE)
/**
 * Macro specifies the session cookie name used by both request and response
 *
 * \note Doesn't do anything if \ref GHTTP_SERVER_USE_SESSIONS and \ref GHTTP_REQUEST_PROCESS_COOKIES are not defined
 *
 * \attention Define your own or use this default
 */
#define GHTTP_SERVER_STRING_SESSION_COOKIE      T("SESSION")
#endif // defined(GHTTP_REQUEST_PROCESS_COOKIES) && GHTTP_SERVER_USE_SESSIONS == 1 && !defined(GHTTP_SERVER_STRING_SESSION_COOKIE)


#if !defined(GHTTP_SERVER_ACCEPT_AUTHORIZATION)
/**
 * Macro specifies if server accepts HTTP authorization (type basic parsed by request).  If employed then your route(s) should include 
 * something like the following header as part of a redirect when invalid username and/or password are given:
 *
 * s.response.Header(T("WWW-Authenticate: Basic realm='You must authenticate with server', charset='iso-8859-3'"));
 *
 * \attention Define as 1 if you do want HTTP authorization (default 0)
 */
#define GHTTP_SERVER_ACCEPT_AUTHORIZATION       0
#endif


#if !defined(GHTTP_SERVER_AUTHORIZATION_REUSE_VARS)
/**
 * Macro specifies if server accepts HTTP authorization then the URL username and password are reused since it is unlikely a client will 
 * use both methods.  Enabling this macro will overwrite request.username and request.password with HTTP authorization detail in an effort
 * to reduce memory footprint
 *
 * When not enabled (default) then request.authorization_username and request.authorization_password are setup otherwise request.username
 * and request.password are.
 *
 * \attention Define as 1 if you do want HTTP authorization to overwrite URL request.username and request.password (default 0)
 */
#define GHTTP_SERVER_AUTHORIZATION_REUSE_VARS   0
#endif


#if !defined(GHTTP_USE_HELPERS)
/**
 * Macro define to include use of class \ref HTTPHelper
 *
 * \attention Define as 0 if you do not want the helpers or have your own
 */
#define GHTTP_USE_HELPERS                       1
#endif

}; // namespace ghttp


#include "support/ghttp_stringhelper.h"
#include "support/ghttp_ip4.h"
#include "support/ghttp_mac.h"
#include "support/ghttp_stopwatch.h"
#include "support/ghttp_mimebase64.h"
#include "support/ghttp_uri.h"

#include "substring/ghttp_httpsubstring.h"
#include "substring/ghttp_httpattrvaluestring.h"
#include "substring/ghttp_httpattrvaluestringsetgeneric.h"
#include "substring/ghttp_httpattrvaluestringset.h"
#include "substring/ghttp_httpattrfilestring.h"
#include "substring/ghttp_httprequestcookiejar.h"
#include "substring/ghttp_httprequestfilestringset.h"

#include "session/ghttp_sessionrnd.h"
#include "session/ghttp_sessionid.h"
#include "session/ghttp_session.h"
#include "session/ghttp_sessionmgr.h"

#include "route/ghttp_route.h"
#include "route/ghttp_routemanager.h"

#include "core/ghttp_httpbuffer.h"
#include "core/ghttp_httprequest.h"
#include "core/ghttp_httpresponsestatus.h"
#include "core/ghttp_httpresponse.h"
#include "core/ghttp_httphelper.h"

#endif // GHTTP_H
