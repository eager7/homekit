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
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_PAIRING_STATUS_OK = 0x00,
    E_PAIRING_STATUS_ERROR,
} tePairStatus;

typedef enum {
    E_PAIR_SETUP_SRP_START_REQUEST,
    E_PAIR_SETUP_SRP_START_RESPONSE,
    E_PAIR_SETUP_SRP_VERIFY_REQUEST,
    E_PAIR_SETUP_SRP_VERIFY_RESPONSE,
    E_PAIR_SETUP_EXCHANGE_REQUEST,
    E_PAIR_SETUP_EXCHANGE_RESPONSE,
} tePairSetup;

typedef enum {
    E_PAIR_VERIFY_START_REQUEST,
    E_PAIR_VERIFY_START_RESPONSE,
    E_PAIR_VERIFY_FINISHED_REQUEST,
    E_PAIR_VERIFY_FINISHED_RESPONSE,
} tePairVerify;
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
tePairStatus ePairSetup(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PAIRING_H
