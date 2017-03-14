/****************************************************************************
 *
 * MODULE:             light_bulb.c
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
#include "light_bulb.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_LIGHT (1)
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
static teHapStatus eAccessoryLightBulbInit(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_LIGHT, "eAccessoryLightBulbInit");
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, E_SERVICE_LIGHT_BULB, UUID_SERVICE_CHARACTER, &psService);

    tsCharacteristic sCharaTemp;

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_ON;
    sCharaTemp.eFormat = E_TYPE_BOOL;
    sCharaTemp.uValue.bData = T_TRUE;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_BRIGHTNESS;
    sCharaTemp.eFormat = E_TYPE_INT;
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

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_HUE;
    sCharaTemp.eFormat = E_TYPE_FLOAT;
    sCharaTemp.uValue.fData = 360;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.fData = 1;
    sCharaTemp.eUnit = E_UNIT_ARC_DEGREES;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.fData = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.fData = 360;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_SATURATION;
    sCharaTemp.eFormat = E_TYPE_FLOAT;
    sCharaTemp.uValue.fData = 100;
    sCharaTemp.eUnit = E_UNIT_PERCENTAGE;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.fData = 1;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.fData = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.fData = 100;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    return E_HAP_STATUS_OK;
}

static teHapStatus eLightBulbHandle(tsCharacteristic *psCharacter, json_object *psJson)
{
    if(psCharacter->eType == E_CHARACTERISTIC_ON){
        NOT_vPrintln(DBG_LIGHT, "Turn On/Off Bulb Light:%d", json_object_get_int(psJson));
    } else if(psCharacter->eType == E_CHARACTERISTIC_BRIGHTNESS){
        NOT_vPrintln(DBG_LIGHT, "Set Bulb Light Level:%d", json_object_get_int(psJson));
    }
    return E_HAP_STATUS_OK;
}
static teHapStatus eLightBulbGetInfo(tsCharacteristic *psCharacter)
{
    if(psCharacter->eType == E_CHARACTERISTIC_ON){
        NOT_vPrintln(DBG_LIGHT, "Get Bulb Light On/Off");
    } else if(psCharacter->eType == E_CHARACTERISTIC_BRIGHTNESS){
        NOT_vPrintln(DBG_LIGHT, "Get Bulb Light Level");
    }
    return E_HAP_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tsProfile *psLightBulbProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                             E_HAP_TYPE_LIGHT_BULB, eAccessoryLightBulbInit, eLightBulbHandle, eLightBulbGetInfo);
    return psProfile;
}

teHapStatus eLightBulbProfileRelease(tsProfile *psProfile)
{
    eProfileRelease(psProfile);
    return E_HAP_STATUS_OK;
}
