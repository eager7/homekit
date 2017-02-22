/****************************************************************************
 *
 * MODULE:             controller.h
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
#ifndef HOMEKIT_CONTROLLER_H
#define HOMEKIT_CONTROLLER_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "pairing.h"
#include "http_handle.h"
#include "bonjour.h"
#include "tlv.h"
#include "list.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#if 0
typedef struct {
    int     iSockFd;
    uint8   auBuffer[MABF];
    uint64  u64NumMsgRec;
    uint64  u64NumMsgSend;
    uint8   auControllerToAccessoryKey[32];
    uint8   auAccessoryToControllerKey[32];

    tePairSetup  ePairSetup;
    tePairVerify ePairVerify;
    struct dl_list list;
} tsController;
#endif
typedef struct {
    int     iSockFd;
    int     iLen;
    uint8   auBuffer[MABF];
    uint64  u64NumMsgRec;
    uint64  u64NumMsgSend;
    uint8   auControllerToAccessoryKey[32];
    uint8   auAccessoryToControllerKey[32];

    tePairSetup  ePairSetup;
    tePairVerify ePairVerify;
    struct dl_list list;
} tsController;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern tsController sController;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus eControllerInit();
tePairStatus eHandlePairSetup();
tePairStatus eHandlePairVerify();
teHapStatus eHandleAccessoryRequest();
teHapStatus eControllerFinished();
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_CONTROLLER_H
