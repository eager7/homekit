/****************************************************************************
 *
 * MODULE:             http parse.h
 *
 * COMPONENT:          home kit interface
 *
 * REVISION:           $Revision:  1.0$
 *
 * DATED:              $Date: 2017-01-11 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2017. All rights reserved
 *
 ***************************************************************************/
#ifndef HOMEKIT_HTTP_PARSER_H
#define HOMEKIT_HTTP_PARSER_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "accessory_type.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define HTTP_PROTOCOL_HTTP      "HTTP/1.1"
#define HTTP_PROTOCOL_EVENT     "EVENT/1.0"

#define HTTP_TYPE_JSON          "application/hap+json"

#define HTTP_URL_PAIR_SETUP     "/pair-setup"
#define HTTP_URL_PAIR_VERIFY    "/pair-verify"
#define HTTP_URL_IDENTIFY       "/identify"
#define HTTP_URL_PAIRINGS       "/pairings"
#define HTTP_URL_ACCESSORY      "/accessories"
#define HTTP_URL_CHARACTER      "/characteristics"
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    /* Successful HTTP Status Codes */
    E_HTTP_STATUS_SUCCESS_OK                        = 200,
    E_HTTP_STATUS_SUCCESS_NO_CONTENT                = 204,
    E_HTTP_STATUS_SUCCESS_MULTI_STATUS              = 207,

    /* Client Error HTTP Status Codes */
    E_HTTP_STATUS_BAD_REQUEST                       = 400,
    E_HTTP_STATUS_NOT_FOUND                         = 404,
    E_HTTP_STATUS_UNPROCESSABLE_ENTITY              = 422,
    E_HTTP_STATUS_METHOD_NOT_ALLOWED                = 405,
    E_HTTP_STATUS_TOO_MANY_REQUESTS                 = 429,
    E_HTTP_STATUS_CONNECTION_AUTHORIZATION_REQUIRED = 470,

    /* Server Error Status HTTP Codes */
    E_HTTP_STATUS_INTERNAL_SERVER_ERROR             = 500,
    E_HTTP_STATUS_SERVER_UNAVAILABLE                = 503,
} teHttpCode;

typedef enum {
    E_HTTP_POST,
    E_HTTP_PUT,
    E_HTTP_GET,
} teHttpMethod;

typedef struct {
    uint8   acHttpMethod[MIBF];         /* the method of http, can be POST,GET,PUT */
    uint8   acDirectory[MIBF];          /* the directory of http, such as /pair-setup */
    uint8   acHttpVersion[MIBF];        /* the version of http, usually is HTTP/1.1 */
    uint16  u16ContentLen;              /* the length of http data, can be 0 */
    uint8   acContentType[MIBF];        /* the type of http data, such as application/hap+json */
    uint8   acContentData[MABF];        /* the data of http, such as TLV format of json format */
} tsHttpEntry;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/*****************************************************************************
** Prototype    : psHttpParser
** Description  : create a new object for http message
** Input        : pBuf, the data be received by socket
 *                u16Len, the len of pBuf
** Output       : None
** Return Value : if success, return a struct of tsHttpEntry, need to release
 *                after using it, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsHttpEntry *psHttpParser(const uint8 *pBuf, uint16 u16Len);
/*****************************************************************************
** Prototype    : u16HttpFormat
** Description  : add http header, method, version, status and content-type for users' data
** Input        : eStatus, the status of http, such as E_HTTP_STATUS_SUCCESS_OK
 *                psType, the type of http, such as application/hap+json
 *                pBuffer, user's data will send, can be NULL
 *                u16Length, user's data len, can be 0
** Output       : ppResponse, the http message, need to release after using it
** Return Value : return the length of http message

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
uint16 u16HttpFormat(teHttpCode eStatus, const char *psProtocol, const char *psType, const uint8 *pBuffer, uint16 u16Length,
                     uint8 **ppResponse);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_HTTP_PARSER_H
