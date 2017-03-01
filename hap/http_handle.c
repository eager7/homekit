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
#include <accessory_type.h>
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
teHapStatus eHttpParser(teHttpMethod eMethod, const uint8 *pBuf, uint16 u16Len, tsHttpEntry *psHttpEntry)
{
    INF_vPrintln(DBG_HTTP, "--------Http Parser Package[%d]--------", u16Len);
    char auTemp[MABF] = {0};
    memcpy(auTemp, pBuf, sizeof(auTemp));

    CHECK_POINTER(psHttpEntry, E_HAP_STATUS_ERROR);
    char *psContentLen = NULL;
    char *psContentType = NULL;

    char *psHeader = strtok(auTemp, "\r\n");
    CHECK_POINTER(psHeader, E_HAP_STATUS_ERROR);
    char *psHost = strtok(NULL, "\r\n");
    CHECK_POINTER(psHost, E_HAP_STATUS_ERROR);
    if(eMethod == E_HTTP_POST || eMethod == E_HTTP_PUT){
        psContentLen = strtok(NULL, "\r\n");
        CHECK_POINTER(psContentLen, E_HAP_STATUS_ERROR);

        psContentType = strtok(NULL, "\r\n");
        CHECK_POINTER(psContentType, E_HAP_STATUS_ERROR);
    }

    char *psMethod = strtok(psHeader, " ");
    CHECK_POINTER(psMethod, E_HAP_STATUS_ERROR);
    memcpy(psHttpEntry->acHttpMethod, psMethod, sizeof(psHttpEntry->acHttpMethod));
    char *psDirectory = strtok(NULL, " ");
    CHECK_POINTER(psDirectory, E_HAP_STATUS_ERROR);
    memcpy(psHttpEntry->acDirectory, psDirectory, sizeof(psHttpEntry->acDirectory));
    char *psHttpVer = strtok(NULL, " ");
    CHECK_POINTER(psHttpVer, E_HAP_STATUS_ERROR);
    memcpy(psHttpEntry->acHttpVersion, psHttpVer, sizeof(psHttpEntry->acHttpVersion));
    DBG_vPrintln(DBG_HTTP, "Method:%s,Dir:%s,Version:%s", psHttpEntry->acHttpMethod, psHttpEntry->acDirectory, psHttpEntry->acHttpVersion);

    if(eMethod == E_HTTP_POST || eMethod == E_HTTP_PUT){
        char *psLen = strtok(psContentLen, ":");
        psLen = strtok(NULL, ":");
        CHECK_POINTER(psLen, E_HAP_STATUS_ERROR);
        psHttpEntry->u16ContentLen = (uint16)atoi(psLen);
        DBG_vPrintln(DBG_HTTP, "Len:%d", psHttpEntry->u16ContentLen);

        char *psType = strtok(psContentType, ":");
        psType = strtok(NULL, ":");
        CHECK_POINTER(psType, E_HAP_STATUS_ERROR);
        memcpy(psHttpEntry->acContentType, psType, sizeof(psHttpEntry->acContentType));
        DBG_vPrintln(DBG_HTTP, "Type:%s", psHttpEntry->acContentType);

        memcpy(psHttpEntry->acContentData, &auTemp[u16Len - psHttpEntry->u16ContentLen], psHttpEntry->u16ContentLen);
        PrintArray(DBG_HTTP, psHttpEntry->acContentData, psHttpEntry->u16ContentLen);
    }
    return E_HAP_STATUS_OK;
}

teHapStatus eHttpResponse(int iSockFd, tsHttpEntry *psHttpEntry, uint8 *pBuffer, uint16 u16Length)
{
    INF_vPrintln(DBG_HTTP, "--------Http Send Package[%d]--------", u16Length);

    uint8 temp[MABF] = {0}; int index = 0;
    memcpy(&temp[index], "HTTP/1.1 ", sizeof("HTTP/1.1 ") - 1);
    index += sizeof("HTTP/1.1 ") - 1;
    char auHttpStatus[5] = {0};
    sprintf(auHttpStatus, "%d", psHttpEntry->iHttpStatus);
    memcpy(&temp[index], auHttpStatus, strlen(auHttpStatus));
    index += strlen(auHttpStatus);
    memcpy(&temp[index], " OK\r\n", sizeof(" OK\r\n") - 1);index += sizeof(" OK\r\n") - 1;

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
        return E_HAP_STATUS_ERROR;
    }
    return E_HAP_STATUS_OK;
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

tsHttpEntry *psHttpParser(const uint8 *pBuf, uint16 u16Len)
{
    INF_vPrintln(1, "--------Http Parser Package[%d]--------", u16Len);

    tsHttpEntry *psHttpEntry = (tsHttpEntry*)malloc(sizeof(tsHttpEntry));
    CHECK_POINTER(psHttpEntry, NULL);
    memset(psHttpEntry, 0, sizeof(tsHttpEntry));

    int iNumber = 0,  iLen = 0;
    size_t offset = 0;
    char auHeader[MIBF] = {0}, auHost[MIBF] = {0}, auContentLen[MIBF] = {0}, auContentType[MIBF] = {0};
    for (size_t i = 0; i < u16Len; ++i) {
        if(pBuf[i] == '\r' && pBuf[i+1] == '\n'){
            iNumber++;
            switch (iNumber){
                case 1:{
                    memcpy(auHeader, &pBuf[0], i);
                    sscanf(auHeader, "%s %s %s", psHttpEntry->acHttpMethod, psHttpEntry->acDirectory, psHttpEntry->acHttpVersion);
                    DBG_vPrintln(DBG_HTTP, "[%s][%s][%s]\n", psHttpEntry->acHttpMethod, psHttpEntry->acDirectory, psHttpEntry->acHttpVersion);
                    offset = i + 2;
                }
                    break;
                case 2:{
                    memcpy(auHost, &pBuf[offset], i - offset);
                    DBG_vPrintln(DBG_HTTP, "[%s]\n\n", auHost);
                    offset = i + 2;
                    if(!strstr((char*)pBuf, "Content-Length:")){
                        return psHttpEntry;
                    }
                }
                    break;
                case 3:{
                    memcpy(auContentLen, &pBuf[offset], i - offset);
                    sscanf(auContentLen, "Content-Length: %d", (int*)&psHttpEntry->u16ContentLen);
                    DBG_vPrintln(DBG_HTTP, "[%d]\n\n", psHttpEntry->u16ContentLen);
                    offset = i +2 ;
                }
                    break;
                case 4:{
                    memcpy(auContentType, &pBuf[offset], i - offset);
                    sscanf(auContentType, "Content-Type: %s", psHttpEntry->acContentType);
                    DBG_vPrintln(DBG_HTTP, "[%s]\n\n", psHttpEntry->acContentType);
                    offset = i + 4;
                    memcpy(psHttpEntry->acContentData, &pBuf[offset], u16Len - offset);
                    psHttpEntry->u16ContentLen = u16Len - (uint16)offset;
                    PrintArray(DBG_HTTP, psHttpEntry->acContentData, psHttpEntry->u16ContentLen);
                    return psHttpEntry;
                }
                default:
                    break;
            }
        }
    }

    return psHttpEntry;
}

uint16 u16HttpFormat(teHttpCode eStatus, const char *psType, const uint8 *pBuffer, uint16 u16Length, uint8 **ppResponse)
{
    char temp[MIBF] = {0};
    int iLen = snprintf(temp, 256, "HTTP/1.1 %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", eStatus, psType, u16Length);
    INF_vPrintln(DBG_HTTP, "Http Response:\n%s", temp);
    PrintArray(DBG_HTTP, pBuffer, u16Length);
    uint8 *psBuffer = (uint8*)malloc((size_t)(iLen + u16Length));
    memset(psBuffer, 0, (size_t)(iLen + u16Length));
    snprintf(psBuffer, (size_t)iLen+1, "HTTP/1.1 %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", eStatus, psType, u16Length);
    memcpy(&psBuffer[iLen], pBuffer, u16Length);
    *ppResponse = psBuffer;
    uint16 u16Len = (uint16)iLen + u16Length;
    return u16Len;
}

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

