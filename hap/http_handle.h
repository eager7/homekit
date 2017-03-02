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

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_HTTP_STATUS_CODE_SUCCESS                  = 0,
    E_HTTP_STATUS_CODE_REQUEST_DENIED           = -70401,
    E_HTTP_STATUS_CODE_UNABLE_COMMUNICATION     = -70402,
    E_HTTP_STATUS_CODE_RESOURCE_BUSY            = -70403,
    E_HTTP_STATUS_CODE_READ_ONLY                = -70404,
    E_HTTP_STATUS_CODE_WRITE_ONLY               = -70405,
    E_HTTP_STATUS_CODE_NOT_NOTIFICATION         = -70406,
    E_HTTP_STATUS_CODE_OUT_OF_RESOURCE          = -70407,
    E_HTTP_STATUS_CODE_TIMED_OUT                = -70408,
    E_HTTP_STATUS_CODE_NOT_EXIST                = -70409,
    E_HTTP_STATUS_CODE_INVALID_VALUE_TO_WRITE   = -70410,
    E_HTTP_STATUS_CODE_INSUFFICIENT_AUTHORIZATION = -70411,
} teHttpStatusCode;

typedef enum {
    E_HTTP_STATUS_SUCCESS_OK = 200,
    E_HTTP_STATUS_SUCCESS_NO_CONTENT = 204,
    E_HTTP_STATUS_SUCCESS_MULTI_STATUS = 207,
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
uint16 u16HttpFormat(teHttpCode eStatus, const char *psType, const uint8 *pBuffer, uint16 u16Length, uint8 **ppResponse);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_HTTP_PARSER_H
