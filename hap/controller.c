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
#include "controller.h"
#include "chacha20_simple.h"
#include "poly1305.h"
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

void vBroadcastMessage(tsController *psController, uint8 *psBuffer, size_t sLength)
{
    chacha20_ctx ctx;    bzero(&ctx, sizeof(ctx));
    char temp[64];  bzero(temp, 64); char temp2[64];  bzero(temp2, 64);
    uint8 *reply = malloc(sLength+18);
    reply[0] = (uint8)(sLength%256);
    reply[1] = (uint8)((sLength-(uint8_t)reply[0])/256);
    chacha20_setup(&ctx, (const uint8_t *)psController->auAccessoryToControllerKey, 32, (uint8_t *)&psController->u64NumberSend);
    psController->u64NumberSend++;
    chacha20_encrypt(&ctx, (const uint8_t*)temp, (uint8_t *)temp2, 64);
    chacha20_encrypt(&ctx, (const uint8_t*)psBuffer, &reply[2], sLength);

    poly1305_context verifyContext; bzero(&verifyContext, sizeof(verifyContext));
    poly1305_init(&verifyContext, (const unsigned char*)temp2);
    {
        char waste[16];
        bzero(waste, 16);
        poly1305_update(&verifyContext, (const unsigned char *)reply, 2);
        poly1305_update(&verifyContext, (const unsigned char *)waste, 14);
        poly1305_update(&verifyContext, (const unsigned char *)&reply[2], sLength);
        poly1305_update(&verifyContext, (const unsigned char *)waste, 16-sLength%16);
        unsigned long long _len;
        _len = 2;
        poly1305_update(&verifyContext, (const unsigned char *)&_len, 8);
        _len = sLength;
        poly1305_update(&verifyContext, (const unsigned char *)&_len, 8);
    }
    poly1305_finish(&verifyContext, &reply[sLength+2]);
    if(-1 == send(psController->iSocketFd, reply, sLength+18, 0)){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    FREE(reply);
}