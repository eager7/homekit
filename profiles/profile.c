/****************************************************************************
 *
 * MODULE:             profile.c
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
#include "profile.h"
#include "http_handle.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_WINDOW_COVER 1
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
static json_object *psGetAccessoryInfo(const tsAccessory *psAccessory)
{
    CHECK_POINTER(psAccessory, NULL);
    json_object *psArrayPerms = NULL;
    json_object *psJsonCharacter = NULL;
    json_object *psArrayCharacteristics =NULL;
    json_object *psJsonService = NULL;
    json_object *psJsonRet = json_object_new_object();
    json_object *psJsonAccessory = json_object_new_object();

    json_object *psArrayAccessories = json_object_new_array();
    json_object *psArrayServices = json_object_new_array();
    if((NULL == psJsonRet) || (NULL == psJsonAccessory) ||
       (NULL == psArrayAccessories) || (NULL == psArrayServices))
    {
        goto ERR;
    }

    for (int i = 0; i < psAccessory->u8NumServices; ++i) {
        psArrayCharacteristics = json_object_new_array();
        if(NULL == psArrayCharacteristics) goto ERR;
        for (int j = 0; j < psAccessory->psService[i].u8NumCharacteristics; ++j) {
            psJsonCharacter = json_object_new_object();
            if(NULL == psJsonCharacter) goto ERR;
            char temp[10] = {0};
            snprintf(temp, sizeof(temp), "%X", psAccessory->psService[i].psCharacteristics[j].eType);
            json_object_object_add(psJsonCharacter, "type", json_object_new_string(temp));
            switch (psAccessory->psService[i].psCharacteristics[j].eFormat){
                case E_TYPE_BOOL:{
                    if(psAccessory->psService[i].psCharacteristics[j].eType != E_CHARACTERISTIC_IDENTIFY)
                        json_object_object_add(psJsonCharacter, "value", json_object_new_boolean(psAccessory->psService[i].psCharacteristics[j].uValue.bData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("bool"));
                } break;
                case E_TYPE_UINT8: {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.u8Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.u8Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u8Data));
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARC_DEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u8Data));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.u8Data));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("uint8"));
                } break;
                case E_TYPE_UINT16: {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.u16Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.u16Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u16Data));
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARC_DEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u16Data));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.u16Data));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("uint16"));
                } break;
                case E_TYPE_UINT32: {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.u8Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.u8Data));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u8Data));
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARC_DEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.u8Data));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.u8Data));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("uint8"));
                } break;
                case E_TYPE_INT: {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.iData));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.iData));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.iData));
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARC_DEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.iData));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.iData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("int"));
                } break;
                case E_TYPE_FLOAT:  {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_double(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.fData));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_double(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.fData));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARC_DEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_double(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.fData));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_double(psAccessory->psService[i].psCharacteristics[j].uValue.fData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("float"));
                } break;
                case E_TYPE_STRING: {
                    json_object_object_add(psJsonCharacter, "value", json_object_new_string(psAccessory->psService[i].psCharacteristics[j].uValue.psData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("string"));
                } break;
                default: break;
            }

            json_object_object_add(psJsonCharacter, "iid", json_object_new_int64((int64_t)psAccessory->psService[i].psCharacteristics[j].u64IID));

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
        psJsonService = json_object_new_object();
        if (NULL == psJsonService) {
            goto ERR;
        }
        json_object_object_add(psJsonService, "iid", json_object_new_int64((int64_t)psAccessory->psService[i].u64IID));
        char temp[5] = {0};
        snprintf(temp, sizeof(temp), "%X", psAccessory->psService[i].eType);
        json_object_object_add(psJsonService, "type", json_object_new_string(temp));
        json_object_object_add(psJsonService, "characteristics", psArrayCharacteristics);
        json_object_array_add(psArrayServices, psJsonService);
    }
    json_object_object_add(psJsonAccessory, "aid", json_object_new_int64((int64_t)psAccessory->u64AIDs));
    json_object_object_add(psJsonAccessory, "services", psArrayServices);
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

static json_object *psGetCharacteristicInfo(const tsAccessory *psAccessory, const char *psCmd)
{
    json_object *psJsonCharacter = NULL;
    json_object *psJsonReturn = json_object_new_object();
    json_object *psArrayCharacter = json_object_new_array();
    char auId[MIBF] = {0};
    sscanf(psCmd, "/characteristics?id=%[^\n]", auId);
    char *temp_once = strtok(auId, ",");
    CHECK_POINTER(temp_once, NULL);
    uint64 u64AID = 0, u64IID = 0;
    while(T_TRUE){
        sscanf(temp_once, "%llu.%llu", &u64AID, &u64IID);
        DBG_vPrintln(DBG_WINDOW_COVER, "temp_once:%s,aid:%llu,iid:%llu\n", temp_once, u64AID, u64IID);
        tsCharacteristic *psCharacter = psAccessoryGetCharacterByIID(psAccessory, u64AID, u64IID);
        if((NULL == psCharacter) || !(psCharacter->u8Perms & E_PERM_PAIRED_READ)){
            FREE(psJsonReturn);
            FREE(psArrayCharacter);
            return NULL;
        }
        psJsonCharacter = json_object_new_object();
        json_object_object_add(psJsonCharacter, "aid", json_object_new_int64((int64_t)u64AID));
        json_object_object_add(psJsonCharacter, "iid", json_object_new_int64((int64_t)u64IID));
        switch (psCharacter->eFormat){
            case E_TYPE_BOOL:{
                json_object_object_add(psJsonCharacter, "value", json_object_new_boolean(psCharacter->uValue.bData));
            } break;
            case E_TYPE_INT: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psCharacter->uValue.iData));
            } break;
            case E_TYPE_UINT8: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psCharacter->uValue.u8Data));
            } break;
            case E_TYPE_UINT16: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psCharacter->uValue.u16Data));
            } break;
            case E_TYPE_UINT32: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psCharacter->uValue.u32Data));
            } break;
            case E_TYPE_FLOAT:  {
                json_object_object_add(psJsonCharacter, "value", json_object_new_double(psCharacter->uValue.fData));
            } break;
            case E_TYPE_STRING: {
                json_object_object_add(psJsonCharacter, "value", json_object_new_string(psCharacter->uValue.psData));
            } break;
            default: break;
        }

        json_object_array_add(psArrayCharacter, psJsonCharacter);
        temp_once = strtok(NULL, ",");
        if(NULL == temp_once)
            break;
    }
    json_object_object_add(psJsonReturn, "characteristics", psArrayCharacter);
    return psJsonReturn;
}

static teHapStatus eSetCharacteristicInfo(tsAccessory *psAccessory, const uint8 *psCmd, uint8 **ppsBuffer, uint16 *pu16Len, feHandleRequest fCallback)
{
    DBG_vPrintln(DBG_WINDOW_COVER, "eSetCharacteristicInfo");
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

        tsCharacteristic *psCharacter = psAccessoryGetCharacterByIID(psAccessory, u64AID, u64IID);
        if(NULL == psCharacter){
            ERR_vPrintln(T_TRUE, "Can't Find Character");
            goto FAILED;
        }
        if(json_object_object_get_ex(psJsonCharacter, "ev", &psJsonEvent)){
            bEvent = (bool_t)json_object_get_boolean(psJsonEvent);
            DBG_vPrintln(DBG_WINDOW_COVER, "event:%d", bEvent);
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
            fCallback(psCharacter, psJsonValue);
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
tsProfile *psProfileGenerate(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel,
                             teAccessoryCategories eType, fpeInitCategory fsInitCategory, feHandleRequest eHandleRequest)
{
    tsProfile *psProfile = (tsProfile*)calloc(1, sizeof(tsProfile));
    CHECK_POINTER(psProfile, NULL);
    psProfile->psGetAccessoryInfo = psGetAccessoryInfo;
    psProfile->psGetCharacteristicInfo = psGetCharacteristicInfo;
    psProfile->peSetCharacteristicInfo = eSetCharacteristicInfo;
    psProfile->peInitCategory = fsInitCategory;
    psProfile->eHandleRequest = eHandleRequest;
    psProfile->psAccessory = psAccessoryGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel, eType);
    psProfile->peInitCategory(psProfile->psAccessory);
    return psProfile;
}

teHapStatus eProfileRelease(tsProfile *psProfile)
{
    eAccessoryRelease(psProfile->psAccessory);
    FREE(psProfile);
    return E_HAP_STATUS_OK;
}