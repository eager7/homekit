/****************************************************************************
 *
 * MODULE:             bonjour.c
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
#include <time.h>
#include <profile.h>
#include "bonjour.h"
#include "pairing.h"
#include "controller.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_BONJOUR 1
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
static tsBonjour sBonjour;
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teHapStatus eBonjourSocketInit(void)
{
    sBonjour.iSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sBonjour.iSocketFd == -1){
        ERR_vPrintln(T_TRUE, "socket create failed:[%s]", strerror(errno));
        return E_HAP_STATUS_ERROR;
    }

    int re = 1;
    setsockopt(sBonjour.iSocketFd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re));

    struct sockaddr_in addr_sever;
    memset(&addr_sever, 0, sizeof(addr_sever));
    addr_sever.sin_family = AF_INET;
    addr_sever.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_sever.sin_port = htons(sBonjour.u16Port);
    int ret = bind(sBonjour.iSocketFd, (struct sockaddr*)&addr_sever, sizeof(addr_sever));
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "bind address failed:[%s]", strerror(errno));
        close(sBonjour.iSocketFd);
        return E_HAP_STATUS_ERROR;
    }
    listen(sBonjour.iSocketFd, ACCESSORY_SERVER_LISTEN);

    struct sockaddr_in addr_t; socklen_t len = sizeof(addr_t);
    getsockname(sBonjour.iSocketFd, (struct sockaddr *)&addr_t, &len);
    sBonjour.u16Port = addr_t.sin_port;
    printf("port:%d\n", sBonjour.u16Port);

    return E_HAP_STATUS_OK;
}
static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags, DNSServiceErrorType errorCode, const char *name, const char *regtype, const char *domain, void *context)
{
    (void)client;   // Unused
    (void)flags;    // Unused
    (void)context;  // Unused
    printf("Got a reply for %s.%s%s: ", name, regtype, domain);
    switch (errorCode) {
        case kDNSServiceErr_NoError:      printf("Name now registered and active\n"); break;
        case kDNSServiceErr_NameConflict: printf("Name in use, please choose another\n"); exit(-1);
        default:                          printf("Error %d\n", errorCode); return;
    }
}
static teHapStatus eTextRecordFormat(tsBonjour *psBonjour)
{
    TXTRecordCreate(&psBonjour->txtRecord, 0, NULL);

    char temp_csharp[MIBF] = {0};
    sprintf(temp_csharp, "%llu", psBonjour->sBonjourText.u64CurrentCfgNumber);
    TXTRecordSetValue(&psBonjour->txtRecord, "c#", (uint8)strlen(temp_csharp), temp_csharp);    //Configuration Number

    char temp_ff[MIBF] = {0};
    sprintf(temp_ff, "%d", psBonjour->sBonjourText.u8FeatureFlag);
    TXTRecordSetValue(&psBonjour->txtRecord, "ff", 1, temp_ff);

    char temp_id[MIBF] = {0};
    sprintf(temp_id, "%02x:%02x:%02x:%02x:%02x:%02x",
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*5) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*4) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*3) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*2) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*1) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*0) & 0xff));
    TXTRecordSetValue(&psBonjour->txtRecord, "id", (uint8)strlen(temp_id), temp_id);
    memcpy(psBonjour->sBonjourText.psDeviceID, temp_id, sizeof(psBonjour->sBonjourText.psDeviceID));

    char temp_md[MIBF] = {0};
    sprintf(temp_md, "%s", psBonjour->sBonjourText.psModelName);
    TXTRecordSetValue(&psBonjour->txtRecord, "md", (uint8)strlen(temp_md), temp_md);

    char temp_pv[MIBF] = {0};
    sprintf(temp_pv, "%d.%d", psBonjour->sBonjourText.auProtocolVersion[0], psBonjour->sBonjourText.auProtocolVersion[1]);
    TXTRecordSetValue(&psBonjour->txtRecord, "pv", (uint8)strlen(temp_pv), temp_pv);

    char temp_scharp[MIBF] = {0};
    sprintf(temp_scharp, "%d", psBonjour->sBonjourText.u32CurrentStaNumber);
    TXTRecordSetValue(&psBonjour->txtRecord, "s#", (uint8)strlen(temp_scharp), temp_scharp);

    char temp_sf[MIBF] = {0};
    sprintf(temp_sf, "%d", psBonjour->sBonjourText.u8StatusFlag);
    TXTRecordSetValue(&psBonjour->txtRecord, "sf", (uint8)strlen(temp_sf), temp_sf);

    char temp_ci[MIBF] = {0};
    sprintf(temp_ci, "%d", psBonjour->sBonjourText.eAccessoryCategoryID);
    TXTRecordSetValue(&psBonjour->txtRecord, "ci", (uint8)strlen(temp_ci), temp_ci);

    return E_HAP_STATUS_OK;
}
static teHapStatus eBonjourRegister()
{
    tsBonjour *psBonjour = &sBonjour;
    eTextRecordFormat(psBonjour);
    DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&psBonjour->txtRecord), (const char*)TXTRecordGetBytesPtr(&psBonjour->txtRecord));
    DNSServiceErrorType  ret = DNSServiceRegister(&psBonjour->psDnsRef, 0, 0, psBonjour->sBonjourText.psModelName,
                                                  psBonjour->psServiceName, "", psBonjour->psHostName, psBonjour->u16Port,
                                                  TXTRecordGetLength(&psBonjour->txtRecord), TXTRecordGetBytesPtr(&psBonjour->txtRecord), reg_reply,NULL);
    TXTRecordDeallocate(&psBonjour->txtRecord);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
        return E_HAP_STATUS_ERROR;
    }

    return E_HAP_STATUS_OK;
}
static teHapStatus eBonjourUpdate()
{
    tsBonjour *psBonjour = &sBonjour;
    psBonjour->sBonjourText.u64CurrentCfgNumber++;
    eTextRecordFormat(psBonjour);
    DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&psBonjour->txtRecord), (const char*)TXTRecordGetBytesPtr(&psBonjour->txtRecord));
    DNSServiceErrorType  ret = DNSServiceUpdateRecord(psBonjour->psDnsRef, NULL, 0,
                                                      TXTRecordGetLength(&psBonjour->txtRecord), TXTRecordGetBytesPtr(&psBonjour->txtRecord), 0);
    TXTRecordDeallocate(&psBonjour->txtRecord);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceUpdateRecord Failed:%d", ret);
        return E_HAP_STATUS_ERROR;
    }
    return E_HAP_STATUS_OK;
}
static void *pvBonjourThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    tsProfile *psProfile = (tsProfile*)psThreadInfo->pvThreadData;
    psThreadInfo->eState = E_THREAD_RUNNING;

    fd_set fdSelect, fdTemp;
    FD_ZERO(&fdSelect);//Init fd
    FD_SET(sBonjour.iSocketFd, &fdSelect);//Add socket fd into select fd
    int iListenFD = 0;
    if(sBonjour.iSocketFd > iListenFD) {
        iListenFD = sBonjour.iSocketFd;
    }

    int iSockFd = 0;
    int iLen = 0;
    uint8 auBuffer[MABF] = {0};
    while(psThreadInfo->eState == E_THREAD_RUNNING){
        fdTemp = fdSelect;  /* use temp value, because this value will be clear */
        DBG_vPrintln(DBG_BONJOUR, "========select.... \n");
        int iResult = select(iListenFD + 1, &fdTemp, NULL, NULL, NULL);
        switch(iResult) {
            case 0:
                WAR_vPrintln(DBG_BONJOUR, "receive message time out \n");
                break;
            case -1:
                WAR_vPrintln(DBG_BONJOUR, "receive message error:%s \n", strerror(errno));
                break;
            default: {
                if( FD_ISSET(sBonjour.iSocketFd, &fdTemp) ){//there is client accept
                    DBG_vPrintln(DBG_BONJOUR, "A client connecting... \n");
                    struct sockaddr_in client_addr;
                    memset(&client_addr, 0, sizeof(client_addr));
                    socklen_t client_len = sizeof(client_addr);
                    iSockFd = accept(sBonjour.iSocketFd, (struct sockaddr*)&client_addr, &client_len);
                    if(-1 == iSockFd){
                        ERR_vPrintln(T_TRUE, "Accept client failed:%s", strerror(errno));
                    } else {
                        tsController *psControllerClient = psControllerListAdd(&sControllerHead);
                        DBG_vPrintln(DBG_BONJOUR, "A client connected[%s]", inet_ntoa(client_addr.sin_addr));
                        psControllerClient->iSocketFd = iSockFd;
                        FD_SET(psControllerClient->iSocketFd, &fdSelect);
                        if(psControllerClient->iSocketFd > iListenFD){
                            iListenFD = psControllerClient->iSocketFd;
                        }
                        sBonjour.u8NumberController ++;
                    }
                } else {    /* Client Communication */
                    tsController *psControllerTemp = NULL, *psController = NULL;
                    eLockLock(&sControllerHead.mutex);
                    dl_list_for_each_safe(psController, psControllerTemp, &sControllerHead.list, tsController, list)
                    {
                        if(FD_ISSET(psController->iSocketFd, &fdTemp)){
                            DBG_vPrintln(DBG_BONJOUR, "A client receive data");
                            bool_t bDisconnected = T_FALSE;
                            iLen = (int)recv(psController->iSocketFd, auBuffer, sizeof(auBuffer), 0);
                            if(0 == iLen){
                                bDisconnected = T_TRUE;
                            } else {
                                tsHttpEntry *psHttpEntry = psHttpParser(auBuffer, (uint16)iLen);
                                if(strstr((char*)psHttpEntry->acDirectory, HTTP_URL_PAIR_SETUP)) {
                                    DBG_vPrintln(DBG_BONJOUR, "IOS Device Pair Setup");
                                    if(E_HAP_STATUS_SOCKET_ERROR == eHandlePairSetup(auBuffer, iLen, psController->iSocketFd, &sBonjour)){
                                        bDisconnected = T_TRUE;
                                    }
                                } else if(strstr((char*)psHttpEntry->acDirectory, HTTP_URL_PAIR_VERIFY)) {
                                    DBG_vPrintln(DBG_BONJOUR, "IOS Device Pair Verify");
                                    if(E_HAP_STATUS_SOCKET_ERROR == eHandlePairVerify(auBuffer, iLen, psController, &sBonjour)){
                                        bDisconnected = T_TRUE;
                                    }
                                } else if(strstr((char*)psHttpEntry->acDirectory, HTTP_URL_IDENTIFY)) {
                                    if(bAccessoryIsPaired()){
                                        char *psHttpResp = "HTTP/1.1 400 Bad Request\r\n"
                                                "Content-Type: application/hap+json\r\n"
                                                "Content-Length: 21\r\n\r\n"
                                                "{\"status\" : -70401}";
                                        send(psController->iSocketFd, psHttpResp, strlen(psHttpResp), 0);
                                    } else {
                                        psProfile->eIdentify();
                                        char *psHttpResp = "HTTP/1.1 204 OK\r\n"
                                        "Content-Type: application/hap+json\r\n"
                                        "Content-Length: 0\r\n\r\n";
                                        send(psController->iSocketFd, psHttpResp, strlen(psHttpResp), 0);
                                    }
                                } else {
                                    DBG_vPrintln(DBG_BONJOUR, "Handle Controller Request");
                                    eLockLock(&psController->mutex);
                                    uint8 auHttpData[MMBF] = {0};
                                    uint16 u16MsgLen = 0;
                                    eDecryptedMessageWithLen(auBuffer, (uint16)iLen, psController, auHttpData, &u16MsgLen);
                                    psController->u64NumberReceive++;
                                    uint8 *psRetData = NULL;
                                    uint16 u16RetLen = 0;
                                    eHandleAccessoryPackage(psProfile, auHttpData, u16MsgLen, &psRetData, &u16RetLen, psController);
                                    eLockunLock(&psController->mutex);
                                }
                                FREE(psHttpEntry);
                            }
                            if(bDisconnected == T_TRUE){
                                ERR_vPrintln(T_TRUE, "Close Client:%d\n", psController->iSocketFd);
                                close(psController->iSocketFd);
                                FD_CLR(psController->iSocketFd, &fdSelect);//delete this client from select set
                                eControllerListDel(psController);
                                sBonjour.u8NumberController --;
                            }
                            break; /* dl_list_for_each_safe */
                        }
                    }
                    eLockunLock(&sControllerHead.mutex);
                }
            }   break; /* default */
        }
    }
    DBG_vPrintln(DBG_BONJOUR, "pvBonjourThreadHandle Exit");
    vThreadFinish(psThreadInfo);
    return NULL;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus eBonjourInit(tsProfile *psProfile, char *psSetupCode)
{
    SRP_initialize_library();
    srand((unsigned int)time(NULL));
    memset(&sBonjour, 0, sizeof(sBonjour));
    sBonjour.eBonjourUpdate = eBonjourUpdate;
    sBonjour.eBonjourRegister = eBonjourRegister;
    ePairingInit();
    eControllerInit();

    sBonjour.psServiceName = BONJOUR_SERVER_TYPE;
    sBonjour.pcSetupCode = psSetupCode;
    sBonjour.sBonjourText.u64CurrentCfgNumber = 1;
    sBonjour.sBonjourText.u8FeatureFlag = 0x00; /* Supports HAP Pairing. This flag is required for all HomeKit accessories */
    sBonjour.sBonjourText.u64DeviceID = psProfile->psAccessory->u64DeviceID;
    sBonjour.sBonjourText.psModelName = psAccessoryGetCharacterByType(psProfile->psAccessory, E_CHARACTERISTIC_NAME)->uValue.psData;
    sBonjour.sBonjourText.auProtocolVersion[0] = 0x01;
    sBonjour.sBonjourText.auProtocolVersion[1] = 0x00;
    sBonjour.sBonjourText.u32CurrentStaNumber = 4;
    sBonjour.sBonjourText.u8StatusFlag = 0x00;
    sBonjour.sBonjourText.eAccessoryCategoryID = psProfile->psAccessory->eAccessoryType;

    CHECK_RESULT(eBonjourSocketInit(), E_HAP_STATUS_OK, E_HAP_STATUS_ERROR);
    CHECK_RESULT(sBonjour.eBonjourRegister(), E_HAP_STATUS_OK, E_HAP_STATUS_ERROR);
    DBG_vPrintln(DBG_BONJOUR, "DNSServiceRegister Successful");

    psProfile->psBonjour = (void*)&sBonjour;
    sBonjour.sBonjourThread.pvThreadData = psProfile;
    CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sBonjourThread, E_THREAD_JOINABLE), E_THREAD_OK, E_HAP_STATUS_ERROR);

    return E_HAP_STATUS_OK;
}

teHapStatus eBonjourFinished()
{
    eThreadStop(&sBonjour.sBonjourThread);
    SRP_finalize_library();
    eControllerFinished();
    if(sBonjour.psDnsRef) DNSServiceRefDeallocate(sBonjour.psDnsRef);
    ePairingFinished();
    return E_HAP_STATUS_OK;
}


