/****************************************************************************
 *
 * MODULE:             window_covering.c
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
#include "window_covering.h"
#include "http_handle.h"
#include "pairing.h"
#include "chacha20_simple.h"
#include "poly1305.h"
#include "profile.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_WINDOW_COVER (1)
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
static teHapStatus eAccessoryWindowCoveringInit(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_WINDOW_COVER, "eAccessoryWindowCoveringInit");
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, E_SERVICE_WINDOW_COVERING, UUID_SERVICE_CHARACTER, &psService);

    tsCharacteristic sCharaTemp;

    /* Required Characteristics */
    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_TARGET_POSITION;
    sCharaTemp.eFormat = E_TYPE_UINT8;
    sCharaTemp.uValue.u8Data = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.u8Data = 1;
    sCharaTemp.eUnit = E_UNIT_PERCENTAGE;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.u8Data = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.u8Data = 100;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_CURRENT_POSITION;
    sCharaTemp.eFormat = E_TYPE_UINT8;
    sCharaTemp.uValue.u8Data = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.u8Data = 1;
    sCharaTemp.eUnit = E_UNIT_PERCENTAGE;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.u8Data = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.u8Data = 100;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_POSITION_STATE;
    sCharaTemp.eFormat = E_TYPE_UINT8;
    sCharaTemp.uValue.u8Data = E_POSITION_STATE_STOP;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.u8Data = 1;
    //sCharaTemp.eUnit = E_UNIT_PERCENTAGE;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.u8Data = 0;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.u8Data = 2;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    /* Optional Characteristics */
#if 0 //This Character will lead to pairing failed
    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_HOLD_POSITION;
    sCharaTemp.eFormat = E_TYPE_BOOL;
    sCharaTemp.uValue.bData = T_FALSE;
    sCharaTemp.u8Perms = E_PERM_PAIRED_WRITE;
    eServiceAddCharacter(psService, sCharaTemp, NULL);
#endif

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_CURRENT_HORIZONTAL_TILT_ANGLE;
    sCharaTemp.eFormat = E_TYPE_INT;
    sCharaTemp.uValue.iData = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.iData = 1;
    sCharaTemp.eUnit = E_UNIT_ARC_DEGREES;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.iData = -90;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.iData = 90;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_TARGET_HORIZONTAL_TILT_ANGLE;
    sCharaTemp.eFormat = E_TYPE_INT;
    sCharaTemp.uValue.iData = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.iData = 1;
    sCharaTemp.eUnit = E_UNIT_ARC_DEGREES;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.iData = -90;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.iData = 90;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT | E_PERM_PAIRED_WRITE;
    eServiceAddCharacter(psService, sCharaTemp, NULL);
#if 0
    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_CURRENT_VERTICAL_TILT_ANGLE;
    sCharaTemp.eFormat = E_TYPE_INT;
    sCharaTemp.uValue.iData = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.iData = 1;
    sCharaTemp.eUnit = E_UNIT_ARC_DEGREES;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.iData = -90;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.iData = 90;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_TARGET_VERTICAL_TILT_ANGLE;
    sCharaTemp.eFormat = E_TYPE_INT;
    sCharaTemp.uValue.iData = 0;
    sCharaTemp.sSetupValue.bEnable = T_TRUE;
    sCharaTemp.sSetupValue.uData.iData = 1;
    sCharaTemp.eUnit = E_UNIT_ARC_DEGREES;
    sCharaTemp.sMinimumValue.bEnable = T_TRUE;
    sCharaTemp.sMinimumValue.uData.iData = -90;
    sCharaTemp.sMaximumValue.bEnable = T_TRUE;
    sCharaTemp.sMaximumValue.uData.iData = 90;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT | E_PERM_PAIRED_WRITE;//
    eServiceAddCharacter(psService, sCharaTemp, NULL);
#endif
    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_OBSTRUCTION_DETECTED;
    sCharaTemp.eFormat = E_TYPE_BOOL;
    sCharaTemp.uValue.bData = T_FALSE;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    return E_HAP_STATUS_OK;
}
static teHapStatus eIdentify()
{
    DBG_vPrintln(DBG_WINDOW_COVER, "Identify");
    sleep(1);
    DBG_vPrintln(DBG_WINDOW_COVER, "Identify");
    sleep(1);
    DBG_vPrintln(DBG_WINDOW_COVER, "Identify");
    sleep(1);
    return E_HAP_STATUS_OK;
}
static teHapStatus eHandleSetCmd(tsCharacteristic *psCharacter, json_object *psJson)
{
    if(psCharacter->eType == E_CHARACTERISTIC_TARGET_POSITION){
        NOT_vPrintln(DBG_WINDOW_COVER, "Set Window Position:%d", json_object_get_int(psJson));
        psCharacter->uValue.u8Data = (uint8)json_object_get_int(psJson);
    } else if(psCharacter->eType == E_CHARACTERISTIC_IDENTIFY){
        eIdentify();
    }

    return E_HAP_STATUS_OK;
}
static teHapStatus eHandleGetCmd(tsCharacteristic *psCharacter)
{
    if(psCharacter->eType == E_CHARACTERISTIC_TARGET_POSITION){
        NOT_vPrintln(DBG_WINDOW_COVER, "Get Window Position");
    }
    return E_HAP_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsProfile *psWindowCoveringProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                             E_HAP_TYPE_WINDOW_COVERING, eAccessoryWindowCoveringInit, eHandleSetCmd,
                                             eHandleGetCmd, eIdentify);

    return psProfile;
}

teHapStatus eWindowCoveringProfileRelease(tsProfile *psProfile)
{
    eProfileRelease(psProfile);
    return E_HAP_STATUS_OK;
}
