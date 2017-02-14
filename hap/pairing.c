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
#include <chacha20_simple.h>
#include "pairing.h"
#include "http_handle.h"
#include "poly1305.h"
#include "sodium.h"
#include "hkdf.h"
#include "ed25519.h"
#include "sodium/crypto_aead_chacha20poly1305.h"
#include "curve25519-donna.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_PAIR 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static tePairStatus eM2SrpStartResponse(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry);
static tePairStatus eM4SrpVerifyResponse(int iSockFd, tsHttpEntry *psHttpEntry);
static tePairStatus eM6ExchangeResponse(int iSockFd, char *psDeviceID, tsHttpEntry *psHttpEntry);
static tePairStatus eTlvTypeFormatAdd(tsTlvType *psTlvData, teTlvValue eTlvValue, uint16 u16ValueLen, uint8 *puValueData);
static tePairStatus eTlvTypeGetObject(teTlvValue eTlvValue, uint8 *pBuffer, uint16 u16Len, tsTlvType *psTlvType);
static void Poly1305_GenKey(const unsigned char * key, uint8_t * buf, uint16_t len, bool_t bWithLen, char* verify);

static tePairStatus eIOSDevicePairingIDSave(uint8 *buf, int len);
static tePairStatus eIOSDevicePairingIDRead(uint8 *buf, int len);
static tePairStatus eIOSDeviceLTPKSave(uint8 *buf, int len);
static tePairStatus eIOSDeviceLTPKRead(uint8 *buf, int len);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
const unsigned char modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD, 0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED, 0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D, 0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F, 0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B, 0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10, 0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64, 0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7, 0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0, 0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B, 0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C, 0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31, 0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const unsigned char generator[] = {0x05};
#define devicePassword "523-12-643" //Password
const unsigned char accessorySecretKey[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74};
#define deviceIdentity "12:10:34:23:51:22"  //ID
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
SRP     *pSrp = NULL;
cstr    *pShareKey = NULL;
char    auSessionKey[64];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tePairStatus ePairSetup(int iSockFd, tsBonjour *psBonjour, char *pBuf, uint16 u16Len)
{
    tePairStatus Status = E_PAIRING_STATUS_ERROR;
    pSrp = SRP_new(SRP6a_server_method());
    char auBufferRecv[MABF] = {0};
    uint16 u16LenRecv = u16Len;
    memcpy(auBufferRecv, pBuf, sizeof(auBufferRecv));
    do{
        tsTlvType sTlvResponse;
        memset(&sTlvResponse, 0, sizeof(sTlvResponse));

        tsHttpEntry sHttpEntry; memset(&sHttpEntry, 0 ,sizeof(sHttpEntry));
        if(eHttpParser(auBufferRecv, u16LenRecv, &sHttpEntry) != E_HTTP_PARSER_OK){
            ERR_vPrintln(T_TRUE, "eHttpParser Failed"); goto Failed;
        }
        tsTlvType sTlvState; memset(&sTlvState, 0, sizeof(sTlvState));
        if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_STATE, sHttpEntry.acContentData, sHttpEntry.u16ContentLen, &sTlvState)){
            ERR_vPrintln(T_TRUE, "Can't Find E_TLV_VALUE_TYPE_STATE"); goto Failed;
        }
        tePairSetup ePairSetupState = (tePairSetup)sTlvState.psValue[0];
        FREE(sTlvState.psValue);
        uint8 value_rep[1] = { ePairSetupState + 1 };
        switch (ePairSetupState){
            case E_PAIR_SETUP_M1_SRP_START_REQUEST: {
                if(E_PAIRING_STATUS_OK != eM2SrpStartResponse(iSockFd, psBonjour->pcSetupCode, &sHttpEntry)){
                    ERR_vPrintln(T_TRUE, "eM2SrpStartResponse Failed"); goto Failed;
                }
            }break;
            case E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST:{
                if(E_PAIRING_STATUS_OK != eM4SrpVerifyResponse(iSockFd, &sHttpEntry)){
                    ERR_vPrintln(T_TRUE, "eM4SrpVerifyResponse Failed"); goto Failed;
                }
            }break;
            case E_PAIR_SETUP_M5_EXCHANGE_REQUEST:{
                if(E_PAIRING_STATUS_OK != eM6ExchangeResponse(iSockFd, psBonjour->sBonjourText.psDeviceID, &sHttpEntry)){
                    ERR_vPrintln(T_TRUE, "eM6ExchangeResponse Failed"); goto Failed;
                }
                return E_PAIRING_STATUS_OK;
            }
            default:
                break;
        }
        memset(auBufferRecv, 0, sizeof(auBufferRecv));
    } while(0< (u16LenRecv = (uint16)recv(iSockFd, (void *)auBufferRecv, sizeof(auBufferRecv), 0) ));
    Status = E_PAIRING_STATUS_ERROR_SOCKET;
Failed:
    SRP_free(pSrp);
    return Status;
}

const unsigned char curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
tePairStatus ePairVerify(int iSockFd, tsBonjour *psBonjour, char *pBuf, uint16 u16Len)
{
    tePairStatus Status = E_PAIRING_STATUS_ERROR;
    curved25519_key secretKey;
    curved25519_key publicKey;
    curved25519_key controllerPublicKey;
    curved25519_key sharedKey;
    uint8_t enKey[32];
    char auBufferRecv[MABF] = {0};
    uint16 u16LenRecv = u16Len;
    memcpy(auBufferRecv, pBuf, sizeof(auBufferRecv));
    do{
        tsTlvType sTlvResponse;
        memset(&sTlvResponse, 0, sizeof(sTlvResponse));

        tsHttpEntry sHttpEntry; memset(&sHttpEntry, 0 ,sizeof(sHttpEntry));
        if(eHttpParser(auBufferRecv, u16LenRecv, &sHttpEntry) != E_HTTP_PARSER_OK){
            ERR_vPrintln(T_TRUE, "eHttpParser Failed"); goto Failed;
        }
        tsTlvType sTlvState; memset(&sTlvState, 0, sizeof(sTlvState));
        if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_STATE, sHttpEntry.acContentData, sHttpEntry.u16ContentLen, &sTlvState)){
            ERR_vPrintln(T_TRUE, "Can't Find E_TLV_VALUE_TYPE_STATE"); goto Failed;
        }
        tePairVerify ePairVerifyState = (tePairVerify)sTlvState.psValue[0];
        FREE(sTlvState.psValue);
        uint8 value_rep[1] = { ePairVerifyState + 1 };



        switch (ePairVerifyState){
            case E_PAIR_VERIFY_M1_START_REQUEST: {
                //if(E_PAIRING_STATUS_OK != eM2VerifyStartResponse(iSockFd, psBonjour->pcSetupCode, &sHttpEntry)){
                //    ERR_vPrintln(T_TRUE, "eM2SrpStartResponse Failed"); goto Failed;
                //}
                printf("Pair Verify M1\n");
                tsTlvType pubkey;memset(&pubkey,0,sizeof(pubkey));
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY,sHttpEntry.acContentData,sHttpEntry.u16ContentLen,&pubkey);
                bcopy(pubkey.psValue, controllerPublicKey, 32);
                for (short i = 0; i < sizeof(secretKey); i++) {
                    secretKey[i] = rand();
                }
                curve25519_donna((u8*)publicKey, (const u8 *)secretKey, (const u8 *)curveBasePoint);

                curve25519_donna(sharedKey, secretKey, controllerPublicKey);
                char *temp = malloc(100);
                bcopy(publicKey, temp, 32);
                bcopy(deviceIdentity, &temp[32], strlen(deviceIdentity));
                bcopy(controllerPublicKey, &temp[32+strlen(deviceIdentity)], 32);
                char signRecord[64];
                ed25519_secret_key edSecret;
                bcopy(accessorySecretKey, edSecret, sizeof(edSecret));
                ed25519_public_key edPubKey;
                ed25519_publickey(edSecret, edPubKey);
                ed25519_sign((const unsigned char *)temp, 64+strlen(deviceIdentity), edSecret, edPubKey, (unsigned char *)signRecord);
                tsTlvType sTlvSub;memset(&sTlvSub, 0, sizeof(sTlvSub));
                eTlvTypeFormatAdd(&sTlvSub, E_TLV_VALUE_TYPE_SIGNATURE, 64, signRecord);
                eTlvTypeFormatAdd(&sTlvSub, E_TLV_VALUE_TYPE_IDENTIFIER, strlen(deviceIdentity), deviceIdentity);


                unsigned char salt[] = "Pair-Verify-Encrypt-Salt";
                unsigned char info[] = "Pair-Verify-Encrypt-Info";

                int i = hkdf(salt, 24, sharedKey, 32, info, 24, enKey, 32);
                const char *plainMsg = (char*)sTlvSub.psValue;   unsigned short msgLen = sTlvSub.u16Len;
                char *encryptMsg = malloc(msgLen+16);
                char *polyKey = malloc(64);   bzero(polyKey, 64);

                char zero[64];  bzero(zero, 64);

                chacha20_ctx chacha;
                chacha20_setup(&chacha, enKey, 32, (uint8_t *)"PV-Msg02");
                chacha20_encrypt(&chacha, (uint8_t *)zero, (uint8_t *)polyKey, 64);
                chacha20_encrypt(&chacha, (uint8_t *)plainMsg, (uint8_t *)encryptMsg, msgLen);
                char verify[16];
                memset(verify, 0, 16);
                Poly1305_GenKey((const unsigned char *)polyKey, (uint8_t *)encryptMsg, msgLen, T_FALSE, verify);
                memcpy((unsigned char *)&encryptMsg[msgLen], verify, 16);

                eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_PUBLIC_KEY, 32, publicKey);
                eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ENCRYPTED_DATA, msgLen+16, encryptMsg);

            }break;
            case E_PAIR_VERIFY_M3_FINISHED_REQUEST:{
                //if(E_PAIRING_STATUS_OK != eM4VerifyFinishResponse(iSockFd, &sHttpEntry)){
                //    ERR_vPrintln(T_TRUE, "eM4SrpVerifyResponse Failed"); goto Failed;
                //}
                printf("Pair Verify M3\n");
                tsTlvType encryptedData;memset(&encryptedData, 0, sizeof(encryptedData));
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,sHttpEntry.acContentData,sHttpEntry.u16ContentLen, &encryptedData);
                short packageLen = encryptedData.u16Len;
                chacha20_ctx chacha20;    bzero(&chacha20, sizeof(chacha20));
                chacha20_setup(&chacha20, (const uint8_t *)enKey, 32, (uint8_t *)"PV-Msg03");
                //Ploy1305 key
                char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
                chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);
                char verify[16];    bzero(verify, 16);
                Poly1305_GenKey((const unsigned char *)temp2, (uint8_t *)encryptedData.psValue, packageLen - 16, T_FALSE, verify);
                if (!bcmp(verify, &encryptedData.psValue[packageLen-16], 16)) {
                    char *decryptData = malloc(packageLen-16);
                    chacha20_decrypt(&chacha20, (const uint8_t *)encryptedData.psValue, (uint8_t *)decryptData, packageLen-16);
                    tsTlvType rec;memset(&rec,0,sizeof(rec));
                    tsTlvType sig;memset(&sig,0,sizeof(sig));
                    eTlvTypeGetObject(E_TLV_VALUE_TYPE_IDENTIFIER,decryptData,packageLen-16,&rec);
                    eTlvTypeGetObject(E_TLV_VALUE_TYPE_SIGNATURE,decryptData,packageLen-16,&sig);
                    char recpublickey[32] = {0};
                    if(E_PAIRING_STATUS_OK != eIOSDeviceLTPKRead(recpublickey,32)){
                        ERR_vPrintln(T_TRUE,"Can't Read LTPK");
                        return E_PAIRING_STATUS_ERROR;
                    }
                    char tempMsg[100];
                    bcopy(controllerPublicKey, tempMsg, 32);
                    bcopy(rec.psValue, &tempMsg[32], 36);
                    bcopy(publicKey, &tempMsg[68], 32);
                    int err = ed25519_sign_open((const unsigned char *)tempMsg, 100, (const unsigned char *)recpublickey,
                                                (const unsigned char *)sig.psValue);

                    char *repBuffer = 0;  int repLen = 0;
                    if (err == 0) {
                        uint8_t controllerToAccessoryKey[32];
                        uint8_t accessoryToControllerKey[32];
                        hkdf((uint8_t *)"Control-Salt", 12, sharedKey, 32, (uint8_t *)"Control-Read-Encryption-Key", 27, accessoryToControllerKey, 32);
                        hkdf((uint8_t *)"Control-Salt", 12, sharedKey, 32, (uint8_t *)"Control-Write-Encryption-Key", 28, controllerToAccessoryKey, 32);
                        printf("Verify success\n");
                    } else{
                        printf("Verify failed\n");
                        uint8 err[] = {E_TLV_ERROR_AUTHENTICATION};
                        eTlvTypeFormatAdd(&sTlvResponse,E_TLV_VALUE_TYPE_ERROR,1,err);
                    }

                } else{
                    printf("tag verify failed\n");
                }


            }break;
            default:
                break;
        }
        uint8 rep[1] = {ePairVerifyState+1};
        eTlvTypeFormatAdd(&sTlvResponse,E_TLV_VALUE_TYPE_STATE,1,rep);
        sHttpEntry.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
        eHttpResponse(iSockFd, &sHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
        FREE(sTlvResponse.psValue);

        memset(auBufferRecv, 0, sizeof(auBufferRecv));
    } while(0< (u16LenRecv = (uint16)recv(iSockFd, (void *)auBufferRecv, sizeof(auBufferRecv), 0) ));
    Status = E_PAIRING_STATUS_ERROR_SOCKET;
    Failed:

    return Status;
}

tePairStatus ePairSetup2(int iSockFd, char *pSetupCode, char *pBuf, uint16 u16Len)
{
    SRP *srp;
    srp = SRP_new(SRP6a_server_method());
    cstr *secretKey = NULL, *publicKey = NULL, *response = NULL;
    char sessionKey[64];
    char *responseBuffer = 0; int responseLen = 0;

    char auBufferRecv[MABF] = {0};
    uint16 u16LenRecv = u16Len;
    memcpy(auBufferRecv, pBuf, sizeof(auBufferRecv));
    do{
        tsTlvType mResponse;memset(&mResponse,0,sizeof(tsTlvType));

        tsHttpEntry sHttpEntry; memset(&sHttpEntry, 0 ,sizeof(sHttpEntry));
        if(eHttpParser(auBufferRecv, u16LenRecv, &sHttpEntry) != E_HTTP_PARSER_OK){
            ERR_vPrintln(T_TRUE, "eHttpParser Failed"); goto Failed;
        }

        tsTlvType stateRecord;memset(&stateRecord,0,sizeof(tsTlvType));
        eTlvTypeGetObject(E_TLV_VALUE_TYPE_STATE,sHttpEntry.acContentData,sHttpEntry.u16ContentLen,&stateRecord);
        uint8 value_rep[] ={(tePairSetup)stateRecord.psValue[0]+1} ;
        switch ((tePairSetup)stateRecord.psValue[0]){
            case E_PAIR_SETUP_M1_SRP_START_REQUEST:{
                printf("%s, %d: State_M1_SRPStartRequest\n", __func__, __LINE__);
                tsTlvType saltRec;memset(&saltRec,0,sizeof(tsTlvType));
                tsTlvType publicKeyRec;memset(&publicKeyRec,0,sizeof(tsTlvType));
                unsigned char saltChar[16];
                for (int i = 0; i < 16; i++) {
                    saltChar[i] = rand();
                }
                SRP_RESULT result = SRP_set_username(srp, "Pair-Setup");
                int modulusSize = sizeof(modulusStr) / sizeof(modulusStr[0]);
                result = SRP_set_params(srp, (const unsigned char *)modulusStr, modulusSize, (const unsigned char *)generator, 1, saltChar, 16);
                result = SRP_set_auth_password(srp, devicePassword);
                result = SRP_gen_pub(srp, &publicKey);
                eTlvTypeFormatAdd(&mResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);
                eTlvTypeFormatAdd(&mResponse, E_TLV_VALUE_TYPE_PUBLIC_KEY, publicKey->length, publicKey->data);
                eTlvTypeFormatAdd(&mResponse, E_TLV_VALUE_TYPE_SALT, sizeof(saltChar),saltChar);
            }
                break;
            case E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST:{
                printf("%s, %d: State_M3_SRPVerifyRequest\n", __func__, __LINE__);
                tsTlvType keyStr;memset(&keyStr,0,sizeof(tsTlvType));
                tsTlvType proofStr;memset(&proofStr,0,sizeof(tsTlvType));
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY,sHttpEntry.acContentData,sHttpEntry.u16ContentLen,&keyStr);
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_PROOF,sHttpEntry.acContentData,sHttpEntry.u16ContentLen,&proofStr);
                SRP_RESULT result = SRP_compute_key(srp, &secretKey, (const unsigned char*)keyStr.psValue, keyStr.u16Len);
                result = SRP_verify(srp, (const unsigned char*)proofStr.psValue, proofStr.u16Len);
                if (!SRP_OK(result)) {
                    ERR_vPrintln(T_TRUE, "SRP_verify");
                    return E_PAIRING_STATUS_ERROR;
                }
                SRP_respond(srp, &response);
                tsTlvType responseRecord;memset(&responseRecord,0,sizeof(tsTlvType));
                eTlvTypeFormatAdd(&mResponse,E_TLV_VALUE_TYPE_PROOF,response->length,response->data);
                const char salt[] = "Pair-Setup-Encrypt-Salt";
                const char info[] = "Pair-Setup-Encrypt-Info";
                int i = hkdf((const unsigned char*)salt, strlen(salt), (const unsigned char*)secretKey->data,
                             secretKey->length, (const unsigned char*)info, strlen(info), (uint8_t*)sessionKey, 32);
                if (i != 0) {
                    ERR_vPrintln(T_TRUE, "hkdf");return E_PAIRING_STATUS_ERROR;
                }
            }
                break;
            case E_PAIR_SETUP_M5_EXCHANGE_REQUEST:{
                printf("%s, %d: State_M5_ExchangeRequest\n", __func__, __LINE__);
                tsTlvType encryptedPackage;memset(&encryptedPackage,0,sizeof(tsTlvType));

                eTlvTypeGetObject(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,sHttpEntry.acContentData,sHttpEntry.u16ContentLen,&encryptedPackage);
                int packageLen = encryptedPackage.u16Len;
                char *encryptedData = malloc(packageLen);
                bcopy(encryptedPackage.psValue, encryptedData, packageLen-16);
                char mac[16];
                bcopy(&encryptedPackage.psValue[packageLen-16], mac, 16);
                chacha20_ctx chacha20;    bzero(&chacha20, sizeof(chacha20));
                chacha20_setup(&chacha20, (const uint8_t *)sessionKey, 32, (uint8_t *)"PS-Msg05");
                //Ploy1305 key
                char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
                chacha20_encrypt(&chacha20, (const uint8_t*)temp, (uint8_t *)temp2, 64);
                char verify[16];  bzero(verify, 16);
                Poly1305_GenKey((const unsigned char*)temp2, (unsigned char *)encryptedData, packageLen - 16,
                                T_FALSE, verify);
                char *decryptedData =malloc(packageLen-16);
                bzero(decryptedData, packageLen-16);
                chacha20_decrypt(&chacha20, (const uint8_t *)encryptedData, (uint8_t *)decryptedData, packageLen-16);
                if (bcmp(verify, mac, 16)) {
                    ERR_vPrintln(T_TRUE, "chacha20_decrypt");return E_PAIRING_STATUS_ERROR;
                }
                printf("HAK Pair Setup M6\n");
                tsTlvType controllerIdentifier;memset(&controllerIdentifier,0,sizeof(tsTlvType));
                tsTlvType controllerPublicKey;memset(&controllerPublicKey,0,sizeof(tsTlvType));
                tsTlvType controllerSignature;memset(&controllerSignature,0,sizeof(tsTlvType));
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_IDENTIFIER,decryptedData,packageLen-16,&controllerIdentifier);
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY,decryptedData,packageLen-16,&controllerPublicKey);
                eTlvTypeGetObject(E_TLV_VALUE_TYPE_SIGNATURE,decryptedData,packageLen-16,&controllerSignature);
                char controllerHash[100];
                const char salt[] = "Pair-Setup-Controller-Sign-Salt";
                const char info[] = "Pair-Setup-Controller-Sign-Info";
                int i = hkdf((const unsigned char*)salt, strlen(salt),
                             (const unsigned char*)secretKey->data, secretKey->length,
                             (const unsigned char*)info, strlen(info), (uint8_t*)controllerHash, 32);
                if (i != 0){
                    ERR_vPrintln(T_TRUE, "hkdf");return E_PAIRING_STATUS_ERROR;
                }

                bcopy(controllerIdentifier.psValue, &controllerHash[32], 36);
                bcopy(controllerPublicKey.psValue, &controllerHash[68], 32);
                int ed25519_err = ed25519_sign_open((const unsigned char*)controllerHash, 100,
                                                    (const unsigned char*)controllerPublicKey.psValue, (const unsigned char*)controllerSignature.psValue);

                if (ed25519_err) {
                    ERR_vPrintln(T_TRUE, "ed25519_sign_open");return E_PAIRING_STATUS_ERROR;
                }
                INF_vPrintln(T_TRUE, "ed25519_sign_open Success");
                tsTlvType returnTLV8;memset(&returnTLV8,0,sizeof(tsTlvType));
                //tsTlvType usernameRecord;memset(&usernameRecord,0,sizeof(tsTlvType));
                eTlvTypeFormatAdd(&returnTLV8,E_TLV_VALUE_TYPE_IDENTIFIER,strlen(deviceIdentity),deviceIdentity);
                /*
                 * Generate Signature
                 */
                const char salt2[] = "Pair-Setup-Accessory-Sign-Salt";
                const char info2[] = "Pair-Setup-Accessory-Sign-Info";
                uint8_t output[150];
                hkdf((const unsigned char*)salt2, strlen(salt2), (const unsigned char*)secretKey->data,
                     secretKey->length, (const unsigned char*)info2, strlen(info2), output, 32);

                bcopy(deviceIdentity, &output[32], strlen(deviceIdentity));
                char *signature = malloc(64);
                ed25519_secret_key edSecret;
                bcopy(accessorySecretKey, edSecret, sizeof(edSecret));
                ed25519_public_key edPubKey;
                ed25519_publickey(edSecret, edPubKey);
                bcopy(edPubKey, &output[32+strlen(deviceIdentity)], 32);
                ed25519_sign(output, 64+strlen(deviceIdentity), (const unsigned char*)edSecret,
                             (const unsigned char*)edPubKey, (unsigned char *)signature);
                eTlvTypeFormatAdd(&returnTLV8,E_TLV_VALUE_TYPE_SIGNATURE,64,signature);
                eTlvTypeFormatAdd(&returnTLV8,E_TLV_VALUE_TYPE_PUBLIC_KEY,32,edPubKey);

                unsigned short tlv8Len = returnTLV8.u16Len;
                tsTlvType tlv8Record;memset(&tlv8Record,0,sizeof(tsTlvType));
                tlv8Record.psValue = malloc(tlv8Len+16);tlv8Record.u16Len =tlv8Len +16;
                bzero(tlv8Record.psValue, tlv8Record.u16Len);
                chacha20_ctx ctx;   bzero(&ctx, sizeof(ctx));
                chacha20_setup(&ctx, (const uint8_t *)sessionKey, 32, (uint8_t *)"PS-Msg06");
                char buffer[64], key[64];   bzero(buffer, 64);
                chacha20_encrypt(&ctx, (const uint8_t *)buffer, (uint8_t *)key, 64);
                chacha20_encrypt(&ctx, (const uint8_t *)returnTLV8.psValue, (uint8_t *)tlv8Record.psValue, tlv8Len);
                char verify2[16];
                memset(verify2, 0, 16);
                Poly1305_GenKey((const unsigned char *)key, (unsigned char*)tlv8Record.psValue, tlv8Len, T_FALSE, verify2);
                memcpy((unsigned char *)&tlv8Record.psValue[tlv8Len], verify2, 16);
                eTlvTypeFormatAdd(&mResponse,E_TLV_VALUE_TYPE_ENCRYPTED_DATA,tlv8Record.u16Len,tlv8Record.psValue);

                eTlvTypeFormatAdd(&mResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

                sHttpEntry.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
                eHttpResponse(iSockFd, &sHttpEntry, mResponse.psValue, mResponse.u16Len);
                FREE(mResponse.psValue);
                INF_vPrintln(T_TRUE,"SUCCESS--------");
                return E_PAIRING_STATUS_OK;
            }
                break;
            default:
                break;
        }
        eTlvTypeFormatAdd(&mResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

        sHttpEntry.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
        eHttpResponse(iSockFd, &sHttpEntry, mResponse.psValue, mResponse.u16Len);
        FREE(mResponse.psValue);

        memset(auBufferRecv, 0, sizeof(auBufferRecv));
    }while(0< (u16LenRecv = (uint16)recv(iSockFd, (void *)auBufferRecv, sizeof(auBufferRecv), 0) ));
    Failed:
    SRP_free(pSrp);
    return E_PAIRING_STATUS_ERROR;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

static tePairStatus eM2SrpStartResponse(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry)
{
    const unsigned char modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
                                        0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
                                        0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
                                        0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
                                        0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
                                        0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
                                        0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
                                        0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
                                        0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
                                        0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
                                        0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
                                        0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
                                        0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
                                        0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
                                        0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
                                        0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
                                        0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
                                        0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
                                        0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
                                        0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
                                        0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
                                        0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
                                        0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
                                        0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
                                        0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
                                        0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
                                        0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
                                        0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
                                        0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
                                        0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
                                        0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
                                        0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const unsigned char curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char generator[] = {0x05};
    SRP_set_username(pSrp, "Pair-Setup");
    uint8 auSalt[LEN_SALT] = {0};
    for(int j = 0; j < LEN_SALT; j++){
        auSalt[j] = (uint8)rand();
    }
    SRP_set_params(pSrp, modulusStr, sizeof(modulusStr)/sizeof(modulusStr[0]), generator, sizeof(generator), auSalt, sizeof(auSalt));
    INF_vPrintln(DBG_PAIR, "SetupCode:%s", pSetupCode);
    SRP_set_auth_password(pSrp, pSetupCode);
    cstr *psPublicKey = NULL;
    SRP_gen_pub(pSrp, &psPublicKey);

    tsTlvType sTlvResponse;
    memset(&sTlvResponse, 0, sizeof(sTlvResponse));  //must init
    uint8 value_rep[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_PUBLIC_KEY, (uint16) psPublicKey->length, (uint8 *) psPublicKey->data);
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_SALT, sizeof(auSalt), auSalt);
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_OK;
}

static tePairStatus eM4SrpVerifyResponse(int iSockFd, tsHttpEntry *psHttpEntry)
{
    CHECK_POINTER(psHttpEntry, E_PAIRING_STATUS_ERROR);

    uint8 value_err[1] = {0};
    tsTlvType sTlvResponse; memset(&sTlvResponse, 0, sizeof(sTlvResponse));
    uint8 value_rep[] = {E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE};
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

    tsTlvType sTlvPublicKey, sTlvProof;
    memset(&sTlvPublicKey, 0, sizeof(sTlvPublicKey));
    memset(&sTlvProof, 0, sizeof(sTlvProof));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvPublicKey) ||
       E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PROOF, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvProof)){
        WAR_vPrintln(DBG_PAIR, "Can't find TLV Data");
        FREE(sTlvProof.psValue); FREE(sTlvPublicKey.psValue);
        goto TlvErrorUnknow;
    }
    SRP_RESULT ret = SRP_compute_key(pSrp, &pShareKey, sTlvPublicKey.psValue, sTlvPublicKey.u16Len);
    ret = SRP_verify(pSrp, sTlvProof.psValue, sTlvProof.u16Len);
    FREE(sTlvProof.psValue); FREE(sTlvPublicKey.psValue);
    if(!SRP_OK(ret)){
        WAR_vPrintln(DBG_PAIR, "Verify IOS Proof Failed");
        goto TlvErrorAuthentication;
    }
    DBG_vPrintln(DBG_PAIR, "Verify IOS Proof Success");

    cstr *pCstrProof = NULL;
    SRP_respond(pSrp, &pCstrProof);
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_PROOF, (uint16)pCstrProof->length, (uint8*)pCstrProof->data);

    const char salt[] = "Pair-Setup-Encrypt-Salt";
    const char info[] = "Pair-Setup-Encrypt-Info";
    int i = hkdf((const unsigned char*)salt, (int)strlen(salt), (const unsigned char*)pShareKey->data,
                 pShareKey->length, (const unsigned char*)info, (int)strlen(info), auSessionKey, LEN_HKDF_LEN);
    if(i != 0){
        ERR_vPrintln(T_TRUE, "Generate SessionKey Failed");
        FREE(sTlvResponse.psValue);
        return E_PAIRING_STATUS_ERROR;
    }
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_OK;

TlvErrorUnknow:
    value_err[0] = E_TLV_ERROR_UNKNOW;
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ERROR, sizeof(value_err), value_err);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_ERROR;

TlvErrorAuthentication:
    value_err[0] = E_TLV_ERROR_AUTHENTICATION;
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ERROR, sizeof(value_err), value_err);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_ERROR_KEY;
}

static tePairStatus eM6ExchangeResponse(int iSockFd, char *psDeviceID, tsHttpEntry *psHttpEntry)
{
    uint8 value_err[1] = {0};
    tsTlvType sTlvResponse; memset(&sTlvResponse, 0, sizeof(sTlvResponse));
    uint8 value_rep[] = {E_PAIR_SETUP_M6_EXCHANGE_RESPONSE};
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

    //TODO:(M5 Verification 1) verify authTag, which is appended to the encryptedData
    tsTlvType sTlvEncryptData;
    memset(&sTlvEncryptData, 0, sizeof(sTlvEncryptData));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,
                                                psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvEncryptData)){
        ERR_vPrintln(T_TRUE, "Can't Find E_TLV_VALUE_TYPE_ENCRYPTED_DATA");
        FREE(sTlvResponse.psValue);
        return E_PAIRING_STATUS_ERROR;
    }

    uint8 auAuthTag[LEN_AUTH_TAG] = {0};
    uint16 u16EncryptDataLen = sTlvEncryptData.u16Len - (uint16)LEN_AUTH_TAG;       //delete authtag
    uint8 *psEncryptedData = (uint8*)malloc(u16EncryptDataLen);
    if(NULL == psEncryptedData){
        FREE(sTlvResponse.psValue); FREE(sTlvEncryptData.psValue);
        return E_PAIRING_STATUS_ERROR;
    }
    memset(psEncryptedData, 0, u16EncryptDataLen);
    memcpy(psEncryptedData, sTlvEncryptData.psValue, u16EncryptDataLen);            //sub-TLV data
    memcpy(auAuthTag, &sTlvEncryptData.psValue[u16EncryptDataLen], LEN_AUTH_TAG);   //authTag data
    FREE(sTlvEncryptData.psValue);

    chacha20_ctx context; memset(&context, 0, sizeof(context));
    chacha20_setup(&context, (const uint8_t *)auSessionKey, LEN_CHA20_KEY, (uint8*)"PS-Msg05");

    uint8 auIn[64] = {0}; uint8 auKey[64] = {0};
    chacha20_encrypt(&context, auIn, auKey, 64);

    char auVerify[LEN_AUTH_TAG] = {0};
    Poly1305_GenKey(auKey, psEncryptedData, u16EncryptDataLen, T_FALSE, auVerify);
    if(memcmp(auVerify, auAuthTag, LEN_AUTH_TAG)){
        ERR_vPrintln(T_TRUE, "Verify authTag Failed");
        FREE(psEncryptedData);
        goto AuthenticationError;
    }
    DBG_vPrintln(DBG_PAIR, "Verify authTag Success");
    //TODO:(M5 Verification 2) Decrypt the sub-TLV
    uint8 *psDecryptedData = (uint8*)malloc(u16EncryptDataLen);
    if(NULL == psDecryptedData){
        FREE(sTlvResponse.psValue); FREE(psEncryptedData);
        return E_PAIRING_STATUS_ERROR;
    }
    memset(psDecryptedData, 0, u16EncryptDataLen);
    chacha20_decrypt(&context, psEncryptedData, psDecryptedData, u16EncryptDataLen);//Decrypt Data
    FREE(psEncryptedData);

    tsTlvType sTlvIOSDevicePairingID, sTlvIOSDeviceLTPK, sTlvSignature;
    memset(&sTlvIOSDevicePairingID, 0, sizeof(sTlvIOSDevicePairingID));
    memset(&sTlvIOSDeviceLTPK, 0, sizeof(sTlvIOSDeviceLTPK));
    memset(&sTlvSignature, 0, sizeof(sTlvSignature));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_IDENTIFIER, psDecryptedData, u16EncryptDataLen, &sTlvIOSDevicePairingID)){
        ERR_vPrintln(T_TRUE, "Can't find E_TLV_VALUE_TYPE_IDENTIFIER");
        FREE(psDecryptedData); goto AuthenticationError;
    }
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY, psDecryptedData, u16EncryptDataLen, &sTlvIOSDeviceLTPK)){
        ERR_vPrintln(T_TRUE, "Can't find E_TLV_VALUE_TYPE_PUBLIC_KEY");
        FREE(psDecryptedData); FREE(sTlvIOSDevicePairingID.psValue); goto AuthenticationError;
    }
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_SIGNATURE, psDecryptedData, u16EncryptDataLen, &sTlvSignature)){
        ERR_vPrintln(T_TRUE, "Can't find E_TLV_VALUE_TYPE_SIGNATURE");
        FREE(psDecryptedData); FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue); goto AuthenticationError;
    }
    FREE(psDecryptedData);
    printf("sTlvSignature len:%d, sTlvIOSDevicePairingID len:%d, sTlvIOSDeviceLTPK len:%d\n",
           sTlvSignature.u16Len, sTlvIOSDevicePairingID.u16Len, sTlvIOSDeviceLTPK.u16Len);
    DBG_vPrintln(DBG_PAIR, "Decrypt Data Success");
    //TODO:(M5 Verification 3) Derive IOSDeviceX from the SRP shared secret by using HKDF-SHA-512

    uint8 auIOSDeviceX[LEN_HKDF_LEN] = {0};
    const char salt[] = "Pair-Setup-Controller-Sign-Salt";
    const char info[] = "Pair-Setup-Controller-Sign-Info";
    if(hkdf((const unsigned char*)salt, (int)strlen(salt), (const unsigned char*)pShareKey->data,
            pShareKey->length, (const unsigned char*)info, (int)strlen(info), auIOSDeviceX, LEN_HKDF_LEN)){
        ERR_vPrintln(T_TRUE, "HKDF-SHA-512 Failed");
        FREE(sTlvResponse.psValue); FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue);FREE(sTlvSignature.psValue);
        return E_PAIRING_STATUS_ERROR;
    }
    //TODO:(M5 Verification 4) Construct IOSDeviceInfo by concatenating IOSDeviceX with the IOSDevicePairingID and IOSDeviceLTPK
    uint8 auIOSDeviceInfo[100] = {0};
    memcpy(auIOSDeviceInfo, auIOSDeviceX, sizeof(auIOSDeviceX));
    memcpy(&auIOSDeviceInfo[32], sTlvIOSDevicePairingID.psValue, 36);
    memcpy(&auIOSDeviceInfo[32 + 36], sTlvIOSDeviceLTPK.psValue, 32);
    //PrintArray(DBG_PAIR, auIOSDeviceInfo, 100);
    //TODO:(M5 Verification 5) Use Ed25519 to verify the signature of the constructed IOSDeviceInfo with the IOSDeviceLTPK form the decrypted sub-TLV
    if(ed25519_sign_open(auIOSDeviceInfo, sizeof(auIOSDeviceInfo), sTlvIOSDeviceLTPK.psValue, sTlvSignature.psValue)){
        ERR_vPrintln(T_TRUE, "ed25519_sign_open Failed");
        FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue);FREE(sTlvSignature.psValue);
        goto AuthenticationError;
    }
    DBG_vPrintln(DBG_PAIR, "Verify IOSDeviceInfo Success");
    //TODO:(M5 Verification 6) Persistently save the IOSDevicePairingId and IOSDeviceLTPK as a pairing
    eIOSDevicePairingIDSave(sTlvIOSDevicePairingID.psValue, sTlvIOSDevicePairingID.u16Len);
    eIOSDeviceLTPKSave(sTlvIOSDeviceLTPK.psValue, sTlvIOSDeviceLTPK.u16Len);
    FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue);

    tsTlvType sTlvSubTlv;memset(&sTlvSubTlv, 0, sizeof(sTlvSubTlv));
    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_IDENTIFIER, strlen(psDeviceID), psDeviceID);
    const char salt2[] = "Pair-Setup-Accessory-Sign-Salt";
    const char info2[] = "Pair-Setup-Accessory-Sign-Info";
    uint8 auAccessoryInfo[150] = {0};
    uint8_t auAccessoryX[32];
    int ret = hkdf((const unsigned char*)salt2, strlen(salt2), (const unsigned char*)pShareKey->data,
                   pShareKey->length, (const unsigned char*)info2, strlen(info2), auAccessoryInfo, LEN_HKDF_LEN);
    if(ret){
        ERR_vPrintln(T_TRUE, "HKDF Failed");
        return E_PAIRING_STATUS_ERROR;
    }
    printf("auAccessoryInfo:");
    PrintArray(T_TRUE, auAccessoryInfo, 32);
    printf("deviceIdentity:");
    PrintArray(T_TRUE, psDeviceID, (int)strlen(psDeviceID));
    memcpy(&auAccessoryInfo[32], psDeviceID, strlen(psDeviceID));
    uint8 auAccessorySignature[64] = {0};
    //TODO:(M6 Response 1) Generate its Ed25519 long-term public key,AccessoryLTPK and long-term secret key,AccessoryLTSK
    ed25519_secret_key auAccessoryLTSK;
    memcpy(auAccessoryLTSK, accessorySecretKey, sizeof(auAccessoryLTSK));
    ed25519_public_key auAccessoryLTPK;
    ed25519_publickey(auAccessoryLTSK, auAccessoryLTPK);

    //TODO:(M6 Response 2) Derive AccessoryX from the SRP shared secret by using HKDF-SHA-512

    //TODO:(M6 Response 3) Concatenate AccessoryX with AccessoryPairingID and AccessoryLTPK to AccessoryInfo

    //memcpy(auAccessoryInfo, auAccessoryX, sizeof(auAccessoryX));
    printf("auAccessoryLTPK:");
    PrintArray(T_TRUE, auAccessoryLTPK, (int)sizeof(auAccessoryLTPK));
    memcpy(&auAccessoryInfo[32+strlen(psDeviceID)], auAccessoryLTPK, 32);
    //TODO:(M6 Response 4) Use Ed25519 to generate AccessorySignature by siging AccessoryInfo with AccessoryLTSK
    printf("auAccessoryInfo:");
    PrintArray(T_TRUE, auAccessoryInfo, (int)sizeof(auAccessoryInfo));
    ed25519_sign(auAccessoryInfo, 64+17, auAccessoryLTSK, auAccessoryLTPK, auAccessorySignature);
    //TODO:(M6 Response 5) Construct the sub-TLV with AccessoryPairingID,AccessoryLTPK,AccessorySignature

    printf("auAccessorySignature:");
    PrintArray(T_TRUE, auAccessorySignature, (int)sizeof(auAccessorySignature));

    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_SIGNATURE, 64, auAccessorySignature);
    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_PUBLIC_KEY,32, auAccessoryLTPK);
    printf("sTlvSubTlv:");
    PrintArray(T_TRUE, sTlvSubTlv.psValue, sTlvSubTlv.u16Len);
    //TODO:(M6 Response 6) Encrypt the sub-TLV,encryptedData,and generate the 16 byte auth tag,authTag, uses the ChaCha20-Poly1305 AEAD algorithm
    uint8 *pauEncryptedData = (uint8*)malloc(sTlvSubTlv.u16Len + 16);
    CHECK_POINTER(pauEncryptedData, E_PAIRING_STATUS_ERROR);
    memset(pauEncryptedData, 0, sTlvSubTlv.u16Len + 16);
    chacha20_ctx ctx; memset(&ctx, 0, sizeof(ctx));
    chacha20_setup(&ctx, (const uint8 *)auSessionKey, 32, (uint8 *)"PS-Msg06");
    uint8 buffer[64] = {0}, key[64] = {0};
    chacha20_encrypt(&ctx, buffer, key, 64);
    chacha20_encrypt(&ctx, sTlvSubTlv.psValue, pauEncryptedData, sTlvSubTlv.u16Len);PrintArray(T_TRUE, pauEncryptedData, sTlvSubTlv.u16Len);

    char authTag[16] = {0};
    Poly1305_GenKey(key, pauEncryptedData, sTlvSubTlv.u16Len, T_FALSE, authTag);PrintArray(T_TRUE, authTag, 16);
    memcpy(&pauEncryptedData[sTlvSubTlv.u16Len], authTag, 16);
    PrintArray(T_TRUE, pauEncryptedData, sTlvSubTlv.u16Len+16);
    //TODO:(M6 Response 7) Send the response to the IOS device with TLV items
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ENCRYPTED_DATA, (uint16)(sTlvSubTlv.u16Len + 16), pauEncryptedData);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);FREE(sTlvSubTlv.psValue);FREE(pauEncryptedData);
    return E_PAIRING_STATUS_OK;

AuthenticationError:
    value_err[0] = E_TLV_ERROR_AUTHENTICATION;
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ERROR, sizeof(value_err), value_err);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_ERROR;

MaxPeersError:
    value_err[0] = E_TLV_ERROR_MAXPEERS;
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ERROR, sizeof(value_err), value_err);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);
    return E_PAIRING_STATUS_ERROR;
}

static tePairStatus eTlvTypeFormatAdd(tsTlvType *psTlvData, teTlvValue eTlvValue, uint16 u16ValueLen, uint8 *puValueData)
{
    CHECK_POINTER(puValueData, E_PAIRING_STATUS_ERROR);

    unsigned int num = (unsigned int)(u16ValueLen / TLV_FRAGMENTED);
    if(u16ValueLen % TLV_FRAGMENTED){
        num ++;
    }
    psTlvData->u16Len += (uint16)(u16ValueLen + TLV_HEADER * num);
    uint8 *psReturnData = (uint8*)realloc(psTlvData->psValue, psTlvData->u16Len * sizeof(uint8));
    CHECK_POINTER(psReturnData, E_PAIRING_STATUS_ERROR);
    psTlvData->psValue = psReturnData;

    int i = 0;
    for (i = 0; i < num - 1; ++i) {
        psReturnData[psTlvData->u16Offset] = eTlvValue;
        psReturnData[psTlvData->u16Offset + TLV_TYPE_LEN] = TLV_FRAGMENTED;
        memcpy(&psReturnData[psTlvData->u16Offset + TLV_HEADER], &puValueData[TLV_FRAGMENTED*i], TLV_FRAGMENTED);
        psTlvData->u16Offset += TLV_FRAGMENTED + TLV_HEADER;
    }
    psReturnData[psTlvData->u16Offset] = eTlvValue;
    psReturnData[psTlvData->u16Offset + TLV_TYPE_LEN] = (uint8)(u16ValueLen - TLV_FRAGMENTED * i);
    memcpy(&psReturnData[psTlvData->u16Offset + TLV_HEADER], &puValueData[TLV_FRAGMENTED * i], (size_t)(u16ValueLen - TLV_FRAGMENTED * i));
    psTlvData->u16Offset += u16ValueLen - TLV_FRAGMENTED * i + TLV_HEADER;

    return E_PAIRING_STATUS_OK;
}

static tePairStatus eTlvTypeGetObject(teTlvValue eTlvValue, uint8 *pBuffer, uint16 u16Len, tsTlvType *psTlvType)
{
    int index = 0;
    int fragmented = 0;
    if(psTlvType->psValue != NULL){
        WAR_vPrintln(T_TRUE, "The Tlv value is not null, will be leak memory");
        return E_PAIRING_STATUS_ERROR;
    }

    do {
        if(pBuffer[index] == eTlvValue){
            fragmented = 1;
            psTlvType->u8Type = eTlvValue;
            psTlvType->u16Len += pBuffer[index + TLV_TYPE_LEN];
            uint8* psTemp = (uint8*)realloc(psTlvType->psValue, psTlvType->u16Len * sizeof(uint8));
            CHECK_POINTER(psTemp, E_PAIRING_STATUS_ERROR);
            psTlvType->psValue = psTemp;
            memcpy(&psTlvType->psValue[psTlvType->u16Offset], &pBuffer[index + TLV_HEADER], pBuffer[index + TLV_TYPE_LEN]);
            psTlvType->u16Offset = psTlvType->u16Len;
        }
        int len = pBuffer[index + TLV_TYPE_LEN] + TLV_HEADER;
        index += len;
        if((index >= u16Len)) break;
        if(fragmented && (pBuffer[index] != eTlvValue)) break;
    } while(T_TRUE);

    if(psTlvType->u16Offset == 0){
        return E_PAIRING_STATUS_NOT_FOUND;
    }

    //DBG_vPrintln(DBG_PAIR, "eTlvTypeGetObject[%d][%d]", psTlvType->u8Type, psTlvType->u16Len);
    PrintArray(T_FALSE, psTlvType->psValue, psTlvType->u16Len);

    return E_PAIRING_STATUS_OK;
}

static void Poly1305_GenKey(const unsigned char * key, uint8_t * buf, uint16_t len, bool_t bWithLen, char* verify)
{
    if (key == NULL || buf == NULL || len < 2 || verify == NULL)
        return;

    poly1305_context verifyContext; bzero(&verifyContext, sizeof(verifyContext));
    poly1305_init(&verifyContext, key);

    char waste[16];
    bzero(waste, 16);
    if (bWithLen) {
        poly1305_update(&verifyContext, (const unsigned char *)&buf[0], 1);
        poly1305_update(&verifyContext, (const unsigned char *)&buf[1], 1);
        poly1305_update(&verifyContext, (const unsigned char *)waste, 14);
        poly1305_update(&verifyContext, (const unsigned char *)&buf[2], len);
    } else {
        poly1305_update(&verifyContext, (const unsigned char *)buf, len);
    }
    if (len%16 > 0)
        poly1305_update(&verifyContext, (const unsigned char *)waste, 16-(len%16));
    unsigned char _len;
    if (bWithLen) {
        _len = 2;
    } else {
        _len = 0;
    }

    poly1305_update(&verifyContext, (const unsigned char *)&_len, 1);
    poly1305_update(&verifyContext, (const unsigned char *)&waste, 7);
    _len = len;
    poly1305_update(&verifyContext, (const unsigned char *)&_len, 1);
    _len = len/256;
    poly1305_update(&verifyContext, (const unsigned char *)&_len, 1);
    poly1305_update(&verifyContext, (const unsigned char *)&waste, 6);
    poly1305_finish(&verifyContext, (unsigned char*)verify);
}

static tePairStatus eIOSDevicePairingIDSave(uint8 *buf, int len)
{
    FILE *fp = fopen("IOSDevicePairingID.txt", "w");
    if(len != fwrite(buf,1,len,fp)){
        return E_PAIRING_STATUS_ERROR;
    }
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDevicePairingIDRead(uint8 *buf, int len)
{
    FILE *fp = fopen("IOSDevicePairingID.txt", "r");
    if(len != fread(buf,1,len,fp)){
        return E_PAIRING_STATUS_ERROR;
    }
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDeviceLTPKSave(uint8 *buf, int len)
{
    FILE *fp = fopen("IOSDeviceLTPK.txt", "w");
    if(len != fwrite(buf,1,len,fp)){
        return E_PAIRING_STATUS_ERROR;
    }
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDeviceLTPKRead(uint8 *buf, int len)
{
    FILE *fp = fopen("IOSDeviceLTPK.txt", "r");
    if(len != fread(buf,1,len,fp)){
        return E_PAIRING_STATUS_ERROR;
    }
    return E_PAIRING_STATUS_OK;
}