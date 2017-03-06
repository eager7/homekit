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

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_PROFILE_CMD_LIGHT_BULB_ON,
    E_PROFILE_CMD_LIGHT_BULB_OFF,
} teProfileCmd;

typedef teHapStatus (*fpeHandleFunc)(tsAccessory *psAccessory);
typedef teHapStatus (*fpeInitCategory)(tsAccessory *psAccessory);
typedef teHapStatus (*fpeSetCharacteristicInfo)(tsAccessory *psAccessory, const uint8 *psCmd, uint8 **ppsBuffer, uint16 *pu16Len);
typedef json_object* (*fpsGetAccessoryInfo)(const tsAccessory *psAccessory);
typedef json_object* (*fpsGetCharacteristicInfo)(const tsAccessory *psAccessory, const char *psCmd);

typedef struct {
    teProfileCmd eProfileCmd;
    fpeHandleFunc peHandleFunc;
} tsProfileHandle;

typedef struct {
    tsAccessory                 *psAccessory;
    fpeInitCategory             peInitCategory;
    fpsGetAccessoryInfo         psGetAccessoryInfo;
    fpeSetCharacteristicInfo    peSetCharacteristicInfo;
    fpsGetCharacteristicInfo    psGetCharacteristicInfo;
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
tsProfile *psProfileGenerate(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer,
                             char *psModel, teAccessoryCategories eType, fpeInitCategory fsInitCategory,
                             fpeSetCharacteristicInfo fsCallBack);
teHapStatus eProfileRelease(tsProfile *psProfile);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PROFILE_H
