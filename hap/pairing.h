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
#include "controller.h"
#include "ChaCha20-Poly1305.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define MAX_TRIES_PAIR 100
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_PARIING_REMOVE_M1_REMOVE_PAIRING_REQUEST  = 0x01,
    E_PARIING_REMOVE_M2_REMOVE_PAIRING_RESPONSE = 0x02,
} tePairingRemove;

typedef enum {
    E_PAIR_SETUP_SRP_START_NULL             = 0x00,
    E_PAIR_SETUP_M1_SRP_START_REQUEST       = 0x01,
    E_PAIR_SETUP_M2_SRP_START_RESPONSE      = 0x02,
    E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST      = 0x03,
    E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE     = 0x04,
    E_PAIR_SETUP_M5_EXCHANGE_REQUEST        = 0x05,
    E_PAIR_SETUP_M6_EXCHANGE_RESPONSE       = 0x06,
} tePairSetup;

typedef enum {
    E_PAIR_VERIFY_M1_START_REQUEST          = 0x01,
    E_PAIR_VERIFY_M2_START_RESPONSE         = 0x02,
    E_PAIR_VERIFY_M3_FINISHED_REQUEST       = 0x03,
    E_PAIR_VERIFY_M4_FINISHED_RESPONSE      = 0x04,
} tePairVerify;

typedef struct {
    bool_t      bPaired;
    tePairSetup eState;
    SRP         *pSrp;
    cstr        *pSecretKey;
    uint8       auSessionKey[64];
    uint8       auControllerIdentifier[36];
    uint8       auControllerPublicKey[32];
    uint8       u8MaxTries;
    pthread_mutex_t mutex;
} tsPairSetup;

typedef struct {
    tePairVerify    eState;
    curved25519_key auPublicKey;
    curved25519_key auControllerPublicKey;
    curved25519_key auSharedKey;
    uint8_t         auSessionKey[32];
} tsPairVerify;

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
/*****************************************************************************
** Prototype    : ePairingInit
** Description  : Initialize pair's structer, create a mutex, initialize the
 * status of paired
** Input        : none
** Output       : none
** Return Value : none

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus ePairingInit();
teHapStatus ePairingFinished();
/*****************************************************************************
** Prototype    : eHandlePairSetup
** Description  : Handle the pair setup process, in this function, will read
 * socket and store a secret key
** Input        : psBuffer, the first socket buffer read by bonjour program
 *                iLen, the first socket buffer len read by bonjour program
 *                iSocketFd, the socket fd
 *                psBonjour, the bonjour structure
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR,
 * if socket disconnected, return E_HAP_STATUS_SOCKET_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eHandlePairSetup(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour);
/*****************************************************************************
** Prototype    : eHandlePairVerify
** Description  : Handle the pair verify process, in this function, will read
 * socket and store a session key
** Input        : psBuffer, the first socket buffer read by bonjour program
 *                iLen, the first socket buffer len read by bonjour program
 *                psSocketFd, the socket structure
 *                psBonjour, the bonjour structure
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR,
 * if socket disconnected, return E_HAP_STATUS_SOCKET_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eHandlePairVerify(uint8 *psBuffer, int iLen, tsController *psSocketFd, tsBonjour *psBonjour);
teHapStatus eHandlePairingRemove(const uint8 *psBuffer, uint16 u16Len, uint8 **ppResp, uint16 *pu16Len);

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PAIRING_H
