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
#define DBG_HTTP 0
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
teHttpStatus eHttpParser(teHttpMethod eMethod, const uint8 *pBuf, uint16 u16Len, tsHttpEntry *psHttpEntry)
{
    INF_vPrintln(DBG_HTTP, "--------Http Parser Package[%d]--------", u16Len);
    char auTemp[MABF] = {0};
    memcpy(auTemp, pBuf, sizeof(auTemp));

    CHECK_POINTER(psHttpEntry, E_HTTP_PARSER_ERROR);
    char *psContentLen = NULL;
    char *psContentType = NULL;

    char *psHeader = strtok(auTemp, "\r\n");
    CHECK_POINTER(psHeader, E_HTTP_PARSER_ERROR);
    char *psHost = strtok(NULL, "\r\n");
    CHECK_POINTER(psHost, E_HTTP_PARSER_ERROR);
    if(eMethod == E_HTTP_POST || eMethod == E_HTTP_PUT){
        psContentLen = strtok(NULL, "\r\n");
        CHECK_POINTER(psContentLen, E_HTTP_PARSER_ERROR);

        psContentType = strtok(NULL, "\r\n");
        CHECK_POINTER(psContentType, E_HTTP_PARSER_ERROR);
    }

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

    if(eMethod == E_HTTP_POST || eMethod == E_HTTP_PUT){
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

        memcpy(psHttpEntry->acContentData, &auTemp[u16Len - psHttpEntry->u16ContentLen], psHttpEntry->u16ContentLen);
        PrintArray(DBG_HTTP, psHttpEntry->acContentData, psHttpEntry->u16ContentLen);
    }
    return E_HTTP_PARSER_OK;
}

teHttpStatus eHttpResponse(int iSockFd, tsHttpEntry *psHttpEntry, uint8 *pBuffer, uint16 u16Length)
{
    INF_vPrintln(DBG_HTTP, "--------Http Send Package[%d]--------", u16Length);

    uint8 temp[MABF] = {0}; int index = 0;
    memcpy(&temp[index], "HTTP/1.1 ", sizeof("HTTP/1.1 ") - 1);
    index += sizeof("HTTP/1.1 ") - 1;
    char auHttpStatus[5] = {0};
    sprintf(auHttpStatus, "%d", psHttpEntry->iHttpStatus);
    memcpy(&temp[index], auHttpStatus, strlen(auHttpStatus));
    index += strlen(auHttpStatus);

    memcpy(&temp[index], " OK\r\n", sizeof(" OK\r\n") - 1);
    index += sizeof(" OK\r\n") - 1;

    memcpy(&temp[index], "Content-Type:", sizeof("Content-Type:") - 1);
    index += sizeof("Content-Type:") - 1;
    memcpy(&temp[index], psHttpEntry->acContentType, strlen((char*)psHttpEntry->acContentType));
    index += strlen((char*)psHttpEntry->acContentType);
    memcpy(&temp[index], "\r\n", sizeof("\r\n") - 1);
    index += sizeof("\r\n") - 1;
    memcpy(&temp[index], "Content-Length: ", sizeof("Content-Length: ") - 1);
    index += sizeof("Content-Length: ") - 1;
    char auHttpLen[5] = {0};
    sprintf(auHttpLen, "%d", u16Length);
    memcpy(&temp[index], auHttpLen, strlen(auHttpLen));
    index += strlen(auHttpLen);
    memcpy(&temp[index], "\r\n\r\n", sizeof("\r\n\r\n") - 1);
    index += sizeof("\r\n\r\n") - 1;
    INF_vPrintln(DBG_HTTP, "%s", temp);
    memcpy(&temp[index], pBuffer, u16Length);
    index += u16Length;

    INF_vPrintln(DBG_HTTP, "%d", index);
    PrintArray(DBG_HTTP, temp, index);
    ssize_t ret = send(iSockFd, temp, index, 0);
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "send failed:%s", strerror(errno));
        return E_HTTP_PARSER_ERROR;
    }
    return E_HTTP_PARSER_OK;
}

uint16 u16HttpMessageFormat(int iStatus, const char *psContent, const char *pBuffer, uint16 u16Length, uint8 **ppResponse)
{
    INF_vPrintln(DBG_HTTP, "--------Http Send Package[%d]--------", u16Length);

    uint8 *temp = malloc(MABF);

    int index = 0;
    memcpy(&temp[index], "HTTP/1.1 ", sizeof("HTTP/1.1 ") - 1);
    index += sizeof("HTTP/1.1 ") - 1;

    char auHttpStatus[5] = {0};
    sprintf(auHttpStatus, "%d", iStatus);
    memcpy(&temp[index], auHttpStatus, strlen(auHttpStatus));
    index += strlen(auHttpStatus);

    memcpy(&temp[index], " OK\r\n", sizeof(" OK\r\n") - 1);
    index += sizeof(" OK\r\n") - 1;

    memcpy(&temp[index], "Content-Type: ", sizeof("Content-Type: ") - 1);
    index += sizeof("Content-Type: ") - 1;

    memcpy(&temp[index], psContent, strlen(psContent));
    index += strlen(psContent);

    memcpy(&temp[index], "\r\n", sizeof("\r\n") - 1);
    index += sizeof("\r\n") - 1;

    memcpy(&temp[index], "Content-Length: ", sizeof("Content-Length: ") - 1);
    index += sizeof("Content-Length: ") - 1;

    char auHttpLen[5] = {0};
    sprintf(auHttpLen, "%d", u16Length);
    memcpy(&temp[index], auHttpLen, strlen(auHttpLen));
    index += strlen(auHttpLen);

    memcpy(&temp[index], "\r\n\r\n", sizeof("\r\n\r\n") - 1);
    index += sizeof("\r\n\r\n") - 1;
    uint16 u16Ret = (uint16)index;
    if(u16Length)
        memcpy(&temp[index], pBuffer, u16Length);
    index += u16Length;

    if(ppResponse) *ppResponse = temp;
    return u16Ret;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

