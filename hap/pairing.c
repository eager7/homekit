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
#include "poly1305.h"
#include "hkdf.h"
#include "ip.h"
#include "bonjour.h"
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

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
const uint8 generator[] = {0x05};
const uint8 modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD, 0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED, 0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D, 0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F, 0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B, 0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10, 0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64, 0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7, 0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0, 0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B, 0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C, 0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31, 0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8 accessorySecretKey[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74};
const uint8 curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static tsController sController;
static tsPairSetup sPairSetup;
static tsPairVerify sPairVerify;
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static tePairStatus ePoly1305_GenKey(const uint8 *key, uint8 *buf, uint16 len, bool_t bWithLen, char *verify)
{
    if (key == NULL || buf == NULL || len < 2 || verify == NULL)
        return E_PAIRING_STATUS_ERROR;

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
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDevicePairingIDSave(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_PAIRING_STATUS_ERROR);
    FILE *fp = fopen("IOSDevicePairingID.txt", "w");
    CHECK_POINTER(fp, E_PAIRING_STATUS_ERROR);
    if(len != fwrite(buf,1,len,fp)){
        fclose(fp);
        return E_PAIRING_STATUS_ERROR;
    }
    fclose(fp);
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDevicePairingIDRead(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_PAIRING_STATUS_ERROR);
    FILE *fp = fopen("IOSDevicePairingID.txt", "r");
    CHECK_POINTER(fp, E_PAIRING_STATUS_ERROR);
    if(len != fread(buf,1,len,fp)){
        fclose(fp);
        return E_PAIRING_STATUS_ERROR;
    }
    fclose(fp);
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDeviceLTPKSave(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_PAIRING_STATUS_ERROR);
    FILE *fp = fopen("IOSDeviceLTPK.txt", "w");
    CHECK_POINTER(fp, E_PAIRING_STATUS_ERROR);
    if(len != fwrite(buf,1,len,fp)){
        fclose(fp);
        return E_PAIRING_STATUS_ERROR;
    }
    fclose(fp);
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eIOSDeviceLTPKRead(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_PAIRING_STATUS_ERROR);
    FILE *fp = fopen("IOSDeviceLTPK.txt", "r");
    CHECK_POINTER(fp, E_PAIRING_STATUS_ERROR);
    if(len != fread(buf,1,len,fp)){
        fclose(fp);
        return E_PAIRING_STATUS_ERROR;
    }
    fclose(fp);
    return E_PAIRING_STATUS_OK;
}
static tePairStatus eM2SrpStartResponse(int iSockFd, char *pSetupCode, tsIpMessage *psIpMsg)
{
    uint8 *pRespBuffer = NULL;
    uint16 u16RespLen = 0;
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_PAIRING_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;

    uint8 auSaltChar[16];
    for (int i = 0; i < 16; i++) {
        auSaltChar[i] = rand();
    }
    SRP_RESULT Ret = SRP_set_username(sPairSetup.pSrp, "Pair-Setup");
    int modulusSize = sizeof(modulusStr) / sizeof(modulusStr[0]);
    Ret = SRP_set_params(sPairSetup.pSrp, modulusStr, sizeof(modulusStr), generator, sizeof(generator), auSaltChar, sizeof(auSaltChar));
    Ret = SRP_set_auth_password(sPairSetup.pSrp, pSetupCode);
    cstr *pPublicKey = NULL;
    Ret = SRP_gen_pub(sPairSetup.pSrp, &pPublicKey);
    uint8 value_rep[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SALT,auSaltChar,16,psTlvRespMessage);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,(uint8*)pPublicKey->data,(uint16)pPublicKey->length,psTlvRespMessage);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);

    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, &psIpMsg->sHttp, pRespBuffer, u16RespLen);
    eIpMessageRelease(psResponse);
    FREE(pRespBuffer);

    return E_PAIRING_STATUS_OK;
}
static tePairStatus eM4SrpVerifyResponse(int iSockFd, tsIpMessage *psIpMsg)
{
    uint8 *pRespBuffer = NULL;
    uint16 u16RespLen = 0;
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    uint8 value_rep[] = {E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE};
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_PAIRING_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
    tePairStatus eStatus = E_PAIRING_STATUS_OK;

    uint16 u16PublicKeyLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
    uint8 *psPublicKeyBuf  = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
    CHECK_POINTER(psPublicKeyBuf, E_PAIRING_STATUS_ERROR);
    uint16 u16ProofLen     = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_PROOF);
    uint8 *psProofBuf      = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_PROOF);
    CHECK_POINTER(psProofBuf, E_PAIRING_STATUS_ERROR);

    SRP_RESULT Ret = SRP_compute_key(sPairSetup.pSrp, &sPairSetup.pSecretKey, psPublicKeyBuf, u16PublicKeyLen);
    Ret = SRP_verify(sPairSetup.pSrp, psProofBuf, u16ProofLen);
    if (!SRP_OK(Ret)) {
        ERR_vPrintln(T_TRUE, "Setup Code InCorrect");
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        eStatus = E_PAIRING_STATUS_ERROR;
        goto Finished;
    } else {
        cstr *psRespProof = NULL;
        SRP_respond(sPairSetup.pSrp, &psRespProof);
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PROOF, (uint8*)psRespProof->data, (uint16)psRespProof->length, psTlvRespMessage);
        NOT_vPrintln(DBG_PAIR, "Password Correct\n");
    }
    const char salt[] = "Pair-Setup-Encrypt-Salt";
    const char info[] = "Pair-Setup-Encrypt-Info";
    if (0 != hkdf((const unsigned char*)salt, (int)strlen(salt), (const unsigned char*)sPairSetup.pSecretKey->data,
                  sPairSetup.pSecretKey->length, (const unsigned char*)info, (int)strlen(info), sPairSetup.auSessionKey, LEN_HKDF_LEN)) {
        ERR_vPrintln(T_TRUE, "HKDF Failed");
        return E_PAIRING_STATUS_ERROR;
    }
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, &psIpMsg->sHttp, pRespBuffer, u16RespLen);
    eIpMessageRelease(psResponse);
    FREE(pRespBuffer);

    return eStatus;
}
static tePairStatus eM6ExchangeResponse(int iSockFd, uint8 *psDeviceID, tsIpMessage *psIpMsg)
{
    uint8 *pRespBuffer = NULL;
    uint16 u16RespLen = 0;
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    uint8 value_rep[] = {E_PAIR_SETUP_M6_EXCHANGE_RESPONSE};
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_PAIRING_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
    tePairStatus eStatus = E_PAIRING_STATUS_OK;

    uint8 *psEncryptedPackage = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint16 u16EncryptedLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint8 *psEncryptedData = malloc(u16EncryptedLen);
    memcpy(psEncryptedData, psEncryptedPackage, (size_t)(u16EncryptedLen - LEN_AUTH_TAG));
    uint8 auAuthTag[LEN_AUTH_TAG];
    memcpy(auAuthTag, &psEncryptedPackage[u16EncryptedLen - LEN_AUTH_TAG], sizeof(auAuthTag));

    chacha20_ctx context;
    memset(&context, 0, sizeof(context));
    chacha20_setup(&context, sPairSetup.auSessionKey, 32, (uint8*)"PS-Msg05");

    char auInKey[64] = {0};
    char auOutKey[64] = {0};
    chacha20_encrypt(&context, (const uint8*)auInKey, (uint8*)auOutKey, 64);

    char auVerify[16] = {0};
    ePoly1305_GenKey((const uint8*)auOutKey, psEncryptedData, (uint16) (u16EncryptedLen - 16), T_FALSE,
                     auVerify);

    uint8 *psDecryptedData = (uint8*)malloc((size_t)(u16EncryptedLen - LEN_AUTH_TAG));
    memset(psDecryptedData, 0, (size_t)(u16EncryptedLen - LEN_AUTH_TAG));
    chacha20_decrypt(&context, (const uint8*)psEncryptedData, psDecryptedData, (size_t)(u16EncryptedLen - LEN_AUTH_TAG));
    FREE(psEncryptedData);
    if(memcmp(auVerify, auAuthTag, LEN_AUTH_TAG)) {
        FREE(psDecryptedData);
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        eStatus = E_PAIRING_STATUS_ERROR;
        goto Finished;
    }else {
        tsTlvPackage *psSubTlvPackage = psTlvPackageFormat(psDecryptedData, (uint16) (u16EncryptedLen - LEN_AUTH_TAG));
        FREE(psDecryptedData);
        uint8 *psControllerIdentifier = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_IDENTIFIER);
        uint8 *psControllerPublicKey = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
        uint8 *psControllerSignature = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_SIGNATURE);
        uint8 auControllerHash[100];

        DBG_vPrintln(DBG_PAIR,"eIOSDevicePairingIDSave & eIOSDeviceLTPKSave");
        eIOSDevicePairingIDSave(psControllerIdentifier, 36);
        eIOSDeviceLTPKSave(psControllerPublicKey, 32);
        memcpy(sPairSetup.auControllerIdentifier, psControllerIdentifier, 36);
        memcpy(sPairSetup.auControllerPublicKey, psControllerPublicKey,  32);

        const char salt[] = "Pair-Setup-Controller-Sign-Salt";
        const char info[] = "Pair-Setup-Controller-Sign-Info";
        if (0 != hkdf((const uint8*)salt, (int)strlen(salt), (const uint8*)sPairSetup.pSecretKey->data, sPairSetup.pSecretKey->length,
                      (const uint8*)info, (int)strlen(info), (uint8_t*)auControllerHash, 32)) {
            eTlvPackageRelease(psSubTlvPackage);
            ERR_vPrintln(T_TRUE, "HKDF Failed");
            return E_PAIRING_STATUS_ERROR;
        }
        memcpy(&auControllerHash[32], psControllerIdentifier, 36);
        memcpy(&auControllerHash[68], psControllerPublicKey,  32);

        int ed25519_err = ed25519_sign_open(auControllerHash, 100, psControllerPublicKey, psControllerSignature);
        eTlvPackageRelease(psSubTlvPackage);
        if (ed25519_err) {
            ERR_vPrintln(T_TRUE, "ed25519_sign_open error");
            return E_PAIRING_STATUS_ERROR;
        } else {
            DBG_vPrintln(DBG_PAIR, "ed25519_sign_open success");
            tsTlvPackage *pReturnTlvPackage = psTlvPackageNew();
            pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_IDENTIFIER,psDeviceID,17,&pReturnTlvPackage->sMessage);

            const uint8 salt2[] = "Pair-Setup-Accessory-Sign-Salt";
            const uint8 info2[] = "Pair-Setup-Accessory-Sign-Info";
            uint8_t auAccessoryInfo[150];
            hkdf(salt2, (int)strlen(salt2), (const uint8*)sPairSetup.pSecretKey->data, sPairSetup.pSecretKey->length,
                 info2, (int)strlen(info2), auAccessoryInfo, LEN_HKDF_LEN);
            memcpy(&auAccessoryInfo[32], psDeviceID, 17);

            uint8 auSignature[64] = {0};
            ed25519_secret_key edSecret;
            memcpy(edSecret, accessorySecretKey, sizeof(edSecret));
            ed25519_public_key edPubKey;
            ed25519_publickey(edSecret, edPubKey);

            memcpy(&auAccessoryInfo[32 + 17], edPubKey, 32);
            ed25519_sign(auAccessoryInfo, 64 + 17, (const uint8*)edSecret, (const uint8*)edPubKey, auSignature);

            pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SIGNATURE,auSignature,64,&pReturnTlvPackage->sMessage);
            pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,edPubKey,32,&pReturnTlvPackage->sMessage);

            uint8 *tlv8Data = NULL;
            uint16 tlv8Len = 0;
            pReturnTlvPackage->eTlvMessageGetData(&pReturnTlvPackage->sMessage, &tlv8Data, &tlv8Len);

            uint8 *tlv8Recorddata = malloc(tlv8Len+16);
            int tlv8Recordlength = tlv8Len+16;
            memset(tlv8Recorddata, 0, (size_t)tlv8Recordlength);

            chacha20_ctx ctx;   bzero(&ctx, sizeof(ctx));
            chacha20_setup(&ctx, sPairSetup.auSessionKey, 32, (uint8 *)"PS-Msg06");
            uint8 buffer[64] = {0}, key[64] = {0};
            chacha20_encrypt(&ctx, buffer, key, 64);
            chacha20_encrypt(&ctx, tlv8Data, tlv8Recorddata, tlv8Len);

            uint8 verify[16] = {0};
            ePoly1305_GenKey(key, tlv8Recorddata, tlv8Len, T_FALSE, auVerify);
            memcpy(&tlv8Recorddata[tlv8Len], auVerify, 16);
            psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,tlv8Recorddata,(uint16)tlv8Recordlength,psTlvRespMessage);
            eTlvPackageRelease(pReturnTlvPackage);
        }
    }
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, &psIpMsg->sHttp, pRespBuffer, u16RespLen);
    eIpMessageRelease(psResponse);
    FREE(pRespBuffer);

    return eStatus;
}
static tePairStatus eM2VerifyStartResponse(int iSockFd, uint8 *psDeviceID, tsIpMessage *psIpMsg)
{
    uint8 *psRepBuffer = 0;
    uint16 u16RepLen = 0;
    uint8 value_rep[1] = {E_PAIR_VERIFY_M2_START_RESPONSE};
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    tsIpMessage *psResponse = psIpResponseNew();
    tsTlvMessage *psTlvRespMsg = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
    tePairStatus eStatus = E_PAIRING_STATUS_OK;

    memcpy(sPairVerify.auControllerPublicKey, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg, E_TLV_VALUE_TYPE_PUBLIC_KEY), 32);
    curved25519_key auSecretKey;
    for (int i = 0; i < sizeof(auSecretKey); i++) {
        auSecretKey[i] = (uint8)rand();
    }
    curve25519_donna(sPairVerify.auPublicKey, auSecretKey, curveBasePoint);
    curve25519_donna(sPairVerify.auSharedKey, auSecretKey, sPairVerify.auControllerPublicKey);

    uint8 auAccessoryInfo[100] = {0};
    memcpy(auAccessoryInfo, sPairVerify.auPublicKey, 32);
    memcpy(&auAccessoryInfo[32], psDeviceID, LEN_DEVICE_ID);
    memcpy(&auAccessoryInfo[32 + LEN_DEVICE_ID], sPairVerify.auControllerPublicKey, 32);

    ed25519_secret_key edSecret;
    memcpy(edSecret, accessorySecretKey, sizeof(edSecret));
    ed25519_public_key edPubKey;
    ed25519_publickey(edSecret, edPubKey);
    uint8 auSignRecordData[64] = {0};
    ed25519_sign(auAccessoryInfo, 64 + LEN_DEVICE_ID, edSecret, edPubKey, auSignRecordData);

    uint8 auIdRecordData[LEN_DEVICE_ID] = {0};
    memcpy(auIdRecordData, psDeviceID, LEN_DEVICE_ID);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,sPairVerify.auPublicKey,32,psTlvRespMsg);

    tsTlvPackage* psSubTlvPackage = psTlvPackageNew();
    psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SIGNATURE,auSignRecordData,sizeof(auSignRecordData),&psSubTlvPackage->sMessage);
    psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_IDENTIFIER,auIdRecordData,LEN_DEVICE_ID,&psSubTlvPackage->sMessage);

    const char salt[] = "Pair-Verify-Encrypt-Salt";
    const char info[] = "Pair-Verify-Encrypt-Info";
    hkdf((const uint8*)salt, (int)strlen(salt), sPairVerify.auSharedKey, 32, (const uint8*)info, (int)strlen(info), sPairVerify.auEnKey, 32);
    uint8 *psSubMsgData = NULL;
    uint16 u16SubMsgLen = 0;
    psSubTlvPackage->eTlvMessageGetData(&psSubTlvPackage->sMessage,&psSubMsgData, &u16SubMsgLen);
    eTlvPackageRelease(psSubTlvPackage);

    uint8 *psEncryptedData = (uint8*)malloc(u16SubMsgLen + 16);
    uint8 polyKey[64] = {0};
    uint8 zero[64] = {0};
    chacha20_ctx chacha;
    chacha20_setup(&chacha, sPairVerify.auEnKey, 32, (uint8*)"PV-Msg02");
    chacha20_encrypt(&chacha, zero, polyKey, 64);
    chacha20_encrypt(&chacha, psSubMsgData, psEncryptedData, u16SubMsgLen);
    FREE(psSubMsgData);

    char verify[16] = {0};
    ePoly1305_GenKey(polyKey, psEncryptedData, u16SubMsgLen, T_FALSE, verify);
    memcpy(&psEncryptedData[u16SubMsgLen], verify, 16);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,psEncryptedData,(uint16)(u16SubMsgLen + 16),psTlvRespMsg);
    FREE(psEncryptedData);

    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMsg);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMsg,&psRepBuffer,&u16RepLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, &psIpMsg->sHttp, psRepBuffer, u16RepLen);
    eIpMessageRelease(psResponse);
    FREE(psRepBuffer);
    return eStatus;
}
static tePairStatus eM4VerifyFinishResponse(int iSockFd, tsIpMessage *psIpMsg)
{
    uint8 *psRepBuffer = 0;
    uint16 u16RepLen = 0;
    uint8 value_rep[1] = {E_PAIR_VERIFY_M2_START_RESPONSE};
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    tsIpMessage *psResponse = psIpResponseNew();
    tsTlvMessage *psTlvRespMsg = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
    tePairStatus eStatus = E_PAIRING_STATUS_OK;

    uint8 *psEncryptedPackData = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint16 u16EncryptedPackLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint16 u16EncryptedLen = u16EncryptedPackLen - (uint16)LEN_AUTH_TAG;

    chacha20_ctx chacha20;
    memset(&chacha20, 0, sizeof(chacha20));
    chacha20_setup(&chacha20, sPairVerify.auEnKey, 32, (uint8*)"PV-Msg03");
    uint8 auIn[64] = {0}, auOut[64] = {0};
    chacha20_encrypt(&chacha20, auIn, auOut, 64);

    char verify[16] = {0};
    ePoly1305_GenKey(auOut, psEncryptedPackData, u16EncryptedLen, T_FALSE, verify);
    if (!bcmp(verify, &psEncryptedPackData[u16EncryptedLen], LEN_AUTH_TAG)) {
        uint8 *psEncryptedData = (uint8*)malloc(u16EncryptedLen);
        chacha20_decrypt(&chacha20, psEncryptedPackData, psEncryptedData, u16EncryptedLen);
        tsTlvPackage *psSubTlvPack = psTlvPackageFormat(psEncryptedData, u16EncryptedLen);
        FREE(psEncryptedData);

        uint8 *controllerID = psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_IDENTIFIER);
        uint8 key[36] = {0};
        uint8 recpublicKey[32] = {0};
        eIOSDevicePairingIDRead(key, 36);
        if(bcmp(key, controllerID, 36) == 0){
            eIOSDeviceLTPKRead(recpublicKey, 32);
        } else{
            ERR_vPrintln(T_TRUE, "controllerID verify failed");
            return E_PAIRING_STATUS_ERROR;
        }

        uint8 auIOSDeviceInfo[100];
        memcpy(auIOSDeviceInfo, sPairVerify.auControllerPublicKey, 32);
        memcpy(&auIOSDeviceInfo[32], controllerID, 36);
        memcpy(&auIOSDeviceInfo[68], sPairVerify.auPublicKey, 32);

        int err = ed25519_sign_open(auIOSDeviceInfo, 100, recpublicKey, psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_SIGNATURE));
        eTlvPackageRelease(psSubTlvPack);
        if (err == 0) {
            hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32, (uint8*)"Control-Read-Encryption-Key", 27, sController.auAccessoryToControllerKey, 32);
            hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32, (uint8*)"Control-Write-Encryption-Key", 28, sController.auControllerToAccessoryKey, 32);
            INF_vPrintln(DBG_PAIR, "Verify success\n");
        } else {
            psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
            ERR_vPrintln(DBG_PAIR, "Verify failed\n");
            eStatus = E_PAIRING_STATUS_ERROR;
            goto Finished;
        }
    } else{
        ERR_vPrintln(T_TRUE, "Error verify");
        return E_PAIRING_STATUS_ERROR;
    }
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMsg);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMsg,&psRepBuffer,&u16RepLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(iSockFd, &psIpMsg->sHttp, psRepBuffer, u16RepLen);
    eIpMessageRelease(psResponse);
    FREE(psRepBuffer);
    return eStatus;
}

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tePairStatus ePairingInit()
{
    memset(&sController, 0, sizeof(tsController));
    memset(&sPairSetup, 0, sizeof(tsPairSetup));
    memset(&sPairVerify, 0, sizeof(tsPairVerify));
    eLockCreate(&sPairSetup.mutex);
    return E_PAIRING_STATUS_OK;
}
tePairStatus ePairingFinished()
{
    return E_PAIRING_STATUS_OK;
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
    return x;
}
teHapStatus eHandleAccessoryRequest(int iSocketFd, tsBonjour *psBonjour)
{
    DBG_vPrintln(DBG_PAIR, "Successfully Connect\n");

    uint8 auDecryptedData[MABF] = {0};
    sController.u64NumMsgRec = 0;
    sController.u64NumMsgSend = 0;
    do {
        memset(sController.auBuffer, 0, sizeof(sController.auBuffer));
        sController.iLen = (int)recv(iSocketFd, sController.auBuffer, sizeof(sController.auBuffer), 0);
        if (sController.iLen < 0){
            ERR_vPrintln(T_TRUE, "Recvice Data Error");
            return E_HAP_STATUS_ERROR;
        }
        //uint16 u16MsgLen = (sController.auBuffer[0] | ((uint16)sController.auBuffer[1] << 0xff));
        uint16 u16MsgLen = (uint16)((uint8)sController.auBuffer[1]*256 + (uint8)*sController.auBuffer);

        chacha20_ctx chacha20;
        memset(&chacha20, 0, sizeof(chacha20));

        if (!is_big_endian()) sController.u64NumMsgRec = bswap_64(sController.u64NumMsgRec);
        chacha20_setup(&chacha20, sController.auControllerToAccessoryKey, 32, (uint8*)&sController.u64NumMsgRec);
        if (!is_big_endian()) sController.u64NumMsgRec = bswap_64(sController.u64NumMsgRec);
        sController.u64NumMsgRec++;

        uint8 temp[64] = {0}, temp2[64] = {0};
        chacha20_encrypt(&chacha20, temp, temp2, 64);

        char verify[16] = {0};
        ePoly1305_GenKey(temp2, sController.auBuffer, u16MsgLen, T_TRUE, verify);
        memset(auDecryptedData, 0, sizeof(auDecryptedData));
        chacha20_encrypt(&chacha20, &sController.auBuffer[2], auDecryptedData, u16MsgLen);
        DBG_vPrintln(DBG_PAIR, "Request: %s\nPacketLen: %d, MessageLen: %d\n", auDecryptedData, sController.iLen, (int)strlen(auDecryptedData));

        if(sController.iLen >= (2 + u16MsgLen + 16) && memcmp((void *)verify, (void *)&sController.auBuffer[2 + u16MsgLen], 16) == 0) {
            NOT_vPrintln(DBG_PAIR, "Verify Successfully!\n");
        }
        else {
            WAR_vPrintln(T_TRUE, "Verify Failed");
            continue;
        }
        uint8 *psRetData = 0;
        uint16 u16RetLen = 0;
        eHandleAccessoryPackage(auDecryptedData, u16MsgLen, &psRetData, &u16RetLen);
        DBG_vPrintln(DBG_PAIR, "Send:%s", psRetData);
        //18 = 2(resultLen) + 16(poly1305 verify key)
        uint8 *psRespBuf = (uint8*)malloc(u16RetLen + 18);
        psRespBuf[0] = (uint8)(u16RetLen % 256);
        psRespBuf[1] = (uint8)((u16RetLen - psRespBuf[0]) / 256);

        if (!is_big_endian()) sController.u64NumMsgSend = bswap_64(sController.u64NumMsgSend);
        chacha20_setup(&chacha20, (const uint8_t *)sController.auAccessoryToControllerKey, 32, (uint8_t *)&sController.u64NumMsgSend);
        if (!is_big_endian()) sController.u64NumMsgSend = bswap_64(sController.u64NumMsgSend);
        sController.u64NumMsgSend++;

        chacha20_encrypt(&chacha20, temp, temp2, 64);
        chacha20_encrypt(&chacha20, psRetData, &psRespBuf[2], u16RetLen);

        ePoly1305_GenKey((const unsigned char *) temp2, psRespBuf, u16RetLen, T_TRUE, verify);
        memcpy(&psRespBuf[u16RetLen+2], verify, 16);
        write(iSocketFd, psRespBuf, u16RetLen+18);
        FREE(psRespBuf);
        FREE(psRetData);
    }while (sController.iLen > 0);

    return E_HAP_STATUS_OK;
}

tePairStatus eHandlePairSetup(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    CHECK_POINTER(psBuffer, E_PAIRING_STATUS_ERROR);
    CHECK_POINTER(psBonjour, E_PAIRING_STATUS_ERROR);

    uint8 value_rep[1] = {0};
    tsIpMessage *psIpMsg = NULL;
    psIpMsg = psIpMessageFormat(psBuffer, (uint16)iLen);
    tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
    uint8 *pStateController = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_STATE);
    CHECK_POINTER(pStateController, E_PAIRING_STATUS_ERROR);
    memcpy(&sPairSetup.eState, pStateController, 1);
    INF_vPrintln(DBG_PAIR, "State:%d", sPairSetup.eState);
    value_rep[0] = sPairSetup.eState + 1;
    switch (sPairSetup.eState) {
        case E_PAIR_SETUP_M1_SRP_START_REQUEST: {
            INF_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M1_SRP_START_REQUEST");
            sPairSetup.pSrp = SRP_new(SRP6a_server_method());
            eM2SrpStartResponse(iSocketFd, psBonjour->pcSetupCode, psIpMsg);
        }break;
        case E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST: {
            DBG_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST");
            eM4SrpVerifyResponse(iSocketFd, psIpMsg);
        }break;
        case E_PAIR_SETUP_M5_EXCHANGE_REQUEST: {
            DBG_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M5_EXCHANGE_REQUEST");
            eM6ExchangeResponse(iSocketFd, psBonjour->sBonjourText.psDeviceID, psIpMsg);
            SRP_free(sPairSetup.pSrp);
            eUpdateConfiguration(psBonjour);
        }
        default:
            break;
    }
    eIpMessageRelease(psIpMsg);
    return E_PAIRING_STATUS_OK;
}

tePairStatus  eHandlePairVerify(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    CHECK_POINTER(psBuffer, E_PAIRING_STATUS_ERROR);
    CHECK_POINTER(psBonjour, E_PAIRING_STATUS_ERROR);

    sController.iSockFd = iSocketFd;
    sController.iLen = iLen;
    memcpy(sController.auBuffer, psBuffer, sizeof(sController.auBuffer));

    bool_t end = T_FALSE;

    uint8 value_rep[1] = {0};
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    tsIpMessage *psIpMsg = NULL, *psResponse = NULL;
    uint8 *psRepBuffer = 0;
    uint16 u16RepLen = 0;
    do {
        psIpMsg = psIpMessageFormat(sController.auBuffer, (uint16)sController.iLen);
        tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
        psResponse = psIpResponseNew();
        tsTlvMessage *psTlvRespMsg = &psResponse->psTlvPackage->sMessage;
        memcpy(&sPairVerify.eState, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg, E_TLV_VALUE_TYPE_STATE), 1);
        value_rep[0] = sPairVerify.eState + 1;
        switch (sPairVerify.eState) {
            case E_PAIR_VERIFY_M1_START_REQUEST:{
                NOT_vPrintln(DBG_PAIR, "E_PAIR_VERIFY_M1_START_REQUEST\n");
                //eM2VerifyStartResponse(iSocketFd, psBonjour->sBonjourText.psDeviceID, psIpMsg);
                memcpy(sPairVerify.auControllerPublicKey, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg, E_TLV_VALUE_TYPE_PUBLIC_KEY), 32);
                curved25519_key auSecretKey;
                for (int i = 0; i < sizeof(auSecretKey); i++) {
                    auSecretKey[i] = (uint8)rand();
                }
                curve25519_donna(sPairVerify.auPublicKey, auSecretKey, curveBasePoint);
                curve25519_donna(sPairVerify.auSharedKey, auSecretKey, sPairVerify.auControllerPublicKey);

                uint8 auAccessoryInfo[100] = {0};
                memcpy(auAccessoryInfo, sPairVerify.auPublicKey, 32);
                memcpy(&auAccessoryInfo[32], psBonjour->sBonjourText.psDeviceID, LEN_DEVICE_ID);
                memcpy(&auAccessoryInfo[32 + LEN_DEVICE_ID], sPairVerify.auControllerPublicKey, 32);

                ed25519_secret_key edSecret;
                memcpy(edSecret, accessorySecretKey, sizeof(edSecret));
                ed25519_public_key edPubKey;
                ed25519_publickey(edSecret, edPubKey);
                uint8 auSignRecordData[64] = {0};
                ed25519_sign(auAccessoryInfo, 64 + LEN_DEVICE_ID, edSecret, edPubKey, auSignRecordData);

                uint8 auIdRecordData[LEN_DEVICE_ID] = {0};
                memcpy(auIdRecordData, psBonjour->sBonjourText.psDeviceID, LEN_DEVICE_ID);
                psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,sPairVerify.auPublicKey,32,psTlvRespMsg);

                tsTlvPackage* psSubTlvPackage = psTlvPackageNew();
                psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SIGNATURE,auSignRecordData,sizeof(auSignRecordData),&psSubTlvPackage->sMessage);
                psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_IDENTIFIER,auIdRecordData,LEN_DEVICE_ID,&psSubTlvPackage->sMessage);

                const char salt[] = "Pair-Verify-Encrypt-Salt";
                const char info[] = "Pair-Verify-Encrypt-Info";
                hkdf((const uint8*)salt, (int)strlen(salt), sPairVerify.auSharedKey, 32, (const uint8*)info, (int)strlen(info), sPairVerify.auEnKey, 32);
                uint8 *psSubMsgData = NULL;
                uint16 u16SubMsgLen = 0;
                psSubTlvPackage->eTlvMessageGetData(&psSubTlvPackage->sMessage,&psSubMsgData, &u16SubMsgLen);
                eTlvPackageRelease(psSubTlvPackage);

                uint8 *psEncryptedData = (uint8*)malloc(u16SubMsgLen + 16);
                uint8 polyKey[64] = {0};
                uint8 zero[64] = {0};
                chacha20_ctx chacha;
                chacha20_setup(&chacha, sPairVerify.auEnKey, 32, (uint8*)"PV-Msg02");
                chacha20_encrypt(&chacha, zero, polyKey, 64);
                chacha20_encrypt(&chacha, psSubMsgData, psEncryptedData, u16SubMsgLen);
                FREE(psSubMsgData);

                char verify[16] = {0};
                ePoly1305_GenKey(polyKey, psEncryptedData, u16SubMsgLen, T_FALSE, verify);
                memcpy(&psEncryptedData[u16SubMsgLen], verify, 16);
                psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,psEncryptedData,(uint16)(u16SubMsgLen + 16),psTlvRespMsg);
                FREE(psEncryptedData);

            }break;
            case E_PAIR_VERIFY_M3_FINISHED_REQUEST:{
                NOT_vPrintln(DBG_PAIR, "E_PAIR_VERIFY_M3_FINISHED_REQUEST\n");
                //eM4VerifyFinishResponse(iSocketFd, psIpMsg);
                uint8 *psEncryptedPackData = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
                uint16 u16EncryptedPackLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
                uint16 u16EncryptedLen = u16EncryptedPackLen - (uint16)LEN_AUTH_TAG;

                chacha20_ctx chacha20;
                memset(&chacha20, 0, sizeof(chacha20));
                chacha20_setup(&chacha20, sPairVerify.auEnKey, 32, (uint8*)"PV-Msg03");
                uint8 auIn[64] = {0}, auOut[64] = {0};
                chacha20_encrypt(&chacha20, auIn, auOut, 64);

                char verify[16] = {0};
                ePoly1305_GenKey(auOut, psEncryptedPackData, u16EncryptedLen, T_FALSE, verify);
                if (!bcmp(verify, &psEncryptedPackData[u16EncryptedLen], LEN_AUTH_TAG)) {
                    uint8 *psEncryptedData = (uint8*)malloc(u16EncryptedLen);
                    chacha20_decrypt(&chacha20, psEncryptedPackData, psEncryptedData, u16EncryptedLen);
                    tsTlvPackage *psSubTlvPack = psTlvPackageFormat(psEncryptedData, u16EncryptedLen);
                    FREE(psEncryptedData);

                    uint8 *controllerID = psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_IDENTIFIER);
                    uint8 key[36] = {0};
                    uint8 recpublicKey[32] = {0};
                    eIOSDevicePairingIDRead(key, 36);
                    if(bcmp(key, controllerID, 36) == 0){
                        eIOSDeviceLTPKRead(recpublicKey, 32);
                    } else{
                        ERR_vPrintln(T_TRUE, "controllerID verify failed");
                        return E_PAIRING_STATUS_ERROR;
                    }

                    uint8 auIOSDeviceInfo[100];
                    memcpy(auIOSDeviceInfo, sPairVerify.auControllerPublicKey, 32);
                    memcpy(&auIOSDeviceInfo[32], controllerID, 36);
                    memcpy(&auIOSDeviceInfo[68], sPairVerify.auPublicKey, 32);

                    int err = ed25519_sign_open(auIOSDeviceInfo, 100, recpublicKey, psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_SIGNATURE));
                    eTlvPackageRelease(psSubTlvPack);
                    if (err == 0) {
                        end = T_TRUE;
                        hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32, (uint8*)"Control-Read-Encryption-Key", 27, sController.auAccessoryToControllerKey, 32);
                        hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32, (uint8*)"Control-Write-Encryption-Key", 28, sController.auControllerToAccessoryKey, 32);
                        INF_vPrintln(DBG_PAIR, "PairVerify success\n");
                    } else {
                        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
                        ERR_vPrintln(DBG_PAIR, "Verify failed\n");
                        goto AuthenticationError;
                    }
                } else{
                    ERR_vPrintln(T_TRUE, "Error verify");
                    return E_PAIRING_STATUS_ERROR;
                }

            }break;
        }
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMsg);
        psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMsg,&psRepBuffer,&u16RepLen);
        psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
        eHttpResponse(sController.iSockFd, &psIpMsg->sHttp, psRepBuffer, u16RepLen);
        eIpMessageRelease(psResponse);
        eIpMessageRelease(psIpMsg);
        FREE(psRepBuffer);
    }while (!end && (0 < (sController.iLen = (int)read(sController.iSockFd, sController.auBuffer, MABF))));
    if(end)
        return E_PAIRING_STATUS_OK;
    else
        return E_PAIRING_STATUS_ERROR;
AuthenticationError:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psResponse->psTlvPackage->sMessage);
    psResponse->psTlvPackage->eTlvMessageGetData(&psResponse->psTlvPackage->sMessage,&psRepBuffer,&u16RepLen);
    psIpMsg->sHttp.iHttpStatus = E_HTTP_STATUS_SUCCESS_OK;
    eHttpResponse(sController.iSockFd, &psIpMsg->sHttp, psRepBuffer, u16RepLen);
    eIpMessageRelease(psResponse);
    eIpMessageRelease(psIpMsg);
    FREE(psRepBuffer);

    return E_PAIRING_STATUS_ERROR;
}
