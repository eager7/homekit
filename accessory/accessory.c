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

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static uint64 u64UUID_SERVICES = 1;
static uint64 u64UUID_ACCESSORY = 1;
#define UUID_SER_CHAR (u64UUID_SERVICES++)
#define UUID_ACCESSORY (u64UUID_ACCESSORY++)

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, char *psName, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    DBG_vPrintln(DBG_ACC, "eAccessoryInformationInit:%s", psName);
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, E_SERVICE_ACCESSORY_INFORMATION, UUID_SER_CHAR, &psService);

    tsCharacteristic asCharaTemp[7];
    memset(asCharaTemp, 0, sizeof(tsCharacteristic)*7);

    asCharaTemp[0].u64IID = UUID_SER_CHAR;
    asCharaTemp[0].eType = E_CHARACTERISTIC_IDENTIFY;
    asCharaTemp[0].eFormat = E_TYPE_BOOL;
    asCharaTemp[0].uValue.bData = T_TRUE;
    asCharaTemp[0].u8Perms = E_PERM_PAIRED_WRITE;
    eServiceAddCharacter(psService, asCharaTemp[0], NULL);

    asCharaTemp[1].u64IID = UUID_SER_CHAR;
    asCharaTemp[1].eType = E_CHARACTERISTIC_MANUFACTURER;
    asCharaTemp[1].eFormat = E_TYPE_STRING;
    asCharaTemp[1].uValue.psData = "ET";
    asCharaTemp[1].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[1], NULL);

    asCharaTemp[2].u64IID = UUID_SER_CHAR;
    asCharaTemp[2].eType = E_CHARACTERISTIC_MODEL;
    asCharaTemp[2].eFormat = E_TYPE_STRING;
    asCharaTemp[2].uValue.psData = psModel;
    asCharaTemp[2].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[2], NULL);

    asCharaTemp[3].u64IID = UUID_SER_CHAR;
    asCharaTemp[3].eType = E_CHARACTERISTIC_NAME;
    asCharaTemp[3].eFormat = E_TYPE_STRING;
    asCharaTemp[3].uValue.psData = "Light 1";
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);

    asCharaTemp[4].u64IID = UUID_SER_CHAR;
    asCharaTemp[4].eType = E_CHARACTERISTIC_SERIAL_NUMBER;
    asCharaTemp[4].eFormat = E_TYPE_STRING;
    asCharaTemp[4].uValue.psData = psSerialNumber;
    asCharaTemp[4].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[4], NULL);
#if 0
    //Option
    asCharaTemp[5].u64IID = UUID_SER_CHAR;
    asCharaTemp[5].eType = E_CHARACTERISTIC_FIRMWARE_VERSION;
    asCharaTemp[5].eFormat = E_TYPE_STRING;
    asCharaTemp[5].uValue.psData = "1.0";
    asCharaTemp[5].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[5], NULL);

    asCharaTemp[6].u64IID = UUID_SER_CHAR;
    asCharaTemp[6].eType = E_CHARACTERISTIC_HARDWARE_VERSION;
    asCharaTemp[6].eFormat = E_TYPE_STRING;
    asCharaTemp[6].uValue.psData = "1.0";
    asCharaTemp[6].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[6], NULL);
#endif
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
    eAccessoryAddService(psAccessory, E_SERVICE_LIGHT_BULB, UUID_SER_CHAR, &psService);

    tsCharacteristic asCharaTemp[E_NUM_OF_SERVICE_LIGHT_BULB];
    memset(asCharaTemp, 0, sizeof(tsCharacteristic)*E_NUM_OF_SERVICE_LIGHT_BULB);

    asCharaTemp[0].u64IID = UUID_SER_CHAR;
    asCharaTemp[0].eType = E_CHARACTERISTIC_ON;
    asCharaTemp[0].eFormat = E_TYPE_BOOL;
    asCharaTemp[0].uValue.bData = T_TRUE;
    asCharaTemp[0].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[0], NULL);

    asCharaTemp[1].u64IID = UUID_SER_CHAR;
    asCharaTemp[1].eType = E_CHARACTERISTIC_BRIGHTNESS;
    asCharaTemp[1].eFormat = E_TYPE_INT;
    asCharaTemp[1].uValue.iData = 50;
    asCharaTemp[1].sSetupValue.bEnable = T_TRUE;
    asCharaTemp[1].sSetupValue.uData.iValue = 1;
    asCharaTemp[1].eUnit = E_UNIT_PERCENTAGE;
    asCharaTemp[1].sMinimumValue.bEnable = T_TRUE;
    asCharaTemp[1].sMinimumValue.uData.iValue = 0;
    asCharaTemp[1].sMaximumValue.bEnable = T_TRUE;
    asCharaTemp[1].sMaximumValue.uData.iValue = 100;
    asCharaTemp[1].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[1], NULL);

    asCharaTemp[3].u64IID = UUID_SER_CHAR;
    asCharaTemp[3].eType = E_CHARACTERISTIC_NAME;
    asCharaTemp[3].eFormat = E_TYPE_STRING;
    asCharaTemp[3].uValue.psData = "light";
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);
#if 0
    asCharaTemp[2].u64IID = UUID_SER_CHAR;
    asCharaTemp[2].eType = E_CHARACTERISTIC_HUE;
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

    asCharaTemp[4].u64IID = UUID_SER_CHAR;
    asCharaTemp[4].eType = E_CHARACTERISTIC_SATURATION;
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
#endif
    return E_HAP_STATUS_OK;
}

static char* trim(const char* str_buf, size_t len)
{
    if(str_buf == NULL || len <= 0){
        ERR_vPrintln(T_TRUE, "Parameter Error");
        return NULL;
    }
    bool_t bDone = T_FALSE;
    char *str_new = NULL;
    size_t len_new = 0;
    for (int i = 0; i < len; ++i) {
        if(str_buf[i] == 0x20){
            continue;
        }
        len_new ++;
        str_new = realloc(str_new, len_new+1);
        str_new[len_new-1] = str_buf[i];
        bDone = T_TRUE;
    }
    if(bDone)
        str_new[len_new] = '\0';
    return str_new;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsAccessory *psAccessoryNew(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel,
                            teAccessoryType eType)
{
    DBG_vPrintln(DBG_ACC, "New accessory:%s type:%d", psName, eType);
    tsAccessory *psAccessory = (tsAccessory*)malloc(sizeof(tsAccessory));
    CHECK_POINTER(psAccessory, NULL);
    memset(psAccessory, 0, sizeof(tsAccessory));
    psAccessory->u64AIDs = UUID_ACCESSORY;
    psAccessory->u64DeviceID = u64DeviceID;
    psAccessory->eAccessoryType = eType;
    eAccessoryInformationInit(psAccessory, psName, psSerialNumber, psManufacturer, psModel);
    eAccessoryCategoriesInit(psAccessory, eType);
    return psAccessory;
}

teHapStatus eAccessoryRelease(tsAccessory *psAccessory)
{
    WAR_vPrintln(DBG_ACC, "Finished accessory type:%d", psAccessory->eAccessoryType);
    FREE(psAccessory->psService->psCharacteristics);
    FREE(psAccessory->psService);
    FREE(psAccessory);
    return E_HAP_STATUS_OK;
}

json_object *psGetAccessoryInfoJson(const tsAccessory *psAccessory)
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

teHapStatus eAccessoryAddService(tsAccessory *psAccessory, teServiceType eType, uint64 u64IID, tsService **ppsService)
{
    tsService *psService;
    for (int i = 0; i < psAccessory->u8NumServices; ++i) {
        if(psAccessory->psService[i].eType == eType){
            INF_vPrintln(DBG_ACC, "Duplicate Service:0x%16x", eType);
            if(ppsService) *ppsService = &psAccessory->psService[i];
            return E_HAP_STATUS_DUPLICATE;
        }
    }
    DBG_vPrintln(DBG_ACC, "Create New Service[%d]:0x%016x", psAccessory->u8NumServices + 1, eType);
    psService = realloc(psAccessory->psService, sizeof(tsService)*(psAccessory->u8NumServices + 1));
    CHECK_POINTER(psService, E_HAP_STATUS_ERROR);
    psAccessory->psService = psService;

    memset(&psAccessory->psService[psAccessory->u8NumServices], 0, sizeof(tsService));
    psService = &psAccessory->psService[psAccessory->u8NumServices];
    psAccessory->u8NumServices ++;

    psService->u64IID = u64IID;
    psService->eType = eType;
    if(ppsService) *ppsService = psService;
    return E_HAP_STATUS_OK;
}

teHapStatus eServiceAddCharacter(tsService *psService, tsCharacteristic sCharaIn, tsCharacteristic **ppCharaOut)
{
    tsCharacteristic *psChara;
    for (int i = 0; i < psService->u8NumCharacteristics; ++i) {
        if(psService->psCharacteristics[i].eType == sCharaIn.eType){
            INF_vPrintln(DBG_ACC, "Duplicate Characteristics:0x%16x", sCharaIn.eType);
            if(ppCharaOut) *ppCharaOut = &psService->psCharacteristics[i];
            return E_HAP_STATUS_DUPLICATE;
        }
    }
    DBG_vPrintln(DBG_ACC, "Create New Characteristics[%d]:0x%016x", psService->u8NumCharacteristics + 1, sCharaIn.eType);
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

json_object *psGetCharacteristicInfo(const tsAccessory *psAccessory, const char *psCmd)
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

tsAccessory *psGetAccessoryInstance(uint64 u64AID)
{

}