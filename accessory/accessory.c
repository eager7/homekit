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
static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, const char *psName, const char *psSerialNumber,
                                             const char *psManufacturer, const char *psModel);
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
static teHapStatus eAccessoryAddService(tsAccessory *psAccessory, teServiceType eType, uint64 u64IID, tsService **ppsService)
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

static teHapStatus eServiceAddCharacter(tsService *psService, tsCharacteristic sCharaIn, tsCharacteristic **ppCharaOut)
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

static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, const char *psName, const char *psSerialNumber,
                                             const char *psManufacturer, const char *psModel)
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
    asCharaTemp[1].uValue.psData = (char*)psManufacturer;
    asCharaTemp[1].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[1], NULL);

    asCharaTemp[2].u64IID = UUID_SER_CHAR;
    asCharaTemp[2].eType = E_CHARACTERISTIC_MODEL;
    asCharaTemp[2].eFormat = E_TYPE_STRING;
    asCharaTemp[2].uValue.psData = (char*)psModel;
    asCharaTemp[2].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[2], NULL);

    asCharaTemp[3].u64IID = UUID_SER_CHAR;
    asCharaTemp[3].eType = E_CHARACTERISTIC_NAME;
    asCharaTemp[3].eFormat = E_TYPE_STRING;
    asCharaTemp[3].uValue.psData = (char*)psName;
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);

    asCharaTemp[4].u64IID = UUID_SER_CHAR;
    asCharaTemp[4].eType = E_CHARACTERISTIC_SERIAL_NUMBER;
    asCharaTemp[4].eFormat = E_TYPE_STRING;
    asCharaTemp[4].uValue.psData = (char*)psSerialNumber;
    asCharaTemp[4].u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, asCharaTemp[4], NULL);

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

    asCharaTemp[3].u64IID = UUID_SER_CHAR;
    asCharaTemp[3].eType = E_CHARACTERISTIC_SATURATION;
    asCharaTemp[3].eFormat = E_TYPE_FLOAT;
    asCharaTemp[3].uValue.fData = 100;
    asCharaTemp[3].eUnit = E_UNIT_PERCENTAGE;
    asCharaTemp[3].sSetupValue.bEnable = T_TRUE;
    asCharaTemp[3].sSetupValue.uData.fValue = 1;
    asCharaTemp[3].sMinimumValue.bEnable = T_TRUE;
    asCharaTemp[3].sMinimumValue.uData.fValue = 0;
    asCharaTemp[3].sMaximumValue.bEnable = T_TRUE;
    asCharaTemp[3].sMaximumValue.uData.fValue = 100;
    asCharaTemp[3].u8Perms = E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOT;
    eServiceAddCharacter(psService, asCharaTemp[3], NULL);

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
tsAccessory *psAccessoryGenerate(const char *psName, uint64 u64DeviceID, const char *psSerialNumber,
                                 const char *psManufacturer, const char *psModel, teAccessoryType eType)
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



