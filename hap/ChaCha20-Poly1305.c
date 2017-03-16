/****************************************************************************
 *
 * MODULE:             ChaCha20-Poly1305.c
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
#include "ChaCha20-Poly1305.h"
#include "chacha20_simple.h"
#include "poly1305.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_CHA_POLY (1)
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

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus eDecryptedMessageNoLen(const uint8 *psBuffer, uint16 u16LenIn, const uint8 *psKey, const uint8* psNonce, uint8 *psDecryptedData)
{
    chacha20_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));
    chacha20_setup(&ctx, psKey, 32, (uint8*)psNonce);

    uint8 auKeyIn[64] = {0}, auKeyOut[64] = {0}, auVerify[16] = {0};
    chacha20_encrypt(&ctx, auKeyIn, auKeyOut, 64);

    ePoly1305_GenKey(auKeyOut, psBuffer, u16LenIn, T_FALSE, auVerify);
    if(memcmp(auVerify, &psBuffer[u16LenIn], LEN_AUTH_TAG)){
        ERR_vPrintln(T_TRUE, "ChaCha20-Poly1305 Verify Failed");
        return E_HAP_STATUS_VERIFY_ERROR;
    }
    chacha20_decrypt(&ctx, psBuffer, psDecryptedData, (size_t)u16LenIn);
    return E_HAP_STATUS_OK;
}
teHapStatus eEncryptedMessageNoLen(const uint8 *psBuffer, uint16 u16LenIn, const uint8 *psKey, const uint8* psNonce, uint8 *psEncryptedData, uint16 *pu16LenOut)
{
    chacha20_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));
    chacha20_setup(&ctx, psKey, 32, (uint8*)psNonce);

    uint8 auKeyIn[64] = {0}, auKeyOut[64] = {0}, auVerify[16] = {0};
    chacha20_encrypt(&ctx, auKeyIn, auKeyOut, 64);
    chacha20_encrypt(&ctx, psBuffer, psEncryptedData, u16LenIn);

    ePoly1305_GenKey(auKeyOut, psEncryptedData, u16LenIn, T_FALSE, auVerify);
    memcpy(&psEncryptedData[u16LenIn], auVerify, 16);
    if(pu16LenOut) *pu16LenOut = u16LenIn + (uint16)LEN_AUTH_TAG;
    return E_HAP_STATUS_OK;
}
teHapStatus eDecryptedMessageWithLen(const uint8 *psBuffer, uint16 u16LenIn, tsController *psController, uint8 *psDecryptedData, uint16 *pu16LenOut)
{
    uint16 u16MsgLen = (psBuffer[0] | ((uint16)psBuffer[1] << 8));
    *pu16LenOut = u16MsgLen;
    chacha20_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));
    chacha20_setup(&ctx, psController->auControllerToAccessoryKey, 32, (uint8*)&psController->u64NumberReceive);

    uint8 auKeyIn[64] = {0}, auKeyOut[64] = {0}, auVerify[16] = {0};
    chacha20_encrypt(&ctx, auKeyIn, auKeyOut, 64);
    ePoly1305_GenKey(auKeyOut, psBuffer, u16MsgLen, T_TRUE, auVerify);
    if(u16LenIn >= (2 + u16MsgLen + 16) && memcmp(auVerify, &psBuffer[2 + u16MsgLen], 16) == 0) {
        chacha20_decrypt(&ctx, &psBuffer[2], psDecryptedData, u16MsgLen);
        NOT_vPrintln(DBG_CHA_POLY, "Verify Successfully!\n");
        return E_HAP_STATUS_OK;
    }
    ERR_vPrintln(T_TRUE, "ChaCha20-Poly1305 Verify Failed");
    return E_HAP_STATUS_VERIFY_ERROR;
}
teHapStatus eEncryptedMessageWithLen(const uint8 *psBuffer, uint16 u16LenIn, tsController *psController, uint8 *psDecryptedData, uint16 *pu16LenOut)
{
    psDecryptedData[0] = (uint8)(u16LenIn % 256);
    psDecryptedData[1] = (uint8)((u16LenIn - psDecryptedData[0]) / 256);

    chacha20_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));
    DBG_vPrintln(DBG_CHA_POLY, "Encrypted Send Num:%llu\n", psController->u64NumberSend);
    PrintArray(DBG_CHA_POLY, psController->auAccessoryToControllerKey, 32);
    chacha20_setup(&ctx, psController->auAccessoryToControllerKey, 32, (uint8*)&psController->u64NumberSend);
    uint8 auKeyIn[64] = {0}, auKeyOut[64] = {0}, auVerify[16] = {0};
    chacha20_encrypt(&ctx, auKeyIn, auKeyOut, 64);
    chacha20_encrypt(&ctx, psBuffer, &psDecryptedData[2], u16LenIn);
    ePoly1305_GenKey(auKeyOut, psDecryptedData, u16LenIn, T_TRUE, auVerify);
    memcpy(&psDecryptedData[u16LenIn+2], auVerify, 16);
    *pu16LenOut = u16LenIn + (uint16)18;
    return E_HAP_STATUS_OK;
}