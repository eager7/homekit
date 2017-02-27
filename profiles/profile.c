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
#include "profile.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

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
static json_object *psGetAccessoryInfoJson(const tsAccessory *psAccessory)
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
                case E_TYPE_INT: {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.iValue));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.iValue));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.iValue));
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARCDEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.iValue));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.iData));
                    json_object_object_add(psJsonCharacter, "format", json_object_new_string("int"));
                } break;
                case E_TYPE_FLOAT:  {
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "minValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMinimumValue.uData.fValue));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.bEnable){
                        json_object_object_add(psJsonCharacter, "maxValue", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sMaximumValue.uData.fValue));
                    }
                    if(psAccessory->psService[i].psCharacteristics[j].sSetupValue.bEnable){
                        switch (psAccessory->psService[i].psCharacteristics[j].eUnit){
                            case E_UNIT_PERCENTAGE:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("percentage"));break;
                            case E_UNIT_ARCDEGREES:json_object_object_add(psJsonCharacter, "unit", json_object_new_string("arcdegrees"));break;
                            default: break;
                        }
                        json_object_object_add(psJsonCharacter, "minStep", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].sSetupValue.uData.fValue));
                    }
                    json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.fData));
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
    int aid = 0, iid = 0;
    while(T_TRUE){
        sscanf(temp_once, "%d.%d", &aid, &iid);
        DBG_vPrintln(1, "temp_once:%s,aid:%d,iid:%d\n", temp_once, aid, iid);
        if(psAccessory->u64AIDs == aid){
            for (int i = 0; i < psAccessory->u8NumServices; ++i) {
                for (int j = 0; j < psAccessory->psService[i].u8NumCharacteristics; ++j) {
                    if(psAccessory->psService[i].psCharacteristics[j].u64IID == iid){
                        psJsonCharacter = json_object_new_object();
                        json_object_object_add(psJsonCharacter, "aid", json_object_new_int64((int64_t)aid));
                        json_object_object_add(psJsonCharacter, "iid", json_object_new_int64((int64_t)iid));
                        switch (psAccessory->psService[i].psCharacteristics[j].eFormat){
                            case E_TYPE_BOOL:{
                                json_object_object_add(psJsonCharacter, "value", json_object_new_boolean(psAccessory->psService[i].psCharacteristics[j].uValue.bData));
                            } break;
                            case E_TYPE_INT: {
                                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.iData));
                            } break;
                            case E_TYPE_FLOAT:  {
                                json_object_object_add(psJsonCharacter, "value", json_object_new_int(psAccessory->psService[i].psCharacteristics[j].uValue.fData));
                            } break;
                            case E_TYPE_STRING: {
                                json_object_object_add(psJsonCharacter, "value", json_object_new_string(psAccessory->psService[i].psCharacteristics[j].uValue.psData));
                            } break;
                            default: break;
                        }
                    }
                }
            }
        }
        json_object_array_add(psArrayCharacter, psJsonCharacter);
        temp_once = strtok(NULL, ",");
        if(NULL == temp_once)
            break;
    }
    json_object_object_add(psJsonReturn, "characteristics", psArrayCharacter);
    return psJsonReturn;
}

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsProfile *psProfileNew(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel, teAccessoryType eType)
{
    tsProfile *psProfile = (tsProfile*)malloc(sizeof(tsProfile));
    CHECK_POINTER(psProfile, NULL);
    memset(psProfile, 0, sizeof(tsProfile));
    psProfile->psGetAccessoryJsonInfo = psGetAccessoryInfoJson;
    psProfile->psGetCharacteristicInfo = psGetCharacteristicInfo;
    psProfile->psAccessory = psAccessoryNew(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel, eType);
    return psProfile;
}

teHapStatus eProfileRelease(tsProfile *psProfile)
{
    eAccessoryRelease(psProfile->psAccessory);
    FREE(psProfile);
    return E_HAP_STATUS_OK;
}