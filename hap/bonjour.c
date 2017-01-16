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
#include <profile.h>
#include <accessory.h>
#include "bonjour.h"
#include "mthread.h"
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
static teBonjStatus eBonjourMdnsInit(void);
static TXTRecordRef TextRecordFormat(tsBonjourText *psBonjourText);
static void *pvBonjourThreadHandle(void *psThreadInfoVoid);
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
teBonjStatus eBonjourInit(tsProfile *psProfile)
{
    memset(&sBonjour, 0, sizeof(sBonjour));
    sBonjour.psServiceName = BONJOUR_SERVER_TYPE;
    sBonjour.psHostName = NULL;
    sBonjour.u16Port = 1200;
    sBonjour.psInstanceName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;

    sBonjour.sBonjourText.u64CurrentCfgNumber = 1;
    sBonjour.sBonjourText.u8FeatureFlag = 0x01; /* Supports HAP Pairing. This flag is required for all HomeKit accessories */
    sBonjour.sBonjourText.u64DeviceID = 0x03d224a1bd75;
    sBonjour.sBonjourText.psModelName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;
    sBonjour.sBonjourText.auProtocolVersion[0] = 0x01;
    sBonjour.sBonjourText.auProtocolVersion[1] = 0x00;
    sBonjour.sBonjourText.u32iCurrentStaNumber = 1;
    sBonjour.sBonjourText.u8StatusFlag = 0;
    sBonjour.sBonjourText.eAccessoryCategoryID = psProfile->sAccessory.eAccessoryType;

    sBonjour.txtRecord = TextRecordFormat(&sBonjour.sBonjourText);

    if(eBonjourMdnsInit() != E_BONJOUR_STATUS_OK){
       return E_BONJOUR_STATUS_ERROR;
    }

    sBonjour.sThread.pvThreadData = psProfile;
    CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sThread, E_THREAD_DETACHED), E_THREAD_OK, E_BONJOUR_STATUS_ERROR);
    printf("%d-%s\n", TXTRecordGetLength(&sBonjour.txtRecord), (const char*)TXTRecordGetBytesPtr(&sBonjour.txtRecord));
    DNSServiceErrorType  ret = DNSServiceRegister(&sBonjour.psDnsRef, 0, 0, sBonjour.psInstanceName, sBonjour.psServiceName,
                                                  "", sBonjour.psHostName, sBonjour.u16Port, TXTRecordGetLength(&sBonjour.txtRecord), TXTRecordGetBytesPtr(&sBonjour.txtRecord), NULL,NULL);
    TXTRecordDeallocate(&sBonjour.txtRecord);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
        return E_BONJOUR_STATUS_ERROR;
    }




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
static teBonjStatus eBonjourMdnsInit(void)
{
    sBonjour.iSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sBonjour.iSocketFd == -1){
        ERR_vPrintln(T_TRUE, "socket create failed:[%s]", strerror(errno));
        return E_BONJOUR_STATUS_ERROR;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(0);
    int ret = bind(sBonjour.iSocketFd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "bind address failed:[%s]", strerror(errno));
        close(sBonjour.iSocketFd);
        return E_BONJOUR_STATUS_ERROR;
    }

    int re = 1;
    setsockopt(sBonjour.iSocketFd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re));
    listen(sBonjour.iSocketFd, 5);

    return E_BONJOUR_STATUS_OK;
}

static void *pvBonjourThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    tsProfile *psProfile = (tsProfile*)psThreadInfo->pvThreadData;
    psThreadInfo->eState = E_THREAD_RUNNING;

    int iSockClient;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = sizeof(client_addr);

    while(psThreadInfo->eState == E_THREAD_RUNNING){
        int s = accept(sBonjour.iSocketFd, (struct sockaddr*)&client_addr, &client_len);
        if(s == -1){
            printf("error:%s\n", strerror(errno));
            continue;
        }

        printf("client ipaddr:%s\n", inet_ntoa(client_addr.sin_addr));
        sleep(1);
    }
    DBG_vPrintln(DBG_BONJOUR, "pvBonjourThreadHandle Exit");
    vThreadFinish(psThreadInfo);

    return NULL;
}

static TXTRecordRef TextRecordFormat(tsBonjourText *psBonjourText)
{
    TXTRecordRef txtRecord;
    TXTRecordCreate(&txtRecord, 0, NULL);

    char temp_csharp[MIBF] = {0};
    sprintf(temp_csharp, "%llu", psBonjourText->u64CurrentCfgNumber);
    TXTRecordSetValue(&txtRecord, "c#", (uint8)strlen(temp_csharp), temp_csharp);    //Configuration Number

    char temp_ff[MIBF] = {0};
    sprintf(temp_ff, "%d", psBonjourText->u8FeatureFlag);
    TXTRecordSetValue(&txtRecord, "ff", 1, temp_ff);

    char temp_id[MIBF] = {0};
    sprintf(temp_id, "%02x:%02x:%02x:%02x:%02x:%02x",
            (uint8)(psBonjourText->u64DeviceID>>8*5 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*4 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*3 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*2 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*1 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*0 & 0xff));
    TXTRecordSetValue(&txtRecord, "id", (uint8)strlen(temp_id), temp_id);

    char temp_md[MIBF] = {0};
    sprintf(temp_md, "%s", psBonjourText->psModelName);
    TXTRecordSetValue(&txtRecord, "md", (uint8)strlen(temp_md), temp_md);

    char temp_pv[MIBF] = {0};
    sprintf(temp_pv, "%d.%d", psBonjourText->auProtocolVersion[0], psBonjourText->auProtocolVersion[1]);
    TXTRecordSetValue(&txtRecord, "pv", (uint8)strlen(temp_pv), temp_pv);

    char temp_scharp[MIBF] = {0};
    sprintf(temp_scharp, "%d", psBonjourText->u32iCurrentStaNumber);
    TXTRecordSetValue(&txtRecord, "s#", (uint8)strlen(temp_scharp), temp_scharp);

    char temp_sf[MIBF] = {0};
    sprintf(temp_sf, "%d", psBonjourText->u8StatusFlag);
    TXTRecordSetValue(&txtRecord, "sf", (uint8)strlen(temp_sf), temp_sf);

    char temp_ci[MIBF] = {0};
    sprintf(temp_ci, "%d", psBonjourText->eAccessoryCategoryID);
    TXTRecordSetValue(&txtRecord, "ci", (uint8)strlen(temp_ci), temp_ci);

    return txtRecord;
}