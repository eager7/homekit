/****************************************************************************
 *
 * MODULE:             ip.c
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

#include "ip.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_IP 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern tsController sController;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static void vUpdateConfiguration(tsBonjour *psBonjour) {
    psBonjour->sBonjourText.u64CurrentCfgNumber++;
    eTextRecordFormat(psBonjour);
    DBG_vPrintln(DBG_IP, "%d-%s", TXTRecordGetLength(&psBonjour->txtRecord), (const char*)TXTRecordGetBytesPtr(&psBonjour->txtRecord));
    DNSServiceErrorType  ret = DNSServiceUpdateRecord(psBonjour->psDnsRef, NULL, 0,
                                                      TXTRecordGetLength(&psBonjour->txtRecord), TXTRecordGetBytesPtr(&psBonjour->txtRecord), 0);
    TXTRecordDeallocate(&psBonjour->txtRecord);
    if(ret){
        ERR_vPrintln(DBG_IP, "DNSServiceUpdateRecord Failed:%d", ret);
        return ;
    }
}

teIpStatus eHapHandleAccessoryRequest(int iSocketFd)
{
    char auBuffer[MABF] = {0};
    int iLen = (int)recv(iSocketFd, auBuffer, sizeof(auBuffer), 0);
    if(-1 == iLen){
        ERR_vPrintln(T_TRUE, "Recv Socket Failed");
        return E_IP_STATUS_ERROR;
    }
    //tsHttpEntry sHttpEntry; memset(&sHttpEntry, 0, sizeof(sHttpEntry));
    //CHECK_RESULT(eHttpParser(auBuffer, (uint16)iLen, &sHttpEntry), E_HTTP_PARSER_OK, E_IP_STATUS_ERROR);
    //eDecryptedHttpMessage(auBuffer, iLen);

    return E_IP_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsIpMessage *psIpResponseNew()
{
    tsIpMessage *psIpMsg = (tsIpMessage*)malloc(sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    memset(psIpMsg, 0, sizeof(tsIpMessage));
    psIpMsg->psTlvPackage = psTlvPackageNew();
    if(psIpMsg->psTlvPackage == NULL){
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}
tsIpMessage *psIpMessageFormat(uint8 *psBuffer, uint16 u16Len)
{
    tsIpMessage *psIpMsg = (tsIpMessage*)malloc(sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    memset(psIpMsg, 0, sizeof(tsIpMessage));
    eHttpParser(psBuffer, u16Len, &psIpMsg->sHttp);
    psIpMsg->psTlvPackage = psTlvPackageFormat(psIpMsg->sHttp.acContentData, psIpMsg->sHttp.u16ContentLen);
    if(psIpMsg->psTlvPackage == NULL){
        ERR_vPrintln(T_TRUE, "psTlvPackageFormat Failed");
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}

teIpStatus eIpMessageRelease(tsIpMessage *psIpMsg)
{
    eTlvPackageRelease(psIpMsg->psTlvPackage);
    FREE(psIpMsg);
    return E_IP_STATUS_OK;
}

teIpStatus eHapHandlePackage(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    tsHttpEntry sHttpEntry;
    eHttpParser(psBuffer, (uint16)iLen, &sHttpEntry);

    DBG_vPrintln(DBG_IP, "Data Len:%d", sHttpEntry.u16ContentLen);
    if(strstr((char*)sHttpEntry.acDirectory, "pair-setup")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Setup");
        eHandlePairSetup(psBuffer, iLen, iSocketFd, psBonjour);
        vUpdateConfiguration(psBonjour);
    } else if(strstr((char*)sHttpEntry.acDirectory, "pair-verify")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Verify");
        eHandlePairVerify(psBuffer, iLen, iSocketFd, psBonjour);
        eHandleAccessoryRequest(psBuffer, iLen, iSocketFd, psBonjour);
    } else if (strstr((char*)sHttpEntry.acDirectory, "identify")){
        close(iSocketFd);
    }
    return E_IP_STATUS_OK;
}