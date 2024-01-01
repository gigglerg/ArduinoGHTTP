/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_get_json_rest_die
 * 
 * Ethercard TCP/IP packet i/f wrapper to simplify application and remove h/w detail.  You don't have to use this API
 * 
 * DG, 2020
 */

#if !defined(ETHERCARD_WRAP_H)
#define ETHERCARD_WRAP_H

namespace examples { namespace ghttp_ethercard_get_json_rest_die {

#if !defined(ECWRAP_UPDATE_CLIENT_IP)
/**
 * Force update of client IP4 address in shared instance when ECWrap::UpdateClientInformation invoked.  This is 
 * an alternative to GHTTP_SERVER_USE_SESSIONS when not enabled for the rest of the framework
 *
 * @note Define your own to override this default which is don't update client IP4
 */
#define ECWRAP_UPDATE_CLIENT_IP             0
#endif


#if !defined(ECWRAP_UPDATE_CLIENT_MAC)
/**
 * Force update of client MAC address in shared instance when ECWrap::UpdateClientInformation invoked.  This is 
 * an alternative to GHTTP_SERVER_USE_SESSIONS with GHTTP_SESSIONMGR_INCLUDE_MAC when not enabled for the rest 
 * of the framework
 *
 * @note Define your own to override this default which is don't update client MAC
 */
#define ECWRAP_UPDATE_CLIENT_MAC            0
#endif


#if !defined(ECWRAP_TX_SEGMENT_SIZE)
/**
 * Transmit segmented data over 1 or more packets using a maximum payload size of N bytes.  This is 
 * related to the ethernet MTU
 *
 * @note Define your own to override this default
 */
#define ECWRAP_TX_SEGMENT_SIZE              1024    // Bytes, TCP payload
#endif


#if !defined(ECWRAP_RX_SEGMENT_TIMEOUT)
/**
 * Segmented receive timeout (ms).  Finger in the air, it's blowing north today so 30ms seems good.
 * Assumed LAN traffic only, not going through WAN/Gateway.  Observed segmented receives were between 
 * 6ms to 13ms, double that for Nyquist
 *
 * @note Define your own to override this default
 */
#define ECWRAP_RX_SEGMENT_TIMEOUT           30
#endif


#if !defined(ECWRAP_RTX_PACKET_LENGTH)
/**
 * Length of Ethercard raw packet buffer as byte Ethernet::buffer[ECWRAP_PACKET_BUFFER_LENGTH]
 * This is related to the ethernet MTU
 *
 * @note Define your own to override this default
 */
#define ECWRAP_RTX_PACKET_LENGTH            1500
#endif


#if !defined(ECWRAP_REQRSP_BUFFER_LENGTH)
/**
 * Length of buffer used by GHTTP for request/response parse/building
 *
 * @attention Must be > ECWRAP_TX_SEGMENT_SIZE and >= ECWRAP_RTX_PACKET_LENGTH
 *
 * @note Define your own to override this default
 */
#define ECWRAP_REQRSP_BUFFER_LENGTH         (4*1024)
#endif


/**
 * Wrapper for Ethercard to hide low level functionality and i/f from application
 */
class ECWrap {
public:
    /**
     * Initialise Ethercard + Enc28j60 i/f driver with static IPs
     *
     * \param[in] server_port Server TCP/IP port
     * \param[in] ip4 Byte array pointer (4 Octets) representing IP4 address of node on LAN
     * \param[in] gwip4 Byte array pointer (4 Octets) representing IP4 address of local gateway
     * \param[in] mac Byte array pointer (6 Octets) representing MAC address of node on LAN
     * \param[in] pin MCU/board specific SPI0 bus, slave select pin
     */
    static uint8_t Initialise(const uint16_t server_port, const uint8_t* ip4, const uint8_t* gwip4, const uint8_t* mac, uint8_t pin);


    /**
     * Get client information from Ethercard packet buffer and update shared instance
     *
     * \note Updates when GHTTP_SERVER_USE_SESSIONS optionally GHTTP_SESSIONMGR_INCLUDE_MAC set to 1
     * or ECWRAP_UPDATE_CLIENT_IP or ECWRAP_UPDATE_CLIENT_MAC
     */
    static void UpdateClientInformation();


    /**
     * Send 1 or more TCP packets, segmenting GHTTP response to client
     * This should really be implemented by a proper POSX socket layer instead of this noddy Ethercard/Enc28j60 api
     *
     * \param[in] response HTTP Your response buffer or user buffer
     * \param[in] response_length Your response length (Bytes/ASCII characters)
     * \param[in] first Flag indicating first invoke or data segment (default true)
     * \param[in] more Flag indicating more data to follow (default false)
     */
    static void Send(const char *response, uint32_t response_length, const bool first=true, const bool more=false);


#if defined(ARDUINO_ARCH_AVR)
    /**
     * Send 1 or more TCP packets, segmenting GHTTP response to client
     * This should really be implemented by a proper POSX socket layer instead of this noddy Ethercard/Enc28j60 api
     *
     * \todo Template the parameter?
     *
     * \param[in] response HTTP Your response buffer or user buffer (flash based)
     * \param[in] response_length Your response length (Bytes/ASCII characters)
     * \param[in] first Flag indicating first invoke or data segment (default true)
     * \param[in] more Flag indicating more data to follow (default false)
     */
    static void Send(PGM_VOID_P response, uint32_t response_length, const bool first=true, const bool more=false);
#endif


    /**
     * Receive 1 or more TCP packets, segmenting GHTTP request from client.  GHTTP parse is used to decide 
     * if request complete and ready for processing.
     *
     * \note Internally invokes @ref UpdateClientInformation
     *
     * Implementation follows:
     * 1. Read request first line.  Parse it, Are we done with no errors?
     * 2. Read headers (if any).  Parse it, Are we done with no errors?
     * 3. Content (if any).  Parse it, Are we done with no errors? 
     *
     * \todo Deal with millis() overflow and roll, right now it will indicate timeout
     * 
     * \param[out] err Pointer to request parse error code
     * sd.request.GetLastError().  Everything is an error that isn't mentioned below
     * sd.request.ERR_NONE Assume request parse complete.  Should send response
     * sd.request.ERR_SEGMENTATION Send acknowledge
     * sd.request.ERR_PARSE_UNEXPECTED_DATA On request buffer overflow.  Should redirect
     * \return Receive status
     * \retval true HTTP request received and parsed
     * \retval false Nothing yet to report
     */
    static bool Receive(uint8_t *err);


    /**
     * Get Ethercard packet buffer, payload start which is after IP+TCP headers
     *
     * \return Data buffer pointer
     */
    static char* GetPacketPayloadBuffer();


    /**
     * Get Ethercard packet buffer payload maximum length
     *
     * \return Data buffer allocation length
     */
    static uint16_t GetPacketPayloadBufferLength();


    /**
     * Get GHTTP request/response buffer
     *
     * \return Buffer pointer
     */
    static char* GetHTTPBuffer();


    /**
     * Get GHTTP request/response buffer maximum length
     *
     * \return Buffer allocation length
     */
    static uint16_t GetHTTPBufferLength();

}; // class ECWrap

} } // namespace examples::ghttp_ethercard_get_json_rest_die

#endif // !defined(ETHERCARD_WRAP_H)
