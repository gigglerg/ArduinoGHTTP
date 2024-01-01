/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_post_file_upload
 * 
 * Application specific include of GHTTP library complete with customisations via macros.  Put into a separate header 
 * for easy include with the same parameters into any application source file
 * 
 * Requires: https://github.com/stm32duino 1.4.0 and library Serasidis_EtherCard_STM with enc28j60.cpp.patch applied
 *
 * DG, 2020
 */

#if !defined(GHTTP_APP_H)
#define GHTTP_APP_H

/* Adjust some parameters to suit our application */
#define GHTTP_REQUEST_PROCESS_COOKIES 1
#define GHTTP_REQUEST_MAX_COOKIE 10
#define GHTTP_SERVER_USE_SESSIONS 0
#define GHTTP_REQUEST_MAX_HEADERS 15
#define GHTTP_REQUEST_MAX_VARS 5
#define GHTTP_SERVER_MAX_ROUTES    2
#define GHTTP_REQUEST_PROCESS_POST_VARS 1
#define GHTTP_REQUEST_PROCESS_POST_FILE_VARS 1
#define GHTTP_REQUEST_MAX_POST_FILE 1

#define ECWRAP_REQRSP_BUFFER_LENGTH (4*1024)

#include "arduino.h"
#include "ghttp.h"
#include "shared.h"
#include "routeroot.h"

#endif // !defined(GHTTP_APP_H)
