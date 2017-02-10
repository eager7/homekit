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
#include <accessory.h>
#include "bonjour.h"
#include "mthread.h"
#include "pairing.h"
#include "http_handle.h"
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
static teBonjStatus eBonjourSocketInit(void);
static teBonjStatus eTextRecordFormat(tsBonjour *psBonjour);
static void *pvBonjourThreadHandle(void *psThreadInfoVoid);
static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags, DNSServiceErrorType errorCode,
                                const char *name, const char *regtype, const char *domain, void *context);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static tsBonjour sBonjour;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teBonjStatus eBonjourInit(tsProfile *psProfile, char *pcSetupCode)
{
    SRP_initialize_library();
    srand((unsigned int)time(NULL));

    memset(&sBonjour, 0, sizeof(sBonjour));
    sBonjour.psServiceName = BONJOUR_SERVER_TYPE;
    sBonjour.psHostName = NULL;
    sBonjour.u16Port = ACCESSORY_SERVER_PORT;
    sBonjour.psInstanceName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;
    sBonjour.pcSetupCode = pcSetupCode;

    sBonjour.sBonjourText.u64CurrentCfgNumber = 1;
    sBonjour.sBonjourText.u8FeatureFlag = 0x00; /* Supports HAP Pairing. This flag is required for all HomeKit accessories */
    sBonjour.sBonjourText.u64DeviceID = 0x03d224a1bd75;
    sBonjour.sBonjourText.psDeviceID = "12:10:34:23:51:13";
    sBonjour.sBonjourText.psModelName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;
    sBonjour.sBonjourText.auProtocolVersion[0] = 0x01;
    sBonjour.sBonjourText.auProtocolVersion[1] = 0x00;
    sBonjour.sBonjourText.u32iCurrentStaNumber = 4;
    sBonjour.sBonjourText.u8StatusFlag = 0x01;
    sBonjour.sBonjourText.eAccessoryCategoryID = psProfile->sAccessory.eAccessoryType;
    eTextRecordFormat(&sBonjour);

    CHECK_RESULT(eBonjourSocketInit(), E_BONJOUR_STATUS_OK, E_BONJOUR_STATUS_ERROR);

    DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&sBonjour.txtRecord), (const char*)TXTRecordGetBytesPtr(&sBonjour.txtRecord));
    DNSServiceErrorType  ret = DNSServiceRegister(&sBonjour.psDnsRef, 0, 0,
                                                  sBonjour.psInstanceName,
                                                  sBonjour.psServiceName, "",
                                                  sBonjour.psHostName, sBonjour.u16Port,
                                                  TXTRecordGetLength(&sBonjour.txtRecord),
                                                  TXTRecordGetBytesPtr(&sBonjour.txtRecord), reg_reply,NULL);
    TXTRecordDeallocate(&sBonjour.txtRecord);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
        return E_BONJOUR_STATUS_ERROR;
    }
    sBonjour.sThread.pvThreadData = psProfile;
    CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sThread, E_THREAD_DETACHED), E_THREAD_OK, E_BONJOUR_STATUS_ERROR);

    return E_BONJOUR_STATUS_OK;
}

teBonjStatus eBonjourFinished(tsProfile *psProfile)
{
    if(sBonjour.psDnsRef) DNSServiceRefDeallocate(sBonjour.psDnsRef);
    return E_BONJOUR_STATUS_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teBonjStatus eBonjourSocketInit(void)
{
    sBonjour.iSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sBonjour.iSocketFd == -1){
        ERR_vPrintln(T_TRUE, "socket create failed:[%s]", strerror(errno));
        return E_BONJOUR_STATUS_ERROR;
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
        return E_BONJOUR_STATUS_ERROR;
    }

    listen(sBonjour.iSocketFd, ACCESSORY_SERVER_LISTEN);

    struct sockaddr_in addr_t; socklen_t len = sizeof(addr_t);
    getsockname(sBonjour.iSocketFd, (struct sockaddr *)&addr_t, &len);
    sBonjour.u16Port = addr_t.sin_port;
    printf("port:%d\n", sBonjour.u16Port);

    return E_BONJOUR_STATUS_OK;
}

static void *pvBonjourThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    //tsProfile *psProfile = (tsProfile*)psThreadInfo->pvThreadData;
    psThreadInfo->eState = E_THREAD_RUNNING;

    fd_set fdSelect, fdTemp;
    FD_ZERO(&fdSelect);//Init fd
    FD_SET(sBonjour.iSocketFd, &fdSelect);//Add socket fd into select fd
    int iListenFD = 0;
    if(sBonjour.iSocketFd > iListenFD) {
        iListenFD = sBonjour.iSocketFd;
    }

    int iSockClient;
    static int iNumberClient = 0;
    while(psThreadInfo->eState == E_THREAD_RUNNING){
        fdTemp = fdSelect;  /* use temp value, because this value will be clear */
        DBG_vPrintln(DBG_BONJOUR, "select \n");
        int iResult = select(iListenFD + 1, &fdTemp, NULL, NULL, NULL);
        switch(iResult) {
            case 0:
                DBG_vPrintln(DBG_BONJOUR, "receive message time out \n");
                break;

            case -1:
                WAR_vPrintln(T_TRUE, "receive message error:%s \n", strerror(errno));
                break;

            default: {
                if( FD_ISSET(sBonjour.iSocketFd, &fdTemp) ){//there is client accept
                    DBG_vPrintln(DBG_BONJOUR, "A client connecting... \n");
                    if(iNumberClient >= MAX_NUMBER_CLIENT){
                        DBG_vPrintln(DBG_BONJOUR, "Client already connected full, don't allow to connected\n");
                        FD_CLR(sBonjour.iSocketFd, &fdSelect);//delete this Server from select set
                        break;
                    } else {
                        struct sockaddr_in client_addr;
                        memset(&client_addr, 0, sizeof(client_addr));
                        socklen_t client_len = sizeof(client_addr);
                        iSockClient = accept(sBonjour.iSocketFd, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
                        if(-1 == iSockClient){
                            ERR_vPrintln(T_TRUE, "Accept client failed:%s", strerror(errno));
                            break;
                        } else {
                            DBG_vPrintln(DBG_BONJOUR, "A client connected[%s]", inet_ntoa(client_addr.sin_addr));
                            FD_SET(iSockClient, &fdSelect);
                            if(iSockClient > iListenFD){
                                iListenFD = iSockClient;
                            }
                            iNumberClient ++;
                        }
                    }
                } else {    /* Client Communication */
                    if(FD_ISSET(iSockClient, &fdTemp)){
                        char buf[MABF] = {0};
                        ssize_t len = recv(iSockClient, buf, sizeof(buf), 0);
                        if(0 == len){
                            ERR_vPrintln(T_TRUE, "Close Client\n");
                            close(iSockClient);
                            FD_SET(sBonjour.iSocketFd, &fdSelect);//Add socket server fd into select fd
                            FD_CLR(iSockClient, &fdSelect);//delete this client from select set
                            iNumberClient --;
                        } else {
                            //DBG_vPrintln(DBG_BONJOUR, "RecvMsg[%d]\n%s", (int)len, buf);
                            tsHttpEntry sHttpEntry;
                            eHttpParser(buf, (uint16)len, &sHttpEntry);
                            if(strstr((char*)sHttpEntry.acDirectory, "pair-setup")){
                                DBG_vPrintln(DBG_BONJOUR, "IOS Device Pair Setup");
                                ePairSetup(iSockClient, &sBonjour, buf, (uint16)len);

                                //eTextRecordFormat(&sBonjour);
                                //DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&sBonjour.txtRecord), (const char*)TXTRecordGetBytesPtr(&sBonjour.txtRecord));
                                //DNSServiceErrorType  ret = DNSServiceUpdateRecord(sBonjour.psDnsRef, NULL, 0, TXTRecordGetLength(&sBonjour.txtRecord),
                                //                                                  TXTRecordGetBytesPtr(&sBonjour.txtRecord), 0);
                                //TXTRecordDeallocate(&sBonjour.txtRecord);
                                //if(ret){
                                //    ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
                                //}

                            } else if(strstr((char*)sHttpEntry.acDirectory, "pair-verify")){
                                DBG_vPrintln(DBG_BONJOUR, "IOS Device Pair Verify");

                            }

                            //TODO:HandleHomeKitMsg(buf);
                        }
                    }
                }
            }   break;
        }
    }
    DBG_vPrintln(DBG_BONJOUR, "pvBonjourThreadHandle Exit");
    vThreadFinish(psThreadInfo);

    return NULL;
}

static teBonjStatus eTextRecordFormat(tsBonjour *psBonjour)
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
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*5 & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*4 & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*3 & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*2 & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*1 & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>8*0 & 0xff));
    //TXTRecordSetValue(&psBonjour->txtRecord, "id", (uint8)strlen(temp_id), temp_id);
    TXTRecordSetValue(&psBonjour->txtRecord, "id", 17, "12:10:34:23:51:12");

    char temp_md[MIBF] = {0};
    sprintf(temp_md, "%s", psBonjour->sBonjourText.psModelName);
    TXTRecordSetValue(&psBonjour->txtRecord, "md", (uint8)strlen(temp_md), temp_md);

    char temp_pv[MIBF] = {0};
    sprintf(temp_pv, "%d.%d", psBonjour->sBonjourText.auProtocolVersion[0], psBonjour->sBonjourText.auProtocolVersion[1]);
    TXTRecordSetValue(&psBonjour->txtRecord, "pv", (uint8)strlen(temp_pv), temp_pv);

    char temp_scharp[MIBF] = {0};
    sprintf(temp_scharp, "%d", psBonjour->sBonjourText.u32iCurrentStaNumber);
    TXTRecordSetValue(&psBonjour->txtRecord, "s#", (uint8)strlen(temp_scharp), temp_scharp);

    char temp_sf[MIBF] = {0};
    sprintf(temp_sf, "%d", psBonjour->sBonjourText.u8StatusFlag);
    TXTRecordSetValue(&psBonjour->txtRecord, "sf", (uint8)strlen(temp_sf), temp_sf);

    char temp_ci[MIBF] = {0};
    sprintf(temp_ci, "%d", psBonjour->sBonjourText.eAccessoryCategoryID);
    TXTRecordSetValue(&psBonjour->txtRecord, "ci", (uint8)strlen(temp_ci), temp_ci);

    return E_BONJOUR_STATUS_OK;
}

static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags, DNSServiceErrorType errorCode,
                                const char *name, const char *regtype, const char *domain, void *context)
{
    (void)client;   // Unused
    (void)flags;    // Unused
    (void)context;  // Unused

    printf("Got a reply for %s.%s%s: ", name, regtype, domain);
    switch (errorCode)
    {
        case kDNSServiceErr_NoError:      printf("Name now registered and active\n"); break;
        case kDNSServiceErr_NameConflict: printf("Name in use, please choose another\n"); exit(-1);
        default:                          printf("Error %d\n", errorCode); return;
    }

}
