/****************************************************************************
 *
 * MODULE:             profile.h
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

#ifndef HOMEKIT_PROFILE_H
#define HOMEKIT_PROFILE_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "accessory.h"
#include "list.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct {
    pthread_mutex_t mutex;
    int iSocketFd;
    uint64 u64NumberReceive;
    uint64 u64NumberSend;
    uint8  auControllerToAccessoryKey[32];
    uint8  auAccessoryToControllerKey[32];
    struct dl_list list;
} tsController;

typedef struct {
    tsCharacteristic sCharacter;
    tsController sController;
} tsQueueData;

typedef teHapStatus (*fpeInitCategory)(tsAccessory *psAccessory);
typedef teHapStatus (*feHandleSetCmd)(tsCharacteristic *psCharacter, json_object *psJson, tsController *psSocket);
typedef teHapStatus (*feHandleGetCmd)(tsCharacteristic *psCharacter);
typedef teHapStatus (*fpeSetCharacteristicInfo)(tsAccessory *psAccessory, tsController *psSocket, const uint8 *psCmd, uint8 **ppsBuffer, uint16 *pu16Len, feHandleSetCmd fCallback);
typedef json_object* (*fpsGetAccessoryInfo)(const tsAccessory *psAccessory);
typedef json_object* (*fpsGetCharacteristicInfo)(const tsAccessory *psAccessory, const char *psCmd, feHandleGetCmd fCallback);

typedef struct {
    tsAccessory                 *psAccessory;
    fpeInitCategory             peInitCategory;
    fpsGetAccessoryInfo         psGetAccessoryInfo;
    fpeSetCharacteristicInfo    peSetCharacteristicInfo;
    fpsGetCharacteristicInfo    psGetCharacteristicInfo;
    feHandleSetCmd              eHandleSetCmd;
    feHandleGetCmd              eHandleGetCmd;
} tsProfile;
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
** Prototype    : psProfileGenerate
** Description  : malloc a new profile object
** Input        :
** Output       : none
** Return Value : if success, return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsProfile *psProfileGenerate(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel,
                             teAccessoryCategories eType, fpeInitCategory fsInitCategory, feHandleSetCmd eHandleSetCmd,
                             feHandleGetCmd eHandleGetCmd);
teHapStatus eProfileRelease(tsProfile *psProfile);
teHapStatus eNotifyEnQueue(tsCharacteristic *psCharacter, tsController *psController);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PROFILE_H
