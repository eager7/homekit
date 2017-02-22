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
#include "http_handle.h"
#include "tlv.h"
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

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
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
    eDecryptedHttpMessage(auBuffer, iLen);

    return E_IP_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teIpStatus eHapHandlePackage(char *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    tsHttpEntry sHttpEntry;
    eHttpParser(psBuffer, (uint16)iLen, &sHttpEntry);
    DBG_vPrintln(DBG_IP, "Data Len:%d", sHttpEntry.u16ContentLen);
    if(strstr((char*)sHttpEntry.acDirectory, "pair-setup")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Setup");
        ePairSetup(iSocketFd, psBonjour, psBuffer, (uint16)iLen);
        //eTextRecordFormat(&sBonjour);
        //DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&sBonjour.txtRecord), (const char*)TXTRecordGetBytesPtr(&sBonjour.txtRecord));
        //DNSServiceErrorType  ret = DNSServiceUpdateRecord(sBonjour.psDnsRef, NULL, 0, TXTRecordGetLength(&sBonjour.txtRecord),
        //                                                  TXTRecordGetBytesPtr(&sBonjour.txtRecord), 0);
        //TXTRecordDeallocate(&sBonjour.txtRecord);
        //if(ret){
        //    ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
        //}
    } else if(strstr((char*)sHttpEntry.acDirectory, "pair-verify")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Verify");
        ePairVerify(iSocketFd, psBonjour, psBuffer, (uint16)iLen);
        eHapHandleAccessoryRequest(iSocketFd);
    }

    //TODO:HandleHomeKitMsg(buf);
    return E_IP_STATUS_OK;
}


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
