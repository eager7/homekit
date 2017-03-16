/****************************************************************************
 *
 * MODULE:             controller.c
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

#include <profile.h>
#include <accessory.h>
#include "controller.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_CONTROLLER (1)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsController sControllerHead;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus eControllerInit()
{
    memset(&sControllerHead, 0, sizeof(tsController));
    dl_list_init(&sControllerHead.list);
    dl_list_init(&sControllerHead.sEventNotify.list);
    return E_HAP_STATUS_OK;
}

teHapStatus eControllerFinished()
{
    tsController *psController = NULL, *psControllerTemp = NULL;
    dl_list_for_each_safe(psController, psControllerTemp, &psController->list, tsController, list){
        eControllerListDel(psController);
    }
    return E_HAP_STATUS_OK;
}

tsController *psControllerListAdd(tsController *psControllerList)
{
    tsController *psController = (tsController*)calloc(1, sizeof(tsController));
    psController->iSocketFd = -1;
    eLockCreate(&psController->mutex);
    dl_list_init(&psController->sEventNotify.list);
    dl_list_add_tail(&psControllerList->list, &psController->list);
    return psController;
}

teHapStatus eControllerListDel(tsController *psController)
{
    dl_list_del(&psController->list);
    tsEventNotify *psEvent = NULL, *psEventTemp = NULL;
    dl_list_for_each_safe(psEvent, psEventTemp, &psController->sEventNotify.list, tsEventNotify, list){
        dl_list_del(&psEvent->list);
        FREE(psEvent);
    }
    eLockDestroy(&psController->mutex);
    FREE(psController);
    return E_HAP_STATUS_OK;
}

teHapStatus eControllerListAddNotify(tsController *psController, tsCharacteristic *psCharacter)
{
    tsEventNotify *psEvent = (tsEventNotify*)calloc(1, sizeof(tsEventNotify));
    memcpy(&psEvent->sCharacter, psCharacter, sizeof(tsCharacteristic));
    dl_list_add_tail(&psController->sEventNotify.list, &psEvent->list);
    return E_HAP_STATUS_OK;
}

bool_t bIsControllerNotify(tsController *psController, tsCharacteristic *psCharacter)
{
    tsEventNotify *psEvent = NULL;
    dl_list_for_each(psEvent, &psController->sEventNotify.list, tsEventNotify, list){
        if((psEvent->sCharacter.u64IID == psCharacter->u64IID) && (psEvent->sCharacter.bEventNot == T_TRUE)){
            return T_TRUE;
        }
    }
    return T_FALSE;
}