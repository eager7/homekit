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
#include <profile.h>
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
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
uint64 u64UUID_SERVICES = 1;
uint64 u64UUID_ACCESSORY = 1;


/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

static teHapStatus eAccessoryInformationInit(tsAccessory *psAccessory, const char *psName, const char *psSerialNumber,
                                             const char *psManufacturer, const char *psModel)
{
    DBG_vPrintln(DBG_ACC, "eAccessoryInformationInit:%s", psName);
    tsService *psService = NULL;
    eAccessoryAddService(psAccessory, E_SERVICE_ACCESSORY_INFORMATION, UUID_SERVICE_CHARACTER, &psService);

    tsCharacteristic sCharaTemp;

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_IDENTIFY;
    sCharaTemp.eFormat = E_TYPE_BOOL;
    sCharaTemp.uValue.bData = T_TRUE;
    sCharaTemp.u8Perms = E_PERM_PAIRED_WRITE;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_MANUFACTURER;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = (char*)psManufacturer;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_MODEL;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = (char*)psModel;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_NAME;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = (char*)psName;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_SERIAL_NUMBER;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = (char*)psSerialNumber;
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    //Option
    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_FIRMWARE_VERSION;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = "1.0";
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

    memset(&sCharaTemp, 0, sizeof(tsCharacteristic));
    sCharaTemp.u64IID = UUID_SERVICE_CHARACTER;
    sCharaTemp.eType = E_CHARACTERISTIC_HARDWARE_VERSION;
    sCharaTemp.eFormat = E_TYPE_STRING;
    sCharaTemp.uValue.psData = "1.0";
    sCharaTemp.u8Perms = E_PERM_PAIRED_READ;
    eServiceAddCharacter(psService, sCharaTemp, NULL);

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
                                 const char *psManufacturer, const char *psModel, teAccessoryCategories eType)
{
    DBG_vPrintln(DBG_ACC, "New accessory:%s type:%d", psName, eType);
    tsAccessory *psAccessory = (tsAccessory*)calloc(1, sizeof(tsAccessory));
    CHECK_POINTER(psAccessory, NULL);
    psAccessory->u64AIDs = UUID_ACCESSORY;
    psAccessory->u64DeviceID = u64DeviceID;
    psAccessory->eAccessoryType = eType;
    eAccessoryInformationInit(psAccessory, psName, psSerialNumber, psManufacturer, psModel);
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

teHapStatus eAccessoryGetCharacter(const tsAccessory *psAccessory, uint64 u64AID, uint64 u64IID,
                                   tsCharacteristic **ppCharacter)
{
    for (int i = 0; i < psAccessory->u8NumServices; ++i) {
        for (int j = 0; j < psAccessory->psService[i].u8NumCharacteristics; ++j) {
            if(psAccessory->psService[i].psCharacteristics[j].u64IID == u64IID){
                *ppCharacter = &psAccessory->psService[i].psCharacteristics[j];
                return E_HAP_STATUS_OK;
            }
        }
    }
    return E_HAP_STATUS_ERROR;
}