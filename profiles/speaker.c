/****************************************************************************
 *
 * MODULE:             speaker.c
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
#include <accessory.h>
#include "speaker.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_SPEAKER (1)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

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
/***        Local    Functions                                            ***/
/****************************************************************************/
static teHapStatus eAccessorySpeakerInit(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_SPEAKER, "eAccessorySpeakerInit");
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, E_SERVICE_SPEAKER, UUID_SERVICE_CHARACTER, &psService);

    tsCharacteristic sCharaTemp;

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_MUTE;
    sCharaTemp.eFormat = E_TYPE_BOOL;
    sCharaTemp.uValue.bData = T_FALSE;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_VOLUME;
    sCharaTemp.eFormat = E_TYPE_UINT8;
    sCharaTemp.uValue.iData = 50;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.iData = 1;
    sCharaTemp.eUnit = E_UNIT_PERCENTAGE;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.iData = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.iData = 100;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    return E_HAP_STATUS_OK;
}

static teHapStatus eSpeakerHandle(tsCharacteristic *psCharacter, json_object *psJson)
{
    if(psCharacter->eType == E_CHARACTERISTIC_MUTE){
        NOT_vPrintln(DBG_SPEAKER, "Turn On/Off Audio:%d", json_object_get_int(psJson));
    } else if(psCharacter->eType == E_CHARACTERISTIC_VOLUME){
        NOT_vPrintln(DBG_SPEAKER, "Set Volume Level:%d", json_object_get_int(psJson));
    }
    return E_HAP_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tsProfile *psSpeakerProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                             E_HAP_TYPE_LIGHT_BULB, eAccessorySpeakerInit, eSpeakerHandle);
    return psProfile;
}

teHapStatus eSpeakerProfileRelease(tsProfile *psProfile)
{
    eProfileRelease(psProfile);
    return E_HAP_STATUS_OK;
}
