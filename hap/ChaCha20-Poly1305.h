/****************************************************************************
 *
 * MODULE:             ChaCha20-Poly1305.h
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
#ifndef HOMEKIT_CHACHA20_POLY1305_H
#define HOMEKIT_CHACHA20_POLY1305_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "accessory_type.h"
#include "controller.h"
#include "profile.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define LEN_SALT 16
#define LEN_AUTH_TAG 16
#define LEN_CHA20_KEY 32
#define LEN_HKDF_LEN 32
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
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus ePoly1305_GenKey(const uint8 *key, const uint8 *buf, uint16 len, bool_t bWithLen, uint8 *verify);
teHapStatus eEncryptedMessageWithLen(const uint8 *psBuffer, uint16 u16LenIn, tsController *psController, uint8 *psDecryptedData, uint16 *pu16LenOut);
teHapStatus eDecryptedMessageWithLen(const uint8 *psBuffer, uint16 u16LenIn, tsController *psController, uint8 *psDecryptedData, uint16 *pu16LenOut);
teHapStatus eEncryptedMessageNoLen(const uint8 *psBuffer, uint16 u16LenIn, const uint8 *psKey, const uint8* psNonce, uint8 *psEncryptedData, uint16 *pu16LenOut);
teHapStatus eDecryptedMessageNoLen(const uint8 *psBuffer, uint16 u16LenIn, const uint8 *psKey, const uint8* psNonce, uint8 *psDecryptedData);
teHapStatus eHandleAccessoryPackage(tsProfile *psProfile, const uint8 *psData, uint16 u16Len, uint8 **ppsResp, uint16 *pu16Len, tsController *psController);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_CHACHA20_POLY1305_H
