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

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_PROFILE (1)
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
    DBG_vPrintln(DBG_PROFILE, "eAccessoryWindowCoveringInit");
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
    sCharaTemp.uValue.u8Data = 0;
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

static teHapStatus eSetCharacteristicInfo(tsAccessory *psAccessory, const uint8 *psCmd, uint8 **ppsBuffer, uint16 *pu16Len)
{
    DBG_vPrintln(DBG_PROFILE, "eSetCharacteristicInfo");
    teHttpCode eHttpCode = E_HTTP_STATUS_SUCCESS_NO_CONTENT;
    json_object *psJsonResp = json_object_new_object();
    json_object *psArrayResp = json_object_new_array();
    json_object *psJsonCmd = json_tokener_parse((const char*)psCmd);
    if(psJsonCmd == NULL || psJsonResp == NULL){
        ERR_vPrintln(T_TRUE, "Json Format Error");
        goto FAILED;
    }
    json_object *psArrayCmd = NULL;
    if(!json_object_object_get_ex(psJsonCmd, "characteristics", &psArrayCmd)){
        ERR_vPrintln(T_TRUE, "Can't Find characteristics");
        goto FAILED;
    }
    int iNum = json_object_array_length(psArrayCmd);
    for (int i = 0; i < iNum; ++i) {
        json_object *psJsonCharacter = json_object_array_get_idx(psArrayCmd, i);
        uint64 u64AID = 0, u64IID = 0;
        bool_t bEvent;
        json_object *psJsonAID = NULL, *psJsonIID = NULL, *psJsonEvent = NULL, *psJsonValue = NULL;
        if(!json_object_object_get_ex(psJsonCharacter, "aid", &psJsonAID)){
            ERR_vPrintln(T_TRUE, "Can't Find aid");
            goto FAILED;
        }
        u64AID = (uint64)json_object_get_int64(psJsonAID);
        if(!json_object_object_get_ex(psJsonCharacter, "iid", &psJsonIID)){
            ERR_vPrintln(T_TRUE, "Can't Find iid");
            goto FAILED;
        }
        u64IID = (uint64)json_object_get_int64(psJsonIID);

        tsCharacteristic *psCharacter = NULL;
        if(E_HAP_STATUS_ERROR == eAccessoryGetCharacter(psAccessory, u64AID, u64IID, &psCharacter)){
            ERR_vPrintln(T_TRUE, "Can't Find Character");
            goto FAILED;
        }
        if(json_object_object_get_ex(psJsonCharacter, "ev", &psJsonEvent)){
            bEvent = (bool_t)json_object_get_boolean(psJsonEvent);
            DBG_vPrintln(DBG_PROFILE, "event:%d", bEvent);
        } else if(json_object_object_get_ex(psJsonCharacter, "value", &psJsonValue)){
            json_object *psJsonRespTemp = json_object_new_object();
            json_object_object_add(psJsonRespTemp, "aid", json_object_new_int64((int64_t)u64AID));
            json_object_object_add(psJsonRespTemp, "iid", json_object_new_int64((int64_t)u64IID));
            if(!(psCharacter->u8Perms & E_PERM_PAIRED_WRITE)){
                if(iNum > 1){
                    eHttpCode = E_HTTP_STATUS_SUCCESS_MULTI_STATUS;
                }
                json_object_object_add(psJsonRespTemp, "status", json_object_new_int(E_HAP_STATUS_CODE_READ_ONLY));
                json_object_array_add(psArrayResp, psJsonRespTemp);
                continue;
            } else {
                json_object_object_add(psJsonRespTemp, "status", json_object_new_int(0));
                json_object_array_add(psArrayResp, psJsonRespTemp);
            }
            switch (psCharacter->eFormat){
                case E_TYPE_INT:    psCharacter->uValue.iData   = json_object_get_int(psJsonValue);
                case E_TYPE_BOOL:   psCharacter->uValue.bData   = (bool_t)json_object_get_boolean(psJsonValue);
                case E_TYPE_UINT8:  psCharacter->uValue.u8Data  = (uint8)json_object_get_int(psJsonValue);
                case E_TYPE_UINT16: psCharacter->uValue.u16Data = (uint16)json_object_get_int(psJsonValue);
                case E_TYPE_UINT32: psCharacter->uValue.u32Data = (uint32)json_object_get_int(psJsonValue);
                case E_TYPE_FLOAT:  psCharacter->uValue.fData   = (float)json_object_get_double(psJsonValue);
                default:break;
            }
        }
    }
    if(eHttpCode == E_HTTP_STATUS_SUCCESS_MULTI_STATUS){
        json_object_object_add(psJsonResp, "characteristics", psArrayResp);
        *pu16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_MULTI_STATUS, "application/hap+json",
                      (uint8*)json_object_get_string(psJsonResp), (uint16) strlen(json_object_get_string(psJsonResp)), ppsBuffer);
        json_object_put(psJsonCmd);
        json_object_put(psJsonResp);
        return E_HAP_STATUS_OK;
    }
    *pu16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_NO_CONTENT, "application/hap+json", NULL, 0, ppsBuffer);
    json_object_put(psJsonCmd);
    json_object_put(psArrayResp);
    json_object_put(psJsonResp);
    return E_HAP_STATUS_OK;

FAILED:
    json_object_put(psJsonCmd);
    json_object_put(psArrayResp);
    json_object_put(psJsonResp);
    return E_HAP_STATUS_ERROR;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tsProfile *psWindowCoveringProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                             E_HAP_TYPE_WINDOW_COVERING, eAccessoryWindowCoveringInit, eSetCharacteristicInfo);
    return psProfile;
}

teHapStatus eWindowCoveringProfileRelease(tsProfile *psProfile)
{
    eProfileRelease(psProfile);
    return E_HAP_STATUS_OK;
}
