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
static char *pcTextRecordFormat(tsBonjourText *psBonjourText);
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
    sBonjour.sBonjourText.psModelName = "Device1,1";
    sBonjour.sBonjourText.auProtocolVersion[0] = 0x01;
    sBonjour.sBonjourText.auProtocolVersion[1] = 0x00;
    sBonjour.sBonjourText.u32iCurrentStaNumber = 1;
    sBonjour.sBonjourText.u8StatusFlag = 0;
    sBonjour.sBonjourText.eAccessoryCategoryID = psProfile->sAccessory.eAccessoryType;

    sBonjour.psTextRecord = pcTextRecordFormat(&sBonjour.sBonjourText);

    //if(eBonjourMdnsInit() != E_BONJOUR_STATUS_OK){
    //   return E_BONJOUR_STATUS_ERROR;
    //}

    //sBonjour.sThread.pvThreadData = psProfile;
    //CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sThread, E_THREAD_DETACHED), E_THREAD_OK, E_BONJOUR_STATUS_ERROR);

    DNSServiceErrorType  ret = DNSServiceRegister(&sBonjour.psDnsRef, 0, 0, sBonjour.psInstanceName, sBonjour.psServiceName,
                                                  "", sBonjour.psHostName, sBonjour.u16Port, 0, "", NULL,NULL);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceRegister Failed:%d", ret);
        return E_BONJOUR_STATUS_ERROR;
    }

    printf("%d-%s\n", (uint16_t)strlen(sBonjour.psTextRecord), sBonjour.psTextRecord);
    ret = DNSServiceUpdateRecord(sBonjour.psDnsRef, NULL, 0, (uint16_t)strlen(sBonjour.psTextRecord), sBonjour.psTextRecord, 0);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceUpdateRecord Failed:%d", ret);
        return E_BONJOUR_STATUS_ERROR;
    }

    return E_BONJOUR_STATUS_OK;
}

teBonjStatus eBonjourFinished(tsProfile *psProfile)
{
    FREE(sBonjour.psTextRecord);
    if(sBonjour.psDnsRef) DNSServiceRefDeallocate(sBonjour.psDnsRef);
    return E_BONJOUR_STATUS_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teBonjStatus eBonjourMdnsInit(void)
{
    sBonjour.iSocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sBonjour.iSocketFd == -1){
        ERR_vPrintln(T_TRUE, "socket create failed:[%s]", strerror(errno));
        return E_BONJOUR_STATUS_ERROR;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(MULTICAST_DNS_PORT);
    int ret = bind(sBonjour.iSocketFd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "bind address failed:[%s]", strerror(errno));
        close(sBonjour.iSocketFd);
        return E_BONJOUR_STATUS_ERROR;
    }

    struct ip_mreq multi_addr;
    memset(&multi_addr, 0, sizeof(multi_addr));
    multi_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_DNS_ADDRESS);
    multi_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    int re = 1;
    setsockopt(sBonjour.iSocketFd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re));
    setsockopt(sBonjour.iSocketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multi_addr, sizeof(multi_addr));

    return E_BONJOUR_STATUS_OK;
}

static void *pvBonjourThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    tsProfile *psProfile = (tsProfile*)psThreadInfo->pvThreadData;
    psThreadInfo->eState = E_THREAD_RUNNING;

    while(psThreadInfo->eState == E_THREAD_RUNNING){

    }
    DBG_vPrintln(DBG_BONJOUR, "pvBonjourThreadHandle Exit");
    vThreadFinish(psThreadInfo);

    return NULL;
}

static char *pcTextRecordFormat(tsBonjourText *psBonjourText)
{
    char temp[MDBF] = {0};
    uint8 u8Index = 0;
    char temp_csharp[MIBF] = {0};
    sprintf(temp_csharp, "c#=%llu", psBonjourText->u64CurrentCfgNumber);
    temp[u8Index] = (uint8)strlen(temp_csharp);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_csharp, strlen(temp_csharp));
    u8Index += strlen(temp_csharp);


    char temp_ff[MIBF] = {0};
    sprintf(temp_ff, "ff=%d", psBonjourText->u8FeatureFlag);
    temp[u8Index] = (uint8)strlen(temp_ff);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_ff, strlen(temp_ff));
    u8Index += strlen(temp_ff);

    char temp_id[MIBF] = {0};
    sprintf(temp_id, "id=%02x:%02x:%02x:%02x:%02x:%02x",
            (uint8)(psBonjourText->u64DeviceID>>8*5 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*4 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*3 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*2 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*1 & 0xff),
            (uint8)(psBonjourText->u64DeviceID>>8*0 & 0xff));
    temp[u8Index] = (uint8)strlen(temp_id);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_id, strlen(temp_id));
    u8Index += strlen(temp_id);

    char temp_md[MIBF] = {0};
    sprintf(temp_md, "md=%s", psBonjourText->psModelName);
    temp[u8Index] = (uint8)strlen(temp_md);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_md, strlen(temp_md));
    u8Index += strlen(temp_md);

    char temp_pv[MIBF] = {0};
    sprintf(temp_pv, "pv=%d.%d", psBonjourText->auProtocolVersion[0], psBonjourText->auProtocolVersion[1]);
    temp[u8Index] = (uint8)strlen(temp_pv);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_pv, strlen(temp_pv));
    u8Index += strlen(temp_pv);

    char temp_scharp[MIBF] = {0};
    sprintf(temp_scharp, "s#=%d", psBonjourText->u32iCurrentStaNumber);
    temp[u8Index] = (uint8)strlen(temp_scharp);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_scharp, strlen(temp_scharp));
    u8Index += strlen(temp_scharp);

    char temp_sf[MIBF] = {0};
    sprintf(temp_sf, "sf=%d", psBonjourText->u8StatusFlag);
    temp[u8Index] = (uint8)strlen(temp_sf);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_sf, strlen(temp_sf));
    u8Index += strlen(temp_sf);

    char temp_ci[MIBF] = {0};
    sprintf(temp_ci, "ci=%d", psBonjourText->eAccessoryCategoryID);
    temp[u8Index] = (uint8)strlen(temp_ci);
    u8Index += sizeof(uint8);
    memcpy(&temp[u8Index], temp_ci, strlen(temp_ci));
    u8Index += strlen(temp_ci);
    printf("u8Index:%d\n", u8Index);

    DBG_vPrintln(DBG_BONJOUR, "TextRecord[%d]:%s", (int)strlen(temp), temp);

    char *record = (char*)malloc(strlen(temp));
    memset(record, 0, strlen(temp));
    memcpy(record, temp, strlen(temp));
    return record;
}