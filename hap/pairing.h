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
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TLV_TYPE_LEN 1
#define TLV_LEN_LEN 1
#define TLV_HEADER (TLV_TYPE_LEN + TLV_LEN_LEN)
#define TLV_FRAGMENTED 255

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
} tePairSetupState;

typedef enum {
    E_TLV_VALUE_TYPE_METHOD         = 0x00,
    E_TLV_VALUE_TYPE_IDENTIFIER     = 0x01,
    E_TLV_VALUE_TYPE_SALT           = 0x02,
    E_TLV_VALUE_TYPE_PUBLIC_KEY     = 0x03,
    E_TLV_VALUE_TYPE_PROOF          = 0x04,
    E_TLV_VALUE_TYPE_ENCRYPTED_DATA = 0x05,
    E_TLV_VALUE_TYPE_STATE          = 0x06,
    E_TLV_VALUE_TYPE_ERROR          = 0x07,
    E_TLV_VALUE_TYPE_RETRY_DELAY    = 0x08,
    E_TLV_VALUE_TYPE_CERTIFICATE    = 0x09,
    E_TLV_VALUE_TYPE_SIGNATURE      = 0x0A,
    E_TLV_VALUE_TYPE_PERMISSIONS    = 0x0B,
    E_TLV_VALUE_TYPE_FRAGMENT_DATA  = 0x0C,
    E_TLV_VALUE_TYPE_FRAGMENT_LAST  = 0x0D,
    E_TLV_VALUE_TYPE_SEPARATOR      = 0x0F,
} teTlvValue;

typedef enum {
    E_PAIR_VERIFY_START_REQUEST,
    E_PAIR_VERIFY_START_RESPONSE,
    E_PAIR_VERIFY_FINISHED_REQUEST,
    E_PAIR_VERIFY_FINISHED_RESPONSE,
} tePairVerify;

typedef enum {
    E_TLV_ERROR_UNKNOW          = 0x01,
    E_TLV_ERROR_AUTHENTICATION  = 0x02,
    E_TLV_ERROR_BACKOFF         = 0x03,
    E_TLV_ERROR_MAXPEERS        = 0x04,
    E_TLV_ERROR_MAXTRIES        = 0x05,
    E_TLV_ERROR_UNAVAILABLE     = 0x06,
    E_TLV_ERROR_BUSY            = 0x07,
} teTlvError;

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
extern tePairSetupState ePair;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tePairStatus ePairSetup(int iSockFd, tsBonjour *psBonjour, char *pBuf, uint16 u16Len);
tePairStatus ePairSetup2(int iSockFd, char *pSetupCode, char *pBuf, uint16 u16Len);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PAIRING_H
