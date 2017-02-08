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
static tePairStatus eM6ExchangeResponse(int iSockFd, tsHttpEntry *psHttpEntry);
static tePairStatus eTlvTypeFormatAdd(tsTlvType *psTlvData, teTlvValue eTlvValue, uint16 u16ValueLen, uint8 *puValueData);
static tePairStatus eTlvTypeGetObject(teTlvValue eTlvValue, uint8 *pBuffer, uint16 u16Len, tsTlvType *psTlvType);
static void Poly1305_GenKey(const unsigned char * key, uint8_t * buf, uint16_t len, bool_t bWithLen, char* verify);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
SRP     *pSrp = NULL;
cstr    *pShareKey = NULL;
uint8    auSessionKey[64] = {0};
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tePairStatus ePairSetup(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry)
{
    tePairSetupState ePair = E_PAIR_SETUP_SRP_START_NULL;
    tsTlvType sTlvType;
    memset(&sTlvType, 0, sizeof(sTlvType));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_STATE, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvType)){
        WAR_vPrintln(DBG_PAIR, "Can't find kTLVType_State Data");
        return E_PAIRING_STATUS_ERROR;
    }
    ePair = (tePairSetupState)sTlvType.psValue[0];
    FREE(sTlvType.psValue);

    DBG_vPrintln(DBG_PAIR, "IOS PairSetup:M%d", ePair);
    switch(ePair){
        case (E_PAIR_SETUP_M1_SRP_START_REQUEST):{
            eM2SrpStartResponse(iSockFd, pSetupCode, psHttpEntry);
        }
            break;
        case (E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST):{
            eM4SrpVerifyResponse(iSockFd, psHttpEntry);
        }
            break;
        case (E_PAIR_SETUP_M5_EXCHANGE_REQUEST):{
            eM6ExchangeResponse(iSockFd, psHttpEntry);
        }
            break;
        default:
            break;
    }
    return E_PAIRING_STATUS_OK;
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

    pSrp =  SRP_new(SRP6a_server_method());
    SRP_set_username(pSrp, "Pair-Setup");
    uint8 auSalt[16] = {0};
    for(int j = 0; j < sizeof(auSalt); j++){
        auSalt[j] = (uint8)rand();
    }
    SRP_set_params(pSrp, modulusStr, sizeof(modulusStr)/sizeof(modulusStr[0]), generator, sizeof(generator), auSalt, sizeof(auSalt));
    SRP_set_auth_password(pSrp, pSetupCode);
    cstr *psPublicKey = NULL;
    SRP_gen_pub(pSrp, &psPublicKey);

    tsTlvType sTlvData;
    memset(&sTlvData, 0, sizeof(sTlvData));  //must init
    uint8 value_rep[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_PUBLIC_KEY, (uint16) psPublicKey->length, (uint8 *) psPublicKey->data);
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_SALT, sizeof(auSalt), auSalt);
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvData.psValue, sTlvData.u16Len);
    FREE(sTlvData.psValue);
    return E_PAIRING_STATUS_OK;
}

static tePairStatus eM4SrpVerifyResponse(int iSockFd, tsHttpEntry *psHttpEntry)
{
    CHECK_POINTER(psHttpEntry, E_PAIRING_STATUS_ERROR);
    tePairStatus eStatus = E_PAIRING_STATUS_OK;
    tsTlvType sTlvData;
    memset(&sTlvData, 0, sizeof(sTlvData));
    uint8 value_rep[] = {E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE};
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);


    tsTlvType sTlvPublicKey, sTlvProof;
    memset(&sTlvPublicKey, 0, sizeof(sTlvPublicKey));
    memset(&sTlvProof, 0, sizeof(sTlvProof));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvPublicKey) ||
       E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PROOF, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvProof)){
        WAR_vPrintln(DBG_PAIR, "Can't find TLV Data");
        uint8 err[] = {E_TLV_ERROR_UNKNOW};
        eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_ERROR, sizeof(err), err);
        eStatus = E_PAIRING_STATUS_ERROR;
        goto send;
    }
    SRP_RESULT ret = SRP_compute_key(pSrp, &pShareKey, sTlvPublicKey.psValue, sTlvPublicKey.u16Len);
    ret = SRP_verify(pSrp, sTlvProof.psValue, sTlvProof.u16Len);

    if(!SRP_OK(ret)){
        WAR_vPrintln(DBG_PAIR, "Verify IOS Proof Failed");
        uint8 err[] = {E_TLV_ERROR_AUTHENTICATION};
        eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_ERROR, sizeof(err), err);
        eStatus = E_PAIRING_STATUS_ERROR_KEY;
        goto send;
    }
    DBG_vPrintln(DBG_PAIR, "Verify IOS Proof Success");

    cstr *pCstrProof = NULL;
    SRP_respond(pSrp, &pCstrProof);
    eTlvTypeFormatAdd(&sTlvData, E_TLV_VALUE_TYPE_PROOF, (uint16)pCstrProof->length, (uint8*)pCstrProof->data);

    const char salt[] = "Pair-Setup-Encrypt-Salt";
    const char info[] = "Pair-Setup-Encrypt-Info";
#define LEN_OUT 32
    int i = hkdf((const unsigned char*)salt, (int)strlen(salt), (const unsigned char*)pShareKey->data,
                 pShareKey->length, (const unsigned char*)info, (int)strlen(info), auSessionKey, LEN_OUT);
    if(i != 0){
        ERR_vPrintln(T_TRUE, "Generate SessionKey Failed");
        FREE(sTlvData.psValue);
        return E_PAIRING_STATUS_ERROR;
    }
send:
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvData.psValue, sTlvData.u16Len);
    FREE(sTlvData.psValue);

    return eStatus;
}

static tePairStatus eM6ExchangeResponse(int iSockFd, tsHttpEntry *psHttpEntry)
{
    uint8 value_err[1] = {0};
    tsTlvType sTlvResponse;memset(&sTlvResponse, 0, sizeof(sTlvResponse));
    uint8 value_rep[] = {E_PAIR_SETUP_M6_EXCHANGE_RESPONSE};
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_STATE, sizeof(value_rep), value_rep);

    //TODO:(M5 Verification 1) verify authTag, which is appended to the encryptedData
    tsTlvType sTlvEncryptData;
    memset(&sTlvEncryptData, 0, sizeof(sTlvEncryptData));
    CHECK_RESULT(eTlvTypeGetObject(E_TLV_VALUE_TYPE_ENCRYPTED_DATA, psHttpEntry->acContentData,
                                   psHttpEntry->u16ContentLen, &sTlvEncryptData),
                 E_PAIRING_STATUS_OK, E_PAIRING_STATUS_ERROR);

    uint8 auAuthTag[LEN_AUTH_TAG] = {0};
    uint16 u16LenEncryptData = sTlvEncryptData.u16Len - (uint16)LEN_AUTH_TAG;       //delete authtag
    uint8 *psEncryptedData = (uint8*)malloc(sTlvEncryptData.u16Len);
    CHECK_POINTER(psEncryptedData, E_PAIRING_STATUS_ERROR);
    memset(psEncryptedData, 0, u16LenEncryptData);
    memcpy(psEncryptedData, sTlvEncryptData.psValue, u16LenEncryptData);            //sub-TLV data
    memcpy(auAuthTag, &sTlvEncryptData.psValue[u16LenEncryptData], LEN_AUTH_TAG);   //authTag data
    FREE(sTlvEncryptData.psValue);

    chacha20_ctx context; memset(&context, 0, sizeof(context));
    chacha20_setup(&context, auSessionKey, LEN_CHA20_KEY, (uint8 *)"PS-Msg05");
    uint8 auIn[64] = {0}; uint8 auKey[64] = {0};
    chacha20_encrypt(&context, auIn, auKey, 64);
    PrintArray(DBG_PAIR, auKey, sizeof(auKey));
    char auVerify[LEN_AUTH_TAG] = {0};
    Poly1305_GenKey(auKey, psEncryptedData, u16LenEncryptData, T_FALSE, auVerify);
    if(memcmp(auVerify, auAuthTag, LEN_AUTH_TAG)){
        ERR_vPrintln(T_TRUE, "Verify authTag Failed");
        FREE(psEncryptedData);
        goto AuthenticationError;
    }
    DBG_vPrintln(DBG_PAIR, "Verify authTag Success");
    //TODO:(M5 Verification 2) Decrypt the sub-TLV
    uint8 *psDecryptedData = (uint8*)malloc(u16LenEncryptData);
    CHECK_POINTER(psDecryptedData, E_PAIRING_STATUS_ERROR);
    memset(psDecryptedData, 0, u16LenEncryptData);
    chacha20_encrypt(&context, psEncryptedData, psDecryptedData, u16LenEncryptData);//Decrypt Data
    FREE(psEncryptedData);

    tsTlvType sTlvIOSDevicePairingID, sTlvIOSDeviceLTPK, sTlvSignature;
    memset(&sTlvIOSDevicePairingID, 0, sizeof(sTlvIOSDevicePairingID));
    memset(&sTlvIOSDeviceLTPK, 0, sizeof(sTlvIOSDeviceLTPK));
    memset(&sTlvSignature, 0, sizeof(sTlvSignature));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_IDENTIFIER, psDecryptedData, (uint16)u16LenEncryptData, &sTlvIOSDevicePairingID)){
        ERR_vPrintln(T_TRUE, "Can't find E_TLV_VALUE_TYPE_IDENTIFIER");
        FREE(psDecryptedData); goto AuthenticationError;
    }
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY, psDecryptedData, (uint16)u16LenEncryptData, &sTlvIOSDeviceLTPK)){
        ERR_vPrintln(T_TRUE, "Can't find E_TLV_VALUE_TYPE_PUBLIC_KEY");
        FREE(psDecryptedData); FREE(sTlvIOSDevicePairingID.psValue); goto AuthenticationError;
    }
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_SIGNATURE, psDecryptedData, (uint16)u16LenEncryptData, &sTlvSignature)){
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
    if(hkdf((const unsigned char*)salt, sizeof(salt), (const unsigned char*)pShareKey->data,
            pShareKey->length, (const unsigned char*)info, sizeof(info), auIOSDeviceX, LEN_HKDF_LEN)){
        ERR_vPrintln(T_TRUE, "HKDF-SHA-512 Failed");
        FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue);FREE(sTlvSignature.psValue);
        return E_PAIRING_STATUS_ERROR;
    }
    //TODO:(M5 Verification 4) Construct IOSDeviceInfo by concatenating IOSDeviceX with the IOSDevicePairingID and IOSDeviceLTPK
    uint8 auIOSDeviceInfo[100] = {0};
    memcpy(auIOSDeviceInfo, auIOSDeviceX, sizeof(auIOSDeviceX));
    memcpy(&auIOSDeviceInfo[32], sTlvIOSDevicePairingID.psValue, 36);
    memcpy(&auIOSDeviceInfo[32 + 36], sTlvIOSDeviceLTPK.psValue, 32);
    PrintArray(DBG_PAIR, auIOSDeviceInfo, 100);
    //TODO:(M5 Verification 5) Use Ed25519 to verify the signature of the constructed IOSDeviceInfo with the IOSDeviceLTPK form the decrypted sub-TLV
    if(ed25519_sign_open(auIOSDeviceInfo, sizeof(auIOSDeviceInfo), sTlvIOSDeviceLTPK.psValue, sTlvSignature.psValue)){
        ERR_vPrintln(T_TRUE, "ed25519_sign_open Failed");
        FREE(sTlvIOSDevicePairingID.psValue); FREE(sTlvIOSDeviceLTPK.psValue);FREE(sTlvSignature.psValue);
        goto AuthenticationError;
    }
    DBG_vPrintln(DBG_PAIR, "Verify IOSDeviceInfo Success");
    //TODO:(M5 Verification 6) Persistently save the IOSDevicePairingId and IOSDeviceLTPK as a pairing

    //TODO:(M6 Response 1) Generate its Ed25519 long-term public key,AccessoryLTPK and long-term secret key,AccessoryLTSK
    uint8 auAccessoryLTSK[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2,
                                 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67,
                                 0xCC, 0x74};
    uint8 auAccessoryLTPK[32] = {0};
    ed25519_publickey(auAccessoryLTSK, auAccessoryLTPK);
    //TODO:(M6 Response 2) Derive AccessoryX from the SRP shared secret by using HKDF-SHA-512
    const char salt2[] = "Pair-Setup-Accessory-Sign-Salt";
    const char info2[] = "Pair-Setup-Accessory-Sign-Info";
    uint8_t auAccessoryX[32];
    int ret = hkdf((const unsigned char*)salt2, (int)strlen(salt2), (const unsigned char*)pShareKey->data,
               pShareKey->length, (const unsigned char*)info2, (int)strlen(info2), auAccessoryX, 32);
    if(ret){
        ERR_vPrintln(T_TRUE, "HKDF Error");
        return E_PAIRING_STATUS_ERROR;
    }
    //TODO:(M6 Response 3) Concatenate AccessoryX with AccessoryPairingID and AccessoryLTPK to AccessoryInfo
    uint8 auAccessoryInfo[150] = {0};
    memcpy(auAccessoryInfo, auAccessoryX, sizeof(auAccessoryX));
    memcpy(&auAccessoryInfo[32], "12:10:34:23:51:12", strlen("12:10:34:23:51:12"));
    memcpy(&auAccessoryInfo[32+strlen("12:10:34:23:51:12")], auAccessoryLTPK, 32);
    //TODO:(M6 Response 4) Use Ed25519 to generate AccessorySignature by siging AccessoryInfo with AccessoryLTSK
    uint8 auAccessorySignature[64] = {0};
    ed25519_sign(auAccessoryInfo, 64+strlen("12:10:34:23:51:12"), auAccessoryLTSK, auAccessoryLTPK, auAccessorySignature);
    //TODO:(M6 Response 5) Construct the sub-TLV with AccessoryPairingID,AccessoryLTPK,AccessorySignature
    tsTlvType sTlvSubTlv;memset(&sTlvSubTlv, 0, sizeof(sTlvSubTlv));
    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_IDENTIFIER, (uint16)strlen("12:10:34:23:51:12"), "12:10:34:23:51:12");
    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_PUBLIC_KEY, sizeof(auAccessoryLTSK), auAccessoryLTSK);
    eTlvTypeFormatAdd(&sTlvSubTlv, E_TLV_VALUE_TYPE_SIGNATURE, sizeof(auAccessorySignature), auAccessorySignature);
    //TODO:(M6 Response 6) Encrypt the sub-TLV,encryptedData,and generate the 16 byte auth tag,authTag, uses the ChaCha20-Poly1305 AEAD algorithm
    uint8 *pauEncryData = (uint8*)malloc(sTlvSubTlv.u16Len + 16);
    CHECK_POINTER(pauEncryData, E_PAIRING_STATUS_ERROR);
    memset(pauEncryData, 0, sTlvSubTlv.u16Len + 16);
    chacha20_ctx ctx;memset(&ctx, 0, sizeof(ctx));
    chacha20_setup(&ctx, (const uint8 *)auSessionKey, 32, (uint8 *)"PS-Msg06");
    uint8 buffer[64] = {0}, key[64] = {0};
    chacha20_encrypt(&ctx, buffer, key, 64);
    chacha20_encrypt(&ctx, sTlvSubTlv.psValue, pauEncryData, sTlvSubTlv.u16Len);

    char authTag[16] = {0};
    Poly1305_GenKey(key, pauEncryData, sTlvSubTlv.u16Len, T_FALSE, authTag);
    memcpy(&pauEncryData[sTlvSubTlv.u16Len], authTag, 16);
    //TODO:(M6 Response 7) Send the response to the IOS device with TLV items
    eTlvTypeFormatAdd(&sTlvResponse, E_TLV_VALUE_TYPE_ENCRYPTED_DATA, (uint16)(sTlvSubTlv.u16Len + 16), pauEncryData);
    psHttpEntry->iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, psHttpEntry, sTlvResponse.psValue, sTlvResponse.u16Len);
    FREE(sTlvResponse.psValue);FREE(sTlvSubTlv.psValue);FREE(pauEncryData);
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

    DBG_vPrintln(DBG_PAIR, "eTlvTypeGetObject[%d][%d]", psTlvType->u8Type, psTlvType->u16Len);
    PrintArray(DBG_PAIR, psTlvType->psValue, psTlvType->u16Len);

    return E_PAIRING_STATUS_OK;
}

static void Poly1305_GenKey(const unsigned char * key, uint8_t * buf, uint16_t len, bool_t bWithLen, char* verify)
{
    printf("Length: %d\n", buf[0]);
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
    }
    else {
        poly1305_update(&verifyContext, (const unsigned char *)buf, len);
    }

    if (len%16 > 0)
        poly1305_update(&verifyContext, (const unsigned char *)waste, 16-(len%16));
    unsigned char _len;
    if (bWithLen) {
        _len = 2;
    }
    else {
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
