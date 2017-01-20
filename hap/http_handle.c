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
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "http_handle.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_HTTP 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

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
teHttpStatus eHttpParser(char *pBuf, uint16 u16Len, tsHttpEntry *psHttpEntry)
{
    INF_vPrintln(DBG_HTTP, "--------Http Parser Package--------");
    CHECK_POINTER(psHttpEntry, E_HTTP_PARSER_ERROR);

    char *psHeader = strtok(pBuf, "\r\n");
    CHECK_POINTER(psHeader, E_HTTP_PARSER_ERROR);

    char *psHost = strtok(NULL, "\r\n");
    CHECK_POINTER(psHost, E_HTTP_PARSER_ERROR);

    char *psContentLen = strtok(NULL, "\r\n");
    CHECK_POINTER(psContentLen, E_HTTP_PARSER_ERROR);

    char *psContentType = strtok(NULL, "\r\n");
    CHECK_POINTER(psContentType, E_HTTP_PARSER_ERROR);

    char *psMethod = strtok(psHeader, " ");
    CHECK_POINTER(psMethod, E_HTTP_PARSER_ERROR);
    memcpy(psHttpEntry->acHttpMethod, psMethod, sizeof(psHttpEntry->acHttpMethod));
    char *psDirectory = strtok(NULL, " ");
    CHECK_POINTER(psDirectory, E_HTTP_PARSER_ERROR);
    memcpy(psHttpEntry->acDirectory, psDirectory, sizeof(psHttpEntry->acDirectory));
    char *psHttpVer = strtok(NULL, " ");
    CHECK_POINTER(psHttpVer, E_HTTP_PARSER_ERROR);
    memcpy(psHttpEntry->acHttpVersion, psHttpVer, sizeof(psHttpEntry->acHttpVersion));
    DBG_vPrintln(DBG_HTTP, "Method:%s,Dir:%s,Version:%s", psHttpEntry->acHttpMethod, psHttpEntry->acDirectory, psHttpEntry->acHttpVersion);

    char *psLen = strtok(psContentLen, ":");
    psLen = strtok(NULL, ":");
    CHECK_POINTER(psLen, E_HTTP_PARSER_ERROR);
    psHttpEntry->u16ContentLen = (uint16)atoi(psLen);
    DBG_vPrintln(DBG_HTTP, "Len:%d", psHttpEntry->u16ContentLen);

    char *psType = strtok(psContentType, ":");
    psType = strtok(NULL, ":");
    CHECK_POINTER(psType, E_HTTP_PARSER_ERROR);
    memcpy(psHttpEntry->acContentType, psType, sizeof(psHttpEntry->acContentType));
    DBG_vPrintln(DBG_HTTP, "Type:%s", psHttpEntry->acContentType);

    memcpy(psHttpEntry->acContentData, &pBuf[u16Len - psHttpEntry->u16ContentLen], psHttpEntry->u16ContentLen);
    if(DBG_HTTP){
        for (int i = 0; i < psHttpEntry->u16ContentLen; ++i) {
            printf("0x%02x,", psHttpEntry->acContentData[i]);
        }printf("\n");
    }
    return E_HTTP_PARSER_OK;
}

teHttpStatus eHttpResponse(int iSockFd, tsHttpEntry *psHttpEntry, uint8 *pBuffer, uint16 u16Length)
{
    INF_vPrintln(DBG_HTTP, "--------Http Send Package--------");
    if(DBG_HTTP){
        for (int i = 0; i < u16Length; ++i) {
            printf("0x%02x,", pBuffer[i]);
        }printf("\nlen[%d]\n", u16Length);
    }

    char temp[MABF] = {0};
    snprintf(temp, sizeof(temp), "HTTP/1.1 %d OK\r\nContent-Type:%s\r\nContent-Length: %d\r\n\r\n",
             psHttpEntry->iHttpStatus, psHttpEntry->acContentType, u16Length);
    ssize_t ret = send(iSockFd, temp, strlen(temp), 0);
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "send failed:%s", strerror(errno));
    }
    ret = send(iSockFd, pBuffer, u16Length, 0);
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "send failed:%s", strerror(errno));
    }
    return E_HTTP_PARSER_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

