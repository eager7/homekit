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
#include "ip.h"


#include <chacha20_simple.h>
#include "pairing.h"
#include "http_handle.h"
#include "poly1305.h"
#include "sodium.h"
#include "hkdf.h"
#include "ed25519.h"
#include "sodium/crypto_aead_chacha20poly1305.h"
#include "curve25519-donna.h"
#include "bonjour.h"
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

void *connectionLoop(void *threadInfo);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
const unsigned char generator[] = {0x05};
const unsigned char modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD, 0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED, 0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D, 0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F, 0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B, 0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10, 0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64, 0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7, 0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0, 0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B, 0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C, 0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31, 0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const unsigned char accessorySecretKey[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74};
const unsigned char curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define devicePassword "523-12-643" //Password
#define deviceIdentity "33:10:34:23:51:12"  //ID
uint8_t controllerToAccessoryKey[32];
uint8_t accessoryToControllerKey[32];
unsigned long long numberOfMsgRec = 0;
unsigned long long numberOfMsgSend = 0;
int currentConfigurationNum = 1;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static tsBonjour sBonjour;
tsController sController;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
unsigned short getSocketPortNumberV4(int _socket) {
    struct sockaddr_in addr; socklen_t len = sizeof(addr);
    getsockname(_socket, (struct sockaddr *)&addr, &len);
    return ntohs(addr.sin_port);
}
teBonjStatus eBonjourInit(tsProfile *psProfile, char *psSetupCode)
{
    SRP_initialize_library();
    srand((unsigned int)time(NULL));

    memset(&sBonjour, 0, sizeof(sBonjour));
    memset(&sController, 0, sizeof(tsController));
    sController.subSocket = -1;

    sBonjour.psServiceName = BONJOUR_SERVER_TYPE;
    sBonjour.psHostName = NULL;
    sBonjour.u16Port = 0;
    sBonjour.psInstanceName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;
    sBonjour.pcSetupCode = psSetupCode;
    sBonjour.sBonjourText.u64CurrentCfgNumber = 1;
    sBonjour.sBonjourText.u8FeatureFlag = 0x00; /* Supports HAP Pairing. This flag is required for all HomeKit accessories */
    sBonjour.sBonjourText.u64DeviceID = psProfile->sAccessory.u64AIDs;
    //sBonjour.sBonjourText.psDeviceID = psDeviceId;
    sBonjour.sBonjourText.psModelName = psProfile->sAccessory.eInformation.sCharacteristics[3].uValue.psData;
    sBonjour.sBonjourText.auProtocolVersion[0] = 0x01;
    sBonjour.sBonjourText.auProtocolVersion[1] = 0x00;
    sBonjour.sBonjourText.u32iCurrentStaNumber = 4;
    sBonjour.sBonjourText.u8StatusFlag = 0x01;
    sBonjour.sBonjourText.eAccessoryCategoryID = psProfile->sAccessory.eAccessoryType;

    CHECK_RESULT(eBonjourSocketInit(), E_BONJOUR_STATUS_OK, E_BONJOUR_STATUS_ERROR);
    eTextRecordFormat(&sBonjour);
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
    DBG_vPrintln(DBG_BONJOUR, "DNSServiceRegister Successful");
#if 1
    do{
        DBG_vPrintln(DBG_BONJOUR, "Waiting Controller Connect");
        int subSocket = accept(sBonjour.iSocketFd, 0, NULL);
        DBG_vPrintln(DBG_BONJOUR, "Controller:%d Connected", subSocket);
        sController.subSocket = subSocket;
        pthread_create(&sController.thread, NULL, connectionLoop, &sController);
    }while(1);
#else
    sBonjour.sThread.pvThreadData = psProfile;
    CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sThread, E_THREAD_DETACHED), E_THREAD_OK, E_BONJOUR_STATUS_ERROR);
    while(1){sleep(1);}
#endif
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
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*5) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*4) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*3) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*2) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*1) & 0xff),
            (uint8)(psBonjour->sBonjourText.u64DeviceID>>(8*0) & 0xff));
    //TXTRecordSetValue(&psBonjour->txtRecord, "id", (uint8)strlen(temp_id), temp_id);
    //memcpy(psBonjour->sBonjourText.psDeviceID, temp_id, sizeof(psBonjour->sBonjourText.psDeviceID));
    TXTRecordSetValue(&psBonjour->txtRecord, "id", (uint8)strlen(deviceIdentity), deviceIdentity);
    memcpy(psBonjour->sBonjourText.psDeviceID, deviceIdentity, strlen(deviceIdentity));
    INF_vPrintln(DBG_BONJOUR, "Device ID:%s", psBonjour->sBonjourText.psDeviceID);

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

static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags,
                                DNSServiceErrorType errorCode, const char *name, const char *regtype, const char *domain, void *context)
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


void handlePairSeup()
{
    tsTlv stateRecord;memset(&stateRecord,0,sizeof(tsTlv));
    uint8 value_rep[1] = {0};
    tePairSetup state = E_PAIR_SETUP_M1_SRP_START_REQUEST;
    SRP *srp;
    srp = SRP_new(SRP6a_server_method());
    cstr *secretKey = NULL, *publicKey = NULL, *response = NULL;
    char sessionKey[64];
    char *responseBuffer = 0; int responseLen = 0;
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    int len = 0;
    do{
        printf("Msg:%s", sController.buffer);
        tsIpMessage *psIpMsg = psIpMessageFormat(sController.buffer, sController.len);
        tsIpMessage *psResponse = psIpResponseNew();
        //tsTlvType
        if(psIpMsg->sTlvMsg.psTlvMsgGetRecordData == NULL){ERR_vPrintln(1,"Null");return;}
        uint8 *pstate = psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, 6);
        if(pstate == NULL){ERR_vPrintln(1,"Null");return;}
        bcopy(pstate, &state, 1);
        //state = (tePairSetup)psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, E_TLV_VALUE_TYPE_STATE);
        value_rep[0] = state+1;
        switch (state) {
            case E_PAIR_SETUP_M1_SRP_START_REQUEST: {
                INF_vPrintln(DBG_BONJOUR, "%s, %d: State_M1_SRPStartRequest\n", __func__, __LINE__);
                unsigned char saltChar[16];
                for (int i = 0; i < 16; i++) {
                    saltChar[i] = rand();
                }
                SRP_RESULT result = SRP_set_username(srp, "Pair-Setup");
                int modulusSize = sizeof(modulusStr) / sizeof(modulusStr[0]);
                result = SRP_set_params(srp, (const unsigned char *)modulusStr, modulusSize, (const unsigned char *)generator, 1, saltChar, 16);
                result = SRP_set_auth_password(srp, devicePassword);
                result = SRP_gen_pub(srp, &publicKey);
                psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_SALT,saltChar,16,&psResponse->sTlvMsg);
                psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,publicKey->data,publicKey->length,&psResponse->sTlvMsg);
                psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psResponse->sTlvMsg);
            }
                break;
            case E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST: {
                INF_vPrintln(DBG_BONJOUR, "%s, %d: State_M3_SRPVerifyRequest\n", __func__, __LINE__);
                const char *keyStr = 0;
                int keyLen = 0;
                const char *proofStr;
                int proofLen;
                keyStr = psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, E_TLV_VALUE_TYPE_PUBLIC_KEY);
                keyLen = psIpMsg->sTlvMsg.pu16TlvMsgGetRecordLength(&psIpMsg->sTlvMsg, E_TLV_VALUE_TYPE_PUBLIC_KEY);
                char *temp = psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, E_TLV_VALUE_TYPE_PROOF);
                if (temp != NULL) {
                    proofStr = temp;
                    proofLen = psIpMsg->sTlvMsg.pu16TlvMsgGetRecordLength(&psIpMsg->sTlvMsg, E_TLV_VALUE_TYPE_PROOF);
                }
                SRP_RESULT result = SRP_compute_key(srp, &secretKey, (const unsigned char*)keyStr, keyLen);
                result = SRP_verify(srp, (const unsigned char*)proofStr, proofLen);
                if (!SRP_OK(result)) {

                    psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),&psResponse->sTlvMsg);
                    printf("Oops at M3\n");
                } else {
                    SRP_respond(srp, &response);
                    psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_PROOF,response->data,response->length,&psResponse->sTlvMsg);
                    printf("Password Correct\n");
                }
                const char salt[] = "Pair-Setup-Encrypt-Salt";
                const char info[] = "Pair-Setup-Encrypt-Info";
                int i = hkdf((const unsigned char*)salt, strlen(salt), (const unsigned char*)secretKey->data,
                             secretKey->length, (const unsigned char*)info, strlen(info), (uint8_t*)sessionKey, 32);
                if (i != 0) {
                    ERR_vPrintln(T_TRUE, "hkdf error");
                    return;
                }
            }
                break;
            case E_PAIR_SETUP_M5_EXCHANGE_REQUEST: {
                INF_vPrintln(DBG_BONJOUR, "%s, %d: State_M5_ExchangeRequest\n", __func__, __LINE__);
                const char *encryptedPackage = NULL;int packageLen = 0;
                encryptedPackage = psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, 5);
                packageLen = psIpMsg->sTlvMsg.pu16TlvMsgGetRecordLength(&psIpMsg->sTlvMsg, 5);
                char *encryptedData = malloc(packageLen);
                bcopy(encryptedPackage, encryptedData, packageLen-16);
                char mac[16];
                bcopy(&encryptedPackage[packageLen-16], mac, 16);

                chacha20_ctx chacha20;    bzero(&chacha20, sizeof(chacha20));
                chacha20_setup(&chacha20, (const uint8_t *)sessionKey, 32, (uint8_t *)"PS-Msg05");

                //Ploy1305 key
                char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
                chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);

                char verify[16];  bzero(verify, 16);
                Poly1305_GenKey((const unsigned char*)temp2, (unsigned char *)encryptedData, packageLen - 16, T_FALSE, verify);

                char *decryptedData = malloc(packageLen-16);
                bzero(decryptedData, packageLen-16);
                chacha20_decrypt(&chacha20, (const uint8_t *)encryptedData, (uint8_t *)decryptedData, packageLen-16);

                if (bcmp(verify, mac, 16)) {
                    psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),&psResponse->sTlvMsg);
                }else {
                    /*
                    * HAK Pair Setup M6
                    */
                    tsTlvMessage subTLV8; memset(&subTLV8, 0, sizeof(subTLV8));
                    eTlvMessageFormat(decryptedData, packageLen-16, &subTLV8);
                    char *controllerIdentifier = subTLV8.psTlvMsgGetRecordData(&subTLV8, 1);
                    char *controllerPublicKey = subTLV8.psTlvMsgGetRecordData(&subTLV8, 3);
                    char *controllerSignature = subTLV8.psTlvMsgGetRecordData(&subTLV8, 10);
                    char controllerHash[100];

                    DBG_vPrintln(DBG_BONJOUR,"save key");
                    eIOSDevicePairingIDSave(controllerIdentifier, 36);
                    eIOSDeviceLTPKSave(controllerPublicKey, 32);

                    const char salt[] = "Pair-Setup-Controller-Sign-Salt";
                    const char info[] = "Pair-Setup-Controller-Sign-Info";
                    int i = hkdf((const unsigned char*)salt, strlen(salt), (const unsigned char*)secretKey->data, secretKey->length, (const unsigned char*)info, strlen(info), (uint8_t*)controllerHash, 32);
                    if (i != 0) {
                        ERR_vPrintln(T_TRUE, "hkdf error");
                        return;
                    }

                    bcopy(controllerIdentifier, &controllerHash[32], 36);
                    bcopy(controllerPublicKey, &controllerHash[68], 32);

                    DBG_vPrintln(DBG_BONJOUR,"ed25519_sign_open");
                    int ed25519_err = ed25519_sign_open((const unsigned char*)controllerHash, 100, (const unsigned char*)controllerPublicKey, (const unsigned char*)controllerSignature);
                    eTlvMessageRelease(&subTLV8);
                    if (ed25519_err) {
                        ERR_vPrintln(T_TRUE, "ed25519_sign_open error");
                        return;
                    }
                    else {
                        DBG_vPrintln(DBG_BONJOUR, "ed25519_sign_open success");
                        tsTlvMessage *returnTLV8 =  psTlvMessageNew();

                        {
                            returnTLV8->efTlvMsgAddRecord(1,deviceIdentity,strlen(deviceIdentity),returnTLV8);
                        }
                        {
                            /*
                             * Generate Signature
                             */
                            const char salt[] = "Pair-Setup-Accessory-Sign-Salt";
                            const char info[] = "Pair-Setup-Accessory-Sign-Info";
                            uint8_t output[150];
                            hkdf((const unsigned char*)salt, strlen(salt), (const unsigned char*)secretKey->data, secretKey->length, (const unsigned char*)info, strlen(info), output, 32);

                            bcopy(deviceIdentity, &output[32], strlen(deviceIdentity));

                            char *signature = malloc(64);
                            ed25519_secret_key edSecret;
                            bcopy(accessorySecretKey, edSecret, sizeof(edSecret));
                            ed25519_public_key edPubKey;
                            ed25519_publickey(edSecret, edPubKey);

                            bcopy(edPubKey, &output[32+strlen(deviceIdentity)], 32);
                            ed25519_sign(output, 64+strlen(deviceIdentity), (const unsigned char*)edSecret, (const unsigned char*)edPubKey, (unsigned char *)signature);

                            returnTLV8->efTlvMsgAddRecord(10,signature,64,returnTLV8);
                            returnTLV8->efTlvMsgAddRecord(3,edPubKey,32,returnTLV8);
                        }
                        const char *tlv8Data;unsigned short tlv8Len;
                        eTlvMsgGetBinaryData(returnTLV8,&tlv8Data, &tlv8Len);

                        char *tlv8Recorddata = malloc(tlv8Len+16);
                        int tlv8Recordlength = tlv8Len+16;
                        bzero(tlv8Recorddata, tlv8Recordlength);

                        {

                            chacha20_ctx ctx;   bzero(&ctx, sizeof(ctx));

                            chacha20_setup(&ctx, (const uint8_t *)sessionKey, 32, (uint8_t *)"PS-Msg06");
                            char buffer[64], key[64];   bzero(buffer, 64);
                            chacha20_encrypt(&ctx, (const uint8_t *)buffer, (uint8_t *)key, 64);
                            chacha20_encrypt(&ctx, (const uint8_t *)tlv8Data, (uint8_t *)tlv8Recorddata, tlv8Len);

                            char verify[16];
                            memset(verify, 0, 16);
                            Poly1305_GenKey((const unsigned char *)key, (unsigned char*)tlv8Recorddata, tlv8Len, T_FALSE, verify);
                            memcpy((unsigned char *)&tlv8Recorddata[tlv8Len], verify, 16);
                        }

                        psResponse->sTlvMsg.efTlvMsgAddRecord(5,tlv8Recorddata,tlv8Recordlength,&psResponse->sTlvMsg);

                        eTlvMessageRelease(returnTLV8);

                    }

                }

                psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psResponse->sTlvMsg);
                psResponse->sTlvMsg.eTlvMsgGetBinaryData(&psResponse->sTlvMsg,&responseBuffer,&responseLen);
                if (responseBuffer) {
                    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
                    eHttpResponse(sController.subSocket, &psIpMsg->sHttp, responseBuffer, responseLen);
                    eIpMessageRelease(psResponse);
                }

                FREE(encryptedData);

                return;
            }
                break;
            default:
                break;
        }
        psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psResponse->sTlvMsg);
        psResponse->sTlvMsg.eTlvMsgGetBinaryData(&psResponse->sTlvMsg,&responseBuffer,&responseLen);

        if (responseBuffer) {
            printf("%s, %d, responseBuffer = %s, responseLen = %d\n", __func__, __LINE__, responseBuffer, responseLen);
            //PrintArray(1, responseBuffer, responseLen);
            psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
            eHttpResponse(sController.subSocket, &psIpMsg->sHttp, responseBuffer, responseLen);
            eIpMessageRelease(psResponse);
            eIpMessageRelease(psIpMsg);
            FREE(responseBuffer);
            printf("Pair Setup Transfered length %d\n", len);
        } else {
            printf("Why empty response\n");
        }
    }while(0 < (sController.len = read(sController.subSocket, sController.buffer, 4096)));
}

void handlePairVerify() {
    bool_t end = T_FALSE;
    char state = E_PAIR_VERIFY_M1_START_REQUEST;

    curved25519_key secretKey;
    curved25519_key publicKey;
    curved25519_key controllerPublicKey;
    curved25519_key sharedKey;

    uint8_t enKey[32];
    uint8 value_rep[1] = {0};
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    printf("Start Pair Verify\n");
    do {
        tsIpMessage *psIpMsg = psIpMessageFormat(sController.buffer, sController.len);
        tsIpMessage *psResponse = psIpResponseNew();
        bcopy(psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, 6), &state, 1);
        value_rep[0] = state+1;
        switch (state) {
            case E_PAIR_VERIFY_M1_START_REQUEST:{
                NOT_vPrintln(DBG_BONJOUR, "Pair Verify M1\n");
                bcopy(psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg, 3), controllerPublicKey, 32);
                for (short i = 0; i < sizeof(secretKey); i++) {
                    secretKey[i] = rand();
                }
                curve25519_donna((u8*)publicKey, (const u8 *)secretKey, (const u8 *)curveBasePoint);

                curve25519_donna(sharedKey, secretKey, controllerPublicKey);

                char *temp = malloc(100);
                bcopy(publicKey, temp, 32);
                bcopy(deviceIdentity, &temp[32], strlen(deviceIdentity));
                bcopy(controllerPublicKey, &temp[32+strlen(deviceIdentity)], 32);

                char *signRecorddata = (char*)malloc(64);
                int signRecordlength = 64;

                ed25519_secret_key edSecret;
                bcopy(accessorySecretKey, edSecret, sizeof(edSecret));
                ed25519_public_key edPubKey;
                ed25519_publickey(edSecret, edPubKey);

                ed25519_sign((const unsigned char *)temp, 64+strlen(deviceIdentity), edSecret, edPubKey, (unsigned char *)signRecorddata);
                FREE(temp);

                char *idRecorddata = (char*)malloc(17);
                bcopy(deviceIdentity, idRecorddata, 17);
                int idRecordlength = (unsigned int)17;

                psResponse->sTlvMsg.efTlvMsgAddRecord(3,publicKey,32,&psResponse->sTlvMsg);

                tsTlvMessage* data = psTlvMessageNew();
                data->efTlvMsgAddRecord(10,signRecorddata,signRecordlength,data);
                data->efTlvMsgAddRecord(1,idRecorddata,idRecordlength,data);

                unsigned char salt[] = "Pair-Verify-Encrypt-Salt";
                unsigned char info[] = "Pair-Verify-Encrypt-Info";

                int i = hkdf(salt, 24, sharedKey, 32, info, 24, enKey, 32);
                const char *plainMsg = 0;   unsigned short msgLen = 0;
                data->eTlvMsgGetBinaryData(data,&plainMsg, &msgLen);

                char *encryptMsg = (char*)malloc(msgLen+16);
                char *polyKey = (char*)malloc(msgLen+64);   bzero(polyKey, 64);

                char zero[64];  bzero(zero, 64);

                chacha20_ctx chacha;
                chacha20_setup(&chacha, enKey, 32, (uint8_t *)"PV-Msg02");
                chacha20_encrypt(&chacha, (uint8_t *)zero, (uint8_t *)polyKey, 64);
                chacha20_encrypt(&chacha, (uint8_t *)plainMsg, (uint8_t *)encryptMsg, msgLen);

                FREE(plainMsg);

                char verify[16];
                memset(verify, 0, 16);
                Poly1305_GenKey((const unsigned char *)polyKey, (uint8_t *)encryptMsg, msgLen, T_FALSE, verify);
                memcpy((unsigned char *)&encryptMsg[msgLen], verify, 16);

                psResponse->sTlvMsg.efTlvMsgAddRecord(5,encryptMsg,msgLen+16,&psResponse->sTlvMsg);

                FREE(encryptMsg);
                FREE(polyKey);
            }break;
            case E_PAIR_VERIFY_M3_FINISHED_REQUEST:{
                NOT_vPrintln(DBG_BONJOUR, "Pair Verify M3\n");

                char *encryptedData = psIpMsg->sTlvMsg.psTlvMsgGetRecordData(&psIpMsg->sTlvMsg,5);
                short packageLen = psIpMsg->sTlvMsg.pu16TlvMsgGetRecordLength(&psIpMsg->sTlvMsg,5);

                chacha20_ctx chacha20;    bzero(&chacha20, sizeof(chacha20));
                chacha20_setup(&chacha20, (const uint8_t *)enKey, 32, (uint8_t *)"PV-Msg03");

                //Ploy1305 key
                char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
                chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);

                char verify[16];    bzero(verify, 16);
                Poly1305_GenKey((const unsigned char *)temp2, (uint8_t *)encryptedData, packageLen - 16, T_FALSE, verify);

                if (!bcmp(verify, &encryptedData[packageLen-16], 16)) {
                    char *decryptData = (char*)malloc(packageLen-16);
                    chacha20_decrypt(&chacha20, (const uint8_t *)encryptedData, (uint8_t *)decryptData, packageLen-16);
                    tsTlvMessage data;memset(&data,0,sizeof(data));
                    eTlvMessageFormat(decryptData, packageLen-16,&data);

                    uint8 *controllerID = data.psTlvMsgGetRecordData(&data,1);
                    uint8 key[36] = {0}; uint8 recpublicKey[32] = {0};
                    eIOSDevicePairingIDRead(key, 36);
                    if(bcmp(key, controllerID, 36) == 0){
                        eIOSDeviceLTPKRead(recpublicKey, 32);
                    } else{
                        ERR_vPrintln(T_TRUE, "controllerID verify failed");
                        return;
                    }

                    char tempMsg[100];
                    bcopy(controllerPublicKey, tempMsg, 32);
                    bcopy(controllerID, &tempMsg[32], 36);
                    bcopy(publicKey, &tempMsg[68], 32);

                    int err = ed25519_sign_open((const unsigned char *)tempMsg, 100, (const unsigned char *)recpublicKey, (const unsigned char *)data.psTlvMsgGetRecordData(&data,10));

                    char *repBuffer = 0;  int repLen = 0;
                    if (err == 0) {
                        end = T_TRUE;

                        hkdf((uint8_t *)"Control-Salt", 12, sharedKey, 32, (uint8_t *)"Control-Read-Encryption-Key", 27, accessoryToControllerKey, 32);
                        hkdf((uint8_t *)"Control-Salt", 12, sharedKey, 32, (uint8_t *)"Control-Write-Encryption-Key", 28, controllerToAccessoryKey, 32);
                        printf("Verify success\n");
                    } else {
                        psResponse->sTlvMsg.efTlvMsgAddRecord(7,value_err,sizeof(value_err),&psResponse->sTlvMsg);
                        printf("Verify failed\n");

                    }
                    FREE(decryptData);
                } else{
                    ERR_vPrintln(T_TRUE, "Error verify");
                }
            }break;
        }

        psResponse->sTlvMsg.efTlvMsgAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psResponse->sTlvMsg);
        char *repBuffer = 0;  int repLen = 0;
        psResponse->sTlvMsg.eTlvMsgGetBinaryData(&psResponse->sTlvMsg,&repBuffer,&repLen);

        if (repBuffer) {
            psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
            eHttpResponse(sController.subSocket, &psIpMsg->sHttp, repBuffer, repLen);
            eIpMessageRelease(psResponse);
            eIpMessageRelease(psIpMsg);
            FREE(repBuffer);
        }
    }while (!end && (0 < (sController.len = read(sController.subSocket, sController.buffer, 4096))));

}

void updateConfiguration() {
    sBonjour.sBonjourText.u64CurrentCfgNumber++;
    eTextRecordFormat(&sBonjour);
    DBG_vPrintln(DBG_BONJOUR, "%d-%s", TXTRecordGetLength(&sBonjour.txtRecord), (const char*)TXTRecordGetBytesPtr(&sBonjour.txtRecord));
    DNSServiceErrorType  ret = DNSServiceUpdateRecord(sBonjour.psDnsRef, NULL, 0, TXTRecordGetLength(&sBonjour.txtRecord), TXTRecordGetBytesPtr(&sBonjour.txtRecord), 0);
    TXTRecordDeallocate(&sBonjour.txtRecord);
    if(ret){
        ERR_vPrintln(DBG_BONJOUR, "DNSServiceUpdateRecord Failed:%d", ret);
        return ;
    }
}
int is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } e = { 0x01000000 };

    return e.c[0];
}
static inline unsigned short bswap_16(unsigned short x) {
    return (x>>8) | (x<<8);
}

static inline unsigned int bswap_32(unsigned int x) {
    return (bswap_16(x&0xffff)<<16) | (bswap_16(x>>16));
}
static inline unsigned long long bswap_64(unsigned long long x) {
    return x;//(((unsigned long long)bswap_32(x&0xffffffffull))<<32) |
//    (bswap_32(x>>32));
}
void handleAccessoryRequest() {
    char *decryptData = malloc(2048);
    int len;
    printf("Successfully Connect\n");
    numberOfMsgRec = 0;
    numberOfMsgSend = 0;
    do {
        bzero(sController.buffer, 4096);
        len = read(sController.subSocket, sController.buffer, 4096);
        if (len < 0)break;
        //FIXME make sure buffer len > (2 + msgLen + 16)??
        uint16_t msgLen = (uint8_t)sController.buffer[1]*256+(uint8_t)*sController.buffer;

        chacha20_ctx chacha20;    bzero(&chacha20, sizeof(chacha20));

        printf("send: %llx\n", numberOfMsgRec);
        if (!is_big_endian()) numberOfMsgRec = bswap_64(numberOfMsgRec);
        printf("send: %llx\n", numberOfMsgRec);
        chacha20_setup(&chacha20, (const uint8_t *)controllerToAccessoryKey, 32, (uint8_t *)&numberOfMsgRec);
        if (!is_big_endian()) numberOfMsgRec = bswap_64(numberOfMsgRec);
        numberOfMsgRec++;
        printf("send: %llx\n", numberOfMsgRec);

        char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
        chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);

        //Ploy1305 key
        char verify[16];    bzero(verify, 16);
        Poly1305_GenKey((const unsigned char *)temp2, (uint8_t *)sController.buffer, msgLen, T_FALSE, verify);

        bzero(decryptData, 2048);
        chacha20_encrypt(&chacha20, (const uint8_t *)&sController.buffer[2], (uint8_t *)decryptData, msgLen);

        printf("Request: %s\nPacketLen: %d\n, MessageLen: %d\n", decryptData, len, strlen(decryptData));

        if(len >= (2 + msgLen + 16)
           && memcmp((void *)verify, (void *)&sController.buffer[2 + msgLen], 16) == 0) {
            printf("Verify successfully!\n");
        }
        else {
            continue;
        }
        //Output return
        char *resultData = 0; unsigned int resultLen = 0;
        //handleAccessory(decryptData, msgLen, &resultData, &resultLen, this);

        //18 = 2(resultLen) + 16(poly1305 verify key)
        char *reply = malloc(resultLen+18);
        reply[0] = resultLen%256;
        reply[1] = (resultLen-(uint8_t)reply[0])/256;

        if (!is_big_endian()) numberOfMsgSend = bswap_64(numberOfMsgSend);
        chacha20_setup(&chacha20, (const uint8_t *)accessoryToControllerKey, 32, (uint8_t *)&numberOfMsgSend);
        if (!is_big_endian()) numberOfMsgSend = bswap_64(numberOfMsgSend);
        numberOfMsgSend++;

        chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);
        chacha20_encrypt(&chacha20, (const uint8_t*)resultData, (uint8_t*)&reply[2], resultLen);

        Poly1305_GenKey((const unsigned char *)temp2, (uint8_t *)reply, resultLen, T_FALSE, verify);
        memcpy((unsigned char*)&reply[resultLen+2], verify, 16);
        write(sController.subSocket, reply, resultLen+18);
        FREE(reply);
        FREE(resultData);
    }while (len > 0);
    FREE(decryptData);
}

void *connectionLoop(void *threadInfo)
{
    tsController *info = (tsController *)threadInfo;
    int subSocket = info->subSocket;
    ssize_t len = 0;
    if (subSocket >= 0) {
        printf("Start Connect: %d\n", subSocket);
        do {
            memset(info->buffer, 0, 4096);
            len = read(subSocket, info->buffer, 4096);

            printf("Return len %d for socket %d\n", len, subSocket);
            //printf("Message: %s\n", info->buffer);

            if (len > 0) {
                tsHttpEntry sHttp;memset(&sHttp, 0, sizeof(sHttp));
                eHttpParser(info->buffer, len, &sHttp);
                sController.len = len;
                if (strstr(sHttp.acDirectory, "pair-setup")){
                    /*
                     * The process of pair-setup
                     */
                    DBG_vPrintln(DBG_BONJOUR, "Pair Setup");
                    handlePairSeup();
                    updateConfiguration();
                }
                else if (strstr(sHttp.acDirectory, "pair-verify")){
                    DBG_vPrintln(1, "//////////pair-verify");
                    handlePairVerify();
                    //When pair-verify done, we handle Accessory Request
                    handleAccessoryRequest();
                }
                else if (strstr(sHttp.acDirectory, "identify")){
                    close(subSocket);
                }
            }

        } while (len > 0);
        close(subSocket);
        printf("Stop Connect: %d\n", subSocket);
        //info->subSocket = -1;
    }
    return NULL;
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

    //int iSockClient;
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
                        sController.subSocket = accept(sBonjour.iSocketFd, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
                        if(-1 == sController.subSocket){
                            ERR_vPrintln(T_TRUE, "Accept client failed:%s", strerror(errno));
                            break;
                        } else {
                            DBG_vPrintln(DBG_BONJOUR, "A client connected[%s]", inet_ntoa(client_addr.sin_addr));
                            FD_SET(sController.subSocket, &fdSelect);
                            if(sController.subSocket > iListenFD){
                                iListenFD = sController.subSocket;
                            }
                            iNumberClient ++;
                        }
                    }
                } else {    /* Client Communication */
                    if(FD_ISSET(sController.subSocket, &fdTemp)){
                        char buf[MABF] = {0};
                        ssize_t len = recv(sController.subSocket, sController.buffer, sizeof(buf), 0);
                        if(0 == len){
                            ERR_vPrintln(T_TRUE, "Close Client:%d\n", sController.subSocket);
                            close(sController.subSocket);
                            FD_SET(sBonjour.iSocketFd, &fdSelect);//Add socket server fd into select fd
                            FD_CLR(sController.subSocket, &fdSelect);//delete this client from select set
                            iNumberClient --;
                        } else {
                            DBG_vPrintln(DBG_BONJOUR, "RecvMsg[%d]\n%s", (int)len, buf);
                            //eHapHandlePackage(buf, (int)len, iSockClient, &sBonjour);
                            {
                                tsHttpEntry sHttp;memset(&sHttp, 0, sizeof(sHttp));
                                eHttpParser(sController.buffer, len, &sHttp);
                                sController.len = len;
                                if (strstr(sHttp.acDirectory, "pair-setup")){
                                    /*
                                     * The process of pair-setup
                                     */
                                    DBG_vPrintln(DBG_BONJOUR, "Pair Setup");
                                    handlePairSeup();
                                    updateConfiguration();
                                }
                                else if (strstr(sHttp.acDirectory, "pair-verify")){
                                    DBG_vPrintln(1, "//////////pair-verify");
                                    handlePairVerify();
                                    //When pair-verify done, we handle Accessory Request
                                    //handleAccessoryRequest();
                                }
                                else if (strstr(sHttp.acDirectory, "identify")){
                                    //close(sC);
                                }
                            }
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
