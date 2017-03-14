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
static tsQueue sQueue;
static tsThread sThread;
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

static teHapStatus eHandleSetCmd(tsCharacteristic *psCharacter, json_object *psJson, tsController *psSocket)
{
    if(psCharacter->eType == E_CHARACTERISTIC_TARGET_POSITION){
        NOT_vPrintln(DBG_WINDOW_COVER, "Set Window Position:%d", json_object_get_int(psJson));
        //tsQueueData *psQueueData = (tsQueueData*)calloc(1, sizeof(tsQueueData));
        //memcpy(&psQueueData->sCharacter, psCharacter, sizeof(tsCharacteristic));
        //psQueueData->sCharacter.uValue.u8Data = (uint8)json_object_get_int(psJson);
        //memcpy(&psQueueData->sController, psSocket, sizeof(tsController));
        //eQueueEnqueue(&sQueue, psQueueData);
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
static void *pvWindowCoveringThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    psThreadInfo->eState = E_THREAD_RUNNING;

    while(psThreadInfo->eState == E_THREAD_RUNNING){
        tsQueueData *psQueueData = NULL;
        eQueueDequeue(&sQueue, (void**)&psQueueData);
        if(psQueueData->sCharacter.eType == E_CHARACTERISTIC_TARGET_POSITION){
            WAR_vPrintln(1, "E_CHARACTERISTIC_TARGET_POSITION");
#if 0
            sleep(5);
            json_object *psJsonResp = json_object_new_object();
            json_object *psJsonCharacter = json_object_new_object();
            json_object *psArrayCharacter = json_object_new_array();
            json_object_object_add(psJsonCharacter, "aid", json_object_new_int(1));
            json_object_object_add(psJsonCharacter, "iid", json_object_new_int64((int64_t)psQueueData->sCharacter.u64IID));
            json_object_object_add(psJsonCharacter, "value", json_object_new_int(psQueueData->sCharacter.uValue.u8Data));
            json_object_array_add(psArrayCharacter, psJsonCharacter);
            json_object_object_add(psJsonResp, "characteristics", psArrayCharacter);
            uint8 *psBuffer = NULL;
            uint16 u16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_EVENT, HTTP_TYPE_JSON,
                                          (uint8 *) json_object_get_string(psJsonResp),
                                          (uint16) strlen(json_object_get_string(psJsonResp)), &psBuffer);


            WAR_vPrintln(1, "%d\n%s", u16Len, psBuffer);
            uint8 auHttpData[MMBF] = {0};
            uint16 u16SendLen = 0;
            memset(auHttpData, 0, sizeof(auHttpData));
            eEncryptedMessageWithLen(psBuffer, u16Len, &psQueueData->sController, auHttpData, &u16SendLen);
            FREE(psBuffer);
            send(psQueueData->sController.iSocketFd, auHttpData, u16SendLen, 0);
            psQueueData->sController.u64NumberSend++;
            FREE(psQueueData);
#endif
        }
    }
    DBG_vPrintln(DBG_WINDOW_COVER, "pvWindowCoveringThreadHandle Exit");
    vThreadFinish(psThreadInfo);
    return NULL;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tsProfile *psWindowCoveringProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileGenerate(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                             E_HAP_TYPE_WINDOW_COVERING, eAccessoryWindowCoveringInit, eHandleSetCmd,
                                             eHandleGetCmd);
    eQueueCreate(&sQueue, 5);
    eThreadStart(pvWindowCoveringThreadHandle, &sThread, E_THREAD_DETACHED);
    return psProfile;
}

teHapStatus eWindowCoveringProfileRelease(tsProfile *psProfile)
{
    eThreadStop(&sThread);
    eQueueDestroy(&sQueue);
    eProfileRelease(psProfile);
    return E_HAP_STATUS_OK;
}
