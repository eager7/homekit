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

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_HTTP_PARSER_OK = 0x00,
    E_HTTP_PARSER_ERROR,
} teHttpStatus;

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
    int   iHttpStatus;
    uint8 acHttpMethod[MIBF];
    uint8 acDirectory[MIBF];
    uint8 acHttpVersion[MIBF];
    uint16 u16ContentLen;
    uint8 acContentType[MIBF];
    uint8 acContentData[MABF];
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
tsHttpEntry *psHttpParser(const uint8 *pBuf, uint16 u16Len);
char *psHttpFormat(teHttpCode eStatus, const char *psType, uint8 *pBuffer, uint16 u16Length);
teHttpStatus eHttpParser(teHttpMethod eMethod, const uint8 *pBuf, uint16 u16Len, tsHttpEntry *psHttpEntry);
teHttpStatus eHttpResponse(int iSockFd, tsHttpEntry *psHttpEntry, uint8 *pBuffer, uint16 u16Length);
uint16 u16HttpMessageFormat(int iStatus, const char *psContent, const char *pBuffer, uint16 u16Length,
                            uint8 **ppResponse);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_HTTP_PARSER_H
