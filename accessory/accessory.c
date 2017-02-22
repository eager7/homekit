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
#define DBG_ACC 0
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
    DBG_vPrintln(DBG_ACC, "Init accessory:%s type:%d", psName, eType);
    CHECK_POINTER(psAccessory, E_HAP_STATUS_MEMORY_ERROR);
    psAccessory->u64AIDs = E_SERVICE_ACCESSORY_OBJECT;
    psAccessory->u64DeviceID = u64DeviceID;
    psAccessory->eAccessoryType = eType;
    eAccessoryInformationInit(psAccessory, psName, psSerialNumber, psManufacturer, psModel);
    eAccessoryCategoriesInit(psAccessory, eType);

    return E_HAP_STATUS_OK;
}

teHapStatus eAccessoryFinished(tsAccessory *psAccessory)
{
    WAR_vPrintln(DBG_ACC, "Finished accessory type:%d", psAccessory->eAccessoryType);
    FREE(psAccessory->psService->psCharacteristics);
    FREE(psAccessory->psService);
    return E_HAP_STATUS_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static char *psFormatString(teTypeFormat eType, tuCharValue uData)
{
    static char asRet[10] = {10};
    memset(asRet, 0, sizeof(asRet));
    switch (eType){
        case E_TYPE_BOOL:{ sprintf(asRet, "%x", uData.bData); } break;
        case E_TYPE_UINT8:{ sprintf(asRet, "%x", uData.iData); } break;
        case E_TYPE_UINT16:{ sprintf(asRet, "%x", uData.iData); } break;
        case E_TYPE_UINT32:{ sprintf(asRet, "%x", uData.iData); } break;
        case E_TYPE_UINT64:{ sprintf(asRet, "%x", uData.iData); } break;
        case E_TYPE_INT:{ sprintf(asRet, "%x", uData.iData); } break;
        case E_TYPE_FLOAT:{ sprintf(asRet, "%x", uData.fData); } break;
        //case E_TYPE_STRING:{ sprintf(psRet, "%s", uData.bData); } break;
        default:break;
    }
    return asRet;
}
static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, char *psName, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    DBG_vPrintln(DBG_ACC, "eAccessoryInformationInit:%s", psName);
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, "public.hap.service.accessory-information", E_SERVICE_ACCESSORY_INFORMATION, &psService);

    tsCharacteristic asCharaTemp[7];
    memset(asCharaTemp, 0, sizeof(tsCharacteristic)*7);

    asCharaTemp[0].eIID = E_CHARACTERISTIC_IDENTIFY;
    asCharaTemp[0].psType = "public.hap.characteristic.identify";
    asCharaTemp[0].eFormat = E_TYPE_BOOL;
    asCharaTemp[0].uValue.bData = T_FALSE;
    asCharaTemp[0].u8Perms = E_PERM_PAIRED_WRITE;
    eServiceAddCharacter(psService, asCharaTemp[0], NULL);

    asCharaTemp[1].eIID = E_CHARACTERISTIC_MANUFACTURER;
    asCharaTemp[1].psType = "public.hap.characteristic.manufacturer";
    asCharaTemp[1].eFormat = E_TYPE_STRING;
    asCharaTemp[1].uValue.psData = psManufacturer;
    asCharaTemp[1].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[1], NULL);

    asCharaTemp[2].eIID = E_CHARACTERISTIC_MODEL;
    asCharaTemp[2].psType = "public.hap.characteristic.model";
    asCharaTemp[2].eFormat = E_TYPE_STRING;
    asCharaTemp[2].uValue.psData = psModel;
    asCharaTemp[2].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[2], NULL);

    asCharaTemp[3].eIID = E_CHARACTERISTIC_NAME;
    asCharaTemp[3].psType = "public.hap.characteristic.name";
    asCharaTemp[3].eFormat = E_TYPE_STRING;
    asCharaTemp[3].uValue.psData = psName;
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);

    asCharaTemp[4].eIID = E_CHARACTERISTIC_SERIAL_NUMBER;
    asCharaTemp[4].psType = "public.hap.characteristic.serial-number";
    asCharaTemp[4].eFormat = E_TYPE_STRING;
    asCharaTemp[4].uValue.psData = psSerialNumber;
    asCharaTemp[4].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[4], NULL);

    //Option
    asCharaTemp[5].eIID = E_CHARACTERISTIC_FIRMWARE_VERSION;
    asCharaTemp[5].psType = "public.hap.characteristic.firmware.revision";
    asCharaTemp[5].eFormat = E_TYPE_STRING;
    asCharaTemp[5].uValue.psData = "1.0";
    asCharaTemp[5].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[5], NULL);

    asCharaTemp[6].eIID = E_CHARACTERISTIC_HARDWARE_VERSION;
    asCharaTemp[6].psType = "public.hap.characteristic.hardware.revision";
    asCharaTemp[6].eFormat = E_TYPE_STRING;
    asCharaTemp[6].uValue.psData = "1.0";
    asCharaTemp[6].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[6], NULL);
    return E_HAP_STATUS_OK;
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
    return E_HAP_STATUS_OK;
}

static teHapStatus eAccessoryLightBulbInit(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_ACC, "eAccessoryLightBulbInit");
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, "public.hap.service.lightbulb", E_SERVICE_LIGHT_BULB, &psService);

    tsCharacteristic asCharaTemp[E_NUM_OF_SERVICE_LIGHT_BULB];
    memset(asCharaTemp, 0, sizeof(tsCharacteristic)*E_NUM_OF_SERVICE_LIGHT_BULB);

    asCharaTemp[0].eIID = E_CHARACTERISTIC_ON;
    asCharaTemp[0].psType = "public.hap.characteristic.on";
    asCharaTemp[0].eFormat = E_TYPE_BOOL;
    asCharaTemp[0].uValue.bData = T_FALSE;
    asCharaTemp[0].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[0], NULL);

    asCharaTemp[1].eIID = E_CHARACTERISTIC_BRIGHTNESS;
    asCharaTemp[1].psType = "public.hap.characteristic.brightness";
    asCharaTemp[1].eFormat = E_TYPE_INT;
    asCharaTemp[1].uValue.iData = 100;
    asCharaTemp[1].sSetupValue.bEnable = T_TRUE;
    asCharaTemp[1].sSetupValue.uData.iValue = 1;
    asCharaTemp[1].eUnit = E_UNIT_PERCENTAGE;
    asCharaTemp[1].sMinimumValue.bEnable = T_TRUE;
    asCharaTemp[1].sMinimumValue.uData.iValue = 0;
    asCharaTemp[1].sMaximumValue.bEnable = T_TRUE;
    asCharaTemp[1].sMaximumValue.uData.iValue = 100;
    asCharaTemp[1].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[1], NULL);

    asCharaTemp[2].eIID = E_CHARACTERISTIC_HUE;
    asCharaTemp[2].psType = "public.hap.characteristic.hue";
    asCharaTemp[2].eFormat = E_TYPE_FLOAT;
    asCharaTemp[2].uValue.fData = 360;
    asCharaTemp[2].sSetupValue.bEnable = T_TRUE;
    asCharaTemp[2].sSetupValue.uData.fValue = 1;
    asCharaTemp[2].eUnit = E_UNIT_ARCDEGREES;
    asCharaTemp[2].sMinimumValue.bEnable = T_TRUE;
    asCharaTemp[2].sMinimumValue.uData.fValue = 0;
    asCharaTemp[2].sMaximumValue.bEnable = T_TRUE;
    asCharaTemp[2].sMaximumValue.uData.fValue = 360;
    asCharaTemp[2].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[2], NULL);

    asCharaTemp[3].eIID = E_CHARACTERISTIC_NAME;
    asCharaTemp[3].psType = "public.hap.characteristic.name";
    asCharaTemp[3].eFormat = E_TYPE_STRING;
    asCharaTemp[3].uValue.psData = "light";
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);

    asCharaTemp[4].eIID = E_CHARACTERISTIC_SATURATION;
    asCharaTemp[4].psType = "public.hap.characteristic.saturation";
    asCharaTemp[4].eFormat = E_TYPE_FLOAT;
    asCharaTemp[4].uValue.fData = 100;
    asCharaTemp[4].eUnit = E_UNIT_PERCENTAGE;
    asCharaTemp[4].sSetupValue.bEnable = T_TRUE;
    asCharaTemp[4].sSetupValue.uData.fValue = 1;
    asCharaTemp[4].sMinimumValue.bEnable = T_TRUE;
    asCharaTemp[4].sMinimumValue.uData.fValue = 0;
    asCharaTemp[4].sMaximumValue.bEnable = T_TRUE;
    asCharaTemp[4].sMaximumValue.uData.fValue = 100;
    asCharaTemp[4].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[4], NULL);

    return E_HAP_STATUS_OK;
}

json_object* psGetAccessoryInfoJson(tsAccessory *psAccessory)
{
    CHECK_POINTER(psAccessory, NULL);
    json_object *psArrayPerms = NULL;
    json_object *psJsonCharacter = NULL;
    json_object *psArrayCharacteristics =NULL;
    json_object *psJsonRet = json_object_new_object();
    json_object *psJsonAccessory = json_object_new_object();
    json_object *psJsonService = json_object_new_object();

    json_object *psArrayAccessories = json_object_new_array();
    json_object *psArrayServices = json_object_new_array();
    if((NULL == psJsonRet) || (NULL == psJsonAccessory) || (NULL == psJsonService)  ||
       (NULL == psArrayAccessories) || (NULL == psArrayServices))
    {
        goto ERR;
    }

    for (int i = 0; i < psAccessory->u8NumServices; ++i) {
        psArrayCharacteristics = json_object_new_array();
        if(NULL == psArrayCharacteristics) goto ERR;
        for (int j = 0; j < psAccessory->psService[i].u8NumCharacteristics; ++j) {
            INF_vPrintln(1, "[%d][%d]", i, j);
            psJsonCharacter = json_object_new_object();
            if(NULL == psJsonCharacter) goto ERR;
            char temp[5] = {0};
            snprintf(temp, sizeof(temp), "%02X", psAccessory->psService[i].psCharacteristics[j].eIID);
            json_object_object_add(psJsonCharacter, "type", json_object_new_string(temp));
            switch (psAccessory->psService[i].psCharacteristics[j].eFormat){
                case E_TYPE_BOOL:{
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.bData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("bool"));
                } break;
                case E_TYPE_INT: {
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.iData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("int"));
                } break;
                case E_TYPE_FLOAT:  {
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.fData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("float"));
                } break;
                case E_TYPE_STRING: {
                    json_object_object_add(psJsonCharacter, "value", json_object_new_string(psAccessory->psService[i].psCharacteristics[j].uValue.psData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("string"));
                } break;
                default: break;
            }
            json_object_object_add(psJsonCharacter, "iid", json_object_new_int(j));

            /*Perms Json Format*/
            psArrayPerms = json_object_new_array();
            if(NULL == psArrayPerms) goto ERR;
            if(psAccessory->psService[i].psCharacteristics[j].u8Perms & E_PERM_PAIRED_READ){
                json_object_array_add(psArrayPerms, json_object_new_string("pr"));
            }
            if(psAccessory->psService[i].psCharacteristics[j].u8Perms & E_PERM_PAIRED_WRITE){
                json_object_array_add(psArrayPerms, json_object_new_string("pw"));
            }
            if(psAccessory->psService[i].psCharacteristics[j].u8Perms & E_PERM_EVENT_NOT){
                json_object_array_add(psArrayPerms, json_object_new_string("ev"));
            }
            json_object_object_add(psJsonCharacter, "perms", psArrayPerms);
            json_object_array_add(psArrayCharacteristics, psJsonCharacter);
        }
        INF_vPrintln(1, "%s", json_object_get_string(psArrayCharacteristics));
        json_object_object_add(psJsonService, "characteristics", psArrayCharacteristics);
        char temp[5] = {0};
        snprintf(temp, sizeof(temp), "%02X", psAccessory->psService[i].eIID);
        json_object_object_add(psJsonService, "type", json_object_new_string(temp));
        json_object_object_add(psJsonService, "iid", json_object_new_int(i));
        json_object_array_add(psArrayServices, psJsonService);
    }
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

teHapStatus eAccessoryAddService(tsAccessory *psAccessory, char *psType, teServiceUUID eIID, tsService **ppsService)
{
    tsService *psService;
    for (int i = 0; i < psAccessory->u8NumServices; ++i) {
        if(psAccessory->psService[i].eIID == eIID){
            INF_vPrintln(DBG_ACC, "Duplicate Service");
            if(ppsService) *ppsService = &psAccessory->psService[i];
            return E_HAP_STATUS_DUPLICATE;
        }
    }
    DBG_vPrintln(DBG_ACC, "Create New Service[%d]:0x%016x", psAccessory->u8NumServices + 1, eIID);
    psService = realloc(psAccessory->psService, sizeof(tsService)*(psAccessory->u8NumServices + 1));
    CHECK_POINTER(psService, E_HAP_STATUS_ERROR);
    psAccessory->psService = psService;

    memset(&psAccessory->psService[psAccessory->u8NumServices], 0, sizeof(tsService));
    psService = &psAccessory->psService[psAccessory->u8NumServices];
    psAccessory->u8NumServices ++;

    psService->eIID = eIID;
    psService->psType = psType;
    if(ppsService) *ppsService = psService;
    return E_HAP_STATUS_OK;
}

teHapStatus eServiceAddCharacter(tsService *psService, tsCharacteristic sCharaIn, tsCharacteristic **ppCharaOut)
{
    tsCharacteristic *psChara;
    for (int i = 0; i < psService->u8NumCharacteristics; ++i) {
        if(psService->psCharacteristics[i].eIID == sCharaIn.eIID){
            INF_vPrintln(DBG_ACC, "Duplicate Characteristics:0x%16x", sCharaIn.eIID);
            if(ppCharaOut) *ppCharaOut = &psService->psCharacteristics[i];
            return E_HAP_STATUS_DUPLICATE;
        }
    }
    DBG_vPrintln(DBG_ACC, "Create New Characteristics[%d]:0x%016x", psService->u8NumCharacteristics + 1, sCharaIn.eIID);
    psChara = realloc(psService->psCharacteristics, sizeof(tsCharacteristic)*(psService->u8NumCharacteristics + 1));
    CHECK_POINTER(psChara, E_HAP_STATUS_ERROR);
    psService->psCharacteristics = psChara;

    memset(&psService->psCharacteristics[psService->u8NumCharacteristics], 0, sizeof(tsCharacteristic));
    psChara = &psService->psCharacteristics[psService->u8NumCharacteristics];
    psService->u8NumCharacteristics ++;

    memcpy(psChara, &sCharaIn, sizeof(tsCharacteristic));
    if(ppCharaOut) *ppCharaOut = psChara;
    return E_HAP_STATUS_OK;
}