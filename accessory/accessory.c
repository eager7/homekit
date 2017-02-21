/****************************************************************************
 *
 * MODULE:             accessory.c
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
#include "accessory.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_ACC 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, char *psName, char *psSerialNumber, char *psManufacturer, char *psModel);
static teHapStatus eAccessoryCategoriesInit(tsAccessory *psAccessory, teAccessoryType eType);
static teHapStatus eAccessoryLightBulbInit(tsAccessory *psAccessory);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsAccessoryInformation sAccessoryInformation;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus eAccessoryInit(tsAccessory *psAccessory, char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel, teAccessoryType eType)
{
    DBG_vPrintln(DBG_ACC, "Init accessory type:%d", eType);
    CHECK_POINTER(psAccessory, E_HAP_MEMORY_ERROR);
    psAccessory->u64AIDs = u64DeviceID;
    psAccessory->eAccessoryType = eType;
    eAccessoryInformationInit(psAccessory, psName, psSerialNumber, psManufacturer, psModel);
    eAccessoryCategoriesInit(psAccessory, eType);

    return E_HAP_OK;
}

teHapStatus eAccessoryFinished(tsAccessory *psAccessory)
{
    WAR_vPrintln(DBG_ACC, "Finished accessory type:%d", psAccessory->eAccessoryType);
    FREE(psAccessory->sService.psCharacteristics);
    return E_HAP_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, char *psName, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    psAccessory->eInformation.psType = "public.hap.service.accessory-information";
    psAccessory->eInformation.eIID = E_SERVICE_ACCESSORY_INFORMATION;

    psAccessory->eInformation.sCharacteristics[0].eIID = E_CHARACTERISTIC_IDENTIFY;
    psAccessory->eInformation.sCharacteristics[0].psType = "public.hap.characteristic.identify";
    psAccessory->eInformation.sCharacteristics[0].eFormat = E_TYPE_BOOL;
    psAccessory->eInformation.sCharacteristics[0].uValue.bData = T_FALSE;
    psAccessory->eInformation.sCharacteristics[0].u8Permissions = E_PERM_PAIRED_WRITE;

    psAccessory->eInformation.sCharacteristics[1].eIID = E_CHARACTERISTIC_MANUFACTURER;
    psAccessory->eInformation.sCharacteristics[1].psType = "public.hap.characteristic.manufacturer";
    psAccessory->eInformation.sCharacteristics[1].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[1].uValue.psData = psManufacturer;
    psAccessory->eInformation.sCharacteristics[1].u8Permissions = E_PERM_PAIRED_READ;

    psAccessory->eInformation.sCharacteristics[2].eIID = E_CHARACTERISTIC_MODEL;
    psAccessory->eInformation.sCharacteristics[2].psType = "public.hap.characteristic.model";
    psAccessory->eInformation.sCharacteristics[2].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[2].uValue.psData = psModel;
    psAccessory->eInformation.sCharacteristics[2].u8Permissions = E_PERM_PAIRED_READ;

    psAccessory->eInformation.sCharacteristics[3].eIID = E_CHARACTERISTIC_NAME;
    psAccessory->eInformation.sCharacteristics[3].psType = "public.hap.characteristic.name";
    psAccessory->eInformation.sCharacteristics[3].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[3].uValue.psData = psName;
    psAccessory->eInformation.sCharacteristics[3].u8Permissions = E_PERM_PAIRED_READ;

    psAccessory->eInformation.sCharacteristics[4].eIID = E_CHARACTERISTIC_SERIAL_NUMBER;
    psAccessory->eInformation.sCharacteristics[4].psType = "public.hap.characteristic.serial-number";
    psAccessory->eInformation.sCharacteristics[4].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[4].uValue.psData = psSerialNumber;
    psAccessory->eInformation.sCharacteristics[4].u8Permissions = E_PERM_PAIRED_READ;

    //Option
    psAccessory->eInformation.sCharacteristics[5].eIID = E_CHARACTERISTIC_FIRMWARE_VERSION;
    psAccessory->eInformation.sCharacteristics[5].psType = "public.hap.characteristic.firmware.revision";
    psAccessory->eInformation.sCharacteristics[5].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[5].uValue.psData = "1.0";
    psAccessory->eInformation.sCharacteristics[5].u8Permissions = E_PERM_PAIRED_READ;

    psAccessory->eInformation.sCharacteristics[6].eIID = E_CHARACTERISTIC_HARDWARE_VERSION;
    psAccessory->eInformation.sCharacteristics[6].psType = "public.hap.characteristic.hardware.revision";
    psAccessory->eInformation.sCharacteristics[6].eFormat = E_TYPE_STRING;
    psAccessory->eInformation.sCharacteristics[6].uValue.psData = "1.0";
    psAccessory->eInformation.sCharacteristics[6].u8Permissions = E_PERM_PAIRED_READ;

    return E_HAP_OK;
}

static teHapStatus eAccessoryCategoriesInit(tsAccessory *psAccessory, teAccessoryType eType)
{
    switch(eType){
        case E_HAP_TYPE_LIGHT_BULB:
            eAccessoryLightBulbInit(psAccessory);
            break;
        default:
            break;
    }
    return E_HAP_OK;
}

static teHapStatus eAccessoryLightBulbInit(tsAccessory *psAccessory)
{
    psAccessory->sService.eIID = E_SERVICE_LIGHT_BULB;
    psAccessory->sService.psType = "public.hap.service.lightbulb";

    psAccessory->sService.psCharacteristics = (tsCharacteristic*)malloc(sizeof(tsCharacteristic)*E_NUM_OF_SERVICE_LIGHT_BULB);
    CHECK_POINTER(psAccessory->sService.psCharacteristics, E_HAP_MEMORY_ERROR);
    memset(psAccessory->sService.psCharacteristics, 0, sizeof(tsCharacteristic)*E_NUM_OF_SERVICE_LIGHT_BULB);

    psAccessory->sService.psCharacteristics[0].eIID = E_CHARACTERISTIC_ON;
    psAccessory->sService.psCharacteristics[0].psType = "public.hap.characteristic.on";
    psAccessory->sService.psCharacteristics[0].eFormat = E_TYPE_BOOL;
    psAccessory->sService.psCharacteristics[0].uValue.bData = T_FALSE;
    psAccessory->sService.psCharacteristics[0].u8Permissions = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOTIFICATION;

    psAccessory->sService.psCharacteristics[1].eIID = E_CHARACTERISTIC_BRIGHTNESS;
    psAccessory->sService.psCharacteristics[1].psType = "public.hap.characteristic.brightness";
    psAccessory->sService.psCharacteristics[1].eFormat = E_TYPE_INT;
    psAccessory->sService.psCharacteristics[1].uValue.iData = 100;
    psAccessory->sService.psCharacteristics[1].sSetupValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[1].sSetupValue.uData.iValue = 1;
    psAccessory->sService.psCharacteristics[1].eUnit = E_UNIT_PERCENTAGE;
    psAccessory->sService.psCharacteristics[1].sMinimumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[1].sMinimumValue.uData.iValue = 0;
    psAccessory->sService.psCharacteristics[1].sMaximumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[1].sMaximumValue.uData.iValue = 100;
    psAccessory->sService.psCharacteristics[1].u8Permissions = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOTIFICATION;

    psAccessory->sService.psCharacteristics[2].eIID = E_CHARACTERISTIC_HUE;
    psAccessory->sService.psCharacteristics[2].psType = "public.hap.characteristic.hue";
    psAccessory->sService.psCharacteristics[2].eFormat = E_TYPE_FLOAT;
    psAccessory->sService.psCharacteristics[2].uValue.fData = 360;
    psAccessory->sService.psCharacteristics[2].sSetupValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[2].sSetupValue.uData.fValue = 1;
    psAccessory->sService.psCharacteristics[2].eUnit = E_UNIT_ARCDEGREES;
    psAccessory->sService.psCharacteristics[2].sMinimumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[2].sMinimumValue.uData.fValue = 0;
    psAccessory->sService.psCharacteristics[2].sMaximumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[2].sMaximumValue.uData.fValue = 360;
    psAccessory->sService.psCharacteristics[2].u8Permissions = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOTIFICATION;

    psAccessory->sService.psCharacteristics[3].eIID = E_CHARACTERISTIC_NAME;
    psAccessory->sService.psCharacteristics[3].psType = "public.hap.characteristic.name";
    psAccessory->sService.psCharacteristics[3].eFormat = E_TYPE_STRING;
    psAccessory->sService.psCharacteristics[3].uValue.psData = "light";
    psAccessory->sService.psCharacteristics[3].u8Permissions = E_PERM_PAIRED_READ;

    psAccessory->sService.psCharacteristics[4].eIID = E_CHARACTERISTIC_SATURATION;
    psAccessory->sService.psCharacteristics[4].psType = "public.hap.characteristic.saturation";
    psAccessory->sService.psCharacteristics[4].eFormat = E_TYPE_FLOAT;
    psAccessory->sService.psCharacteristics[4].uValue.fData = 100;
    psAccessory->sService.psCharacteristics[4].eUnit = E_UNIT_PERCENTAGE;
    psAccessory->sService.psCharacteristics[4].sSetupValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[4].sSetupValue.uData.fValue = 1;
    psAccessory->sService.psCharacteristics[4].sMinimumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[4].sMinimumValue.uData.fValue = 0;
    psAccessory->sService.psCharacteristics[4].sMaximumValue.bEnable = T_TRUE;
    psAccessory->sService.psCharacteristics[4].sMaximumValue.uData.fValue = 100;
    psAccessory->sService.psCharacteristics[4].u8Permissions = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOTIFICATION;

    return E_HAP_OK;
}

json_object* psGetAccessoryInfoJson(tsAccessory *psAccessory)
{
    CHECK_POINTER(psAccessory, NULL);
    json_object *psArrayPerms = NULL;
    json_object *psJsonCharacter = NULL;
    json_object *psJsonRet = json_object_new_object();
    json_object *psJsonAccessory = json_object_new_object();
    json_object *psJsonService = json_object_new_object();

    json_object *psArrayAccessories = json_object_new_array();
    json_object *psArrayServices = json_object_new_array();
    json_object *psArrayCharacteristics = json_object_new_array();
    if((NULL == psJsonRet) || (NULL == psJsonAccessory) || (NULL == psJsonService)  ||
       (NULL == psArrayAccessories) || (NULL == psArrayServices) || (NULL == psArrayCharacteristics))
    {
        goto ERR;
    }

    for (int j = 0; j < 6; ++j) {
        psJsonCharacter = json_object_new_object();
        if(NULL == psJsonCharacter) goto ERR;
        json_object_object_add(psJsonCharacter, "type", json_object_new_string(psAccessory->eInformation.sCharacteristics[j].psType));
        switch (psAccessory->eInformation.sCharacteristics[j].eFormat){
            case E_TYPE_BOOL:{
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->eInformation.sCharacteristics[j].uValue.bData));
                json_object_object_add(psJsonCharacter, "format", json_object_new_string("bool"));
            } break;
            case E_TYPE_INT: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->eInformation.sCharacteristics[j].uValue.iData));
                json_object_object_add(psJsonCharacter, "format", json_object_new_string("int"));
            } break;
            case E_TYPE_FLOAT:  {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->eInformation.sCharacteristics[j].uValue.fData));
                json_object_object_add(psJsonCharacter, "format", json_object_new_string("float"));
            } break;
            case E_TYPE_STRING: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_string(psAccessory->eInformation.sCharacteristics[j].uValue.psData));
                json_object_object_add(psJsonCharacter, "format", json_object_new_string("string"));
            } break;
            default: break;
        }
        json_object_object_add(psJsonCharacter, "iid", json_object_new_int(psAccessory->eInformation.sCharacteristics[j].eIID));

        /*Perms Json Format*/
        psArrayPerms = json_object_new_array();
        if(NULL == psArrayPerms) goto ERR;
        if(psAccessory->eInformation.sCharacteristics[j].u8Permissions & E_PERM_PAIRED_READ){
            json_object_array_add(psArrayPerms, json_object_new_string("pr"));
        }
        if(psAccessory->eInformation.sCharacteristics[j].u8Permissions & E_PERM_PAIRED_WRITE){
            json_object_array_add(psArrayPerms, json_object_new_string("pw"));
        }
        if(psAccessory->eInformation.sCharacteristics[j].u8Permissions & E_PERM_EVENT_NOTIFICATION){
            json_object_array_add(psArrayPerms, json_object_new_string("ev"));
        }
        json_object_object_add(psJsonCharacter, "perms", psArrayPerms);
        json_object_array_add(psArrayCharacteristics, psJsonCharacter);
    }
    json_object_object_add(psJsonService, "characteristics", psArrayCharacteristics);
    json_object_object_add(psJsonService, "type", json_object_new_int(psAccessory->eAccessoryType));
    json_object_object_add(psJsonService, "iid", json_object_new_int(psAccessory->eInformation.eIID));
    json_object_array_add(psArrayServices, psJsonService);
    json_object_object_add(psJsonAccessory, "services", psArrayServices);
    json_object_object_add(psJsonAccessory, "aid", json_object_new_int64((int64_t)psAccessory->u64AIDs));
    json_object_array_add(psArrayAccessories, psJsonAccessory);
    json_object_object_add(psJsonRet, "accessories", psArrayAccessories);

    return psJsonRet;
ERR:
    if(psJsonRet)json_object_put(psJsonRet);
    if(psJsonAccessory)json_object_put(psJsonAccessory);
    if(psJsonService)json_object_put(psJsonService);
    if(psJsonCharacter)json_object_put(psJsonCharacter);
    if(psArrayPerms)json_object_put(psArrayPerms);
    if(psArrayAccessories)json_object_put(psArrayAccessories);
    if(psArrayServices)json_object_put(psArrayServices);
    if(psArrayCharacteristics)json_object_put(psArrayCharacteristics);
    return NULL;
}