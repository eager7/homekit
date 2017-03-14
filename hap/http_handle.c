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
tsHttpEntry *psHttpParser(const uint8 *pBuf, uint16 u16Len)
{
    INF_vPrintln(1, "--------Http Parser Package[%d]--------", u16Len);

    tsHttpEntry *psHttpEntry = (tsHttpEntry*)calloc(1, sizeof(tsHttpEntry));
    CHECK_POINTER(psHttpEntry, NULL);

    int iNumber = 0;
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

uint16 u16HttpFormat(teHttpCode eStatus, const char *psProtocol, const char *psType, const uint8 *pBuffer, uint16 u16Length,
                     uint8 **ppResponse)
{
    char temp[MIBF] = {0};
    int iLen = snprintf(temp, 256, "%s %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", psProtocol, eStatus, psType, u16Length);
    INF_vPrintln(DBG_HTTP, "Http Response:\n%s", temp);
    PrintArray(DBG_HTTP, pBuffer, u16Length);
    uint8 *psBuffer = (uint8*)calloc(1, (size_t)(iLen + u16Length));
    snprintf((char*)psBuffer, (size_t)iLen+1, "%s %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", psProtocol, eStatus, psType, u16Length);
    memcpy(&psBuffer[iLen], pBuffer, u16Length);
    *ppResponse = psBuffer;
    uint16 u16Len = (uint16)iLen + u16Length;
    return u16Len;
}

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

