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
#ifndef HOMEKIT_PAIRING_H
#define HOMEKIT_PAIRING_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include <srp.h>
#include "http_handle.h"
#include "bonjour.h"
#include "tlv.h"
#include "curve25519-donna.h"
#include "ed25519.h"
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
typedef enum {
    E_PAIRING_STATUS_OK = 0x00,
    E_PAIRING_STATUS_ERROR,
    E_PAIRING_STATUS_ERROR_KEY,
    E_PAIRING_STATUS_ERROR_SOCKET,
    E_PAIRING_STATUS_NOT_FOUND,
} tePairStatus;

typedef enum {
    E_PAIR_SETUP_SRP_START_NULL         = 0x00,
    E_PAIR_SETUP_M1_SRP_START_REQUEST      = 0x01,
    E_PAIR_SETUP_M2_SRP_START_RESPONSE     = 0x02,
    E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST     = 0x03,
    E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE    = 0x04,
    E_PAIR_SETUP_M5_EXCHANGE_REQUEST       = 0x05,
    E_PAIR_SETUP_M6_EXCHANGE_RESPONSE      = 0x06,
} tePairSetup;

typedef enum {
    E_PAIR_VERIFY_M1_START_REQUEST  = 0x01,
    E_PAIR_VERIFY_M2_START_RESPONSE = 0x02,
    E_PAIR_VERIFY_M3_FINISHED_REQUEST = 0x03,
    E_PAIR_VERIFY_M4_FINISHED_RESPONSE = 0x04,
} tePairVerify;

typedef struct {
    tePairSetup eSetup;
    SRP         *pSrp;
    cstr        *pSecretKey;
    uint8       auSessionKey[64];
    uint8       auControllerIdentifier[36];
    uint8       auControllerPublicKey[32];
    pthread_mutex_t mutex;
} tsPairSetup;

typedef struct {
    curved25519_key auPublicKey;
    curved25519_key auControllerPublicKey;
    curved25519_key auSharedKey;
    uint8_t auEnKey[32];
} tsPairVerify;

typedef struct {
    uint8   u8Type;
    uint16  u16Len;
    uint16  u16Offset;
    uint8   *psValue;
} tsTlvType;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern tePairSetup ePair;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tePairStatus ePairSetup(int iSockFd, tsBonjour *psBonjour, char *pBuf, uint16 u16Len);
tePairStatus ePairSetup2(int iSockFd, char *pSetupCode, char *pBuf, uint16 u16Len);
tePairStatus ePairVerify(int iSockFd, tsBonjour *psBonjour, char *pBuf, uint16 u16Len);

tePairStatus eDecryptedHttpMessage(char *psBuffer, int iLen);
void Poly1305_GenKey(const unsigned char * key, uint8_t * buf, uint16_t len, bool_t bWithLen, char* verify);

tePairStatus eIOSDevicePairingIDSave(uint8 *buf, int len);
tePairStatus eIOSDevicePairingIDRead(uint8 *buf, int len);
tePairStatus eIOSDeviceLTPKSave(uint8 *buf, int len);
tePairStatus eIOSDeviceLTPKRead(uint8 *buf, int len);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PAIRING_H
