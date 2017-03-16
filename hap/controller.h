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
#include "list.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    tsCharacteristic sCharacter;
    struct dl_list list;
} tsEventNotify;

typedef struct {
    pthread_mutex_t mutex;
    int iSocketFd;
    uint64 u64NumberReceive;
    uint64 u64NumberSend;
    uint8  auControllerToAccessoryKey[32];
    uint8  auAccessoryToControllerKey[32];
    tsEventNotify sEventNotify;
    struct dl_list list;
} tsController;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern tsController sControllerHead;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus  eControllerInit();
tsController *psControllerListAdd(tsController *psControllerList);
teHapStatus  eControllerListDel(tsController *psController);
teHapStatus eControllerFinished();
teHapStatus eControllerListAddNotify(tsController *psController, tsCharacteristic *psCharacter);
bool_t bIsControllerNotify(tsController *psController, tsCharacteristic *psCharacter);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_CONTROLLER_H
