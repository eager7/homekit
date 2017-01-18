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
    E_HTTP_METHOD_POST,
    E_HTTP_METHOD_PUT,
    E_HTTP_METHOD_GET,
} teHttpMethod;

typedef struct {
    int  iHttpStatus;
    uint8 acHttpMethod[5];
    uint8 acDirectory[40];
    uint8 acHttpVersion[9];
    uint16 u16ContentLen;
    uint8 acContentType[40];
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
teHttpStatus eHttpParser(char *pBuf, uint16 u16Len, tsHttpEntry *psHttpEntry);
teHttpStatus eHttpResponse(int iSockFd, tsHttpEntry *psHttpEntry, char *pBuffer, uint16 u16Length);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_HTTP_PARSER_H
