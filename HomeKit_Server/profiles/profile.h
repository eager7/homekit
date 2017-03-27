/****************************************************************************
 *
 * MODULE:             profile.h
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

#ifndef HOMEKIT_PROFILE_H
#define HOMEKIT_PROFILE_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "accessory.h"
#include "list.h"
#include "controller.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef teHapStatus (*fpeInitCategory)(tsAccessory *psAccessory);
typedef teHapStatus (*feHandleSetCmd)(tsCharacteristic *psCharacter, json_object *psJson);
typedef teHapStatus (*feHandleGetCmd)(tsCharacteristic *psCharacter);
typedef teHapStatus (*feIdentify)();

typedef struct {
    tsAccessory                 *psAccessory;
    fpeInitCategory             peInitCategory;
    feHandleSetCmd              eHandleSetCmd;
    feHandleGetCmd              eHandleGetCmd;
    feIdentify                  eIdentify;
} tsProfile;
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
/***        Exported Functions                                            ***/
/****************************************************************************/
/*****************************************************************************
** Prototype    : psProfileGenerate
** Description  : malloc a new profile object
** Input        :
** Output       : none
** Return Value : if success, return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsProfile *psProfileGenerate(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel,
                             teAccessoryCategories eType, fpeInitCategory fsInitCategory, feHandleSetCmd eHandleSetCmd,
                             feHandleGetCmd eHandleGetCmd, feIdentify eIdentify);
teHapStatus eProfileRelease(tsProfile *psProfile);
/*****************************************************************************
** Prototype    : psGetAccessoryInfo
** Description  : format the accessory info into a json object
** Input        : psAccessory, the object of accessory
** Output       : none
** Return Value : if success, return the json object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
json_object *psGetAccessoryInfo(const tsAccessory *psAccessory);
/*****************************************************************************
** Prototype    : psGetCharacteristicInfo
** Description  : format the character info into a json object
** Input        : psAccessory, the object of accessory
 *                psCmd, http data, such as: id=1.14,1.11,1.10,1.15
 *                fCallback, the usr define function, will read the device
** Output       : none
** Return Value : if success, return the json object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
json_object *psGetCharacteristicInfo(const tsAccessory *psAccessory, const char *psCmd, feHandleGetCmd fCallback);
/*****************************************************************************
** Prototype    : eSetCharacteristicInfo
** Description  : 设置配件的属性，如果此属性支持上报，将会触发上报线程
** Input        : psAccessory, the object of accessory
 *                psController, the IOS device's structure
 *                psCmd, http data, such as: {"characteristics":[{"aid":1,"iid":10,"value":0}]}
 *                fCallback, the usr define function, will operator the device
** Output       : ppsBuffer, the result of content character's value
 *                pu16Len, the result len
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eSetCharacteristicInfo(tsAccessory *psAccessory, tsController *psController, const uint8 *psCmd, uint8 **ppsBuffer, uint16 *pu16Len, feHandleSetCmd fCallback);
teHapStatus eNotifyEnQueue(tsCharacteristic *psCharacter);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PROFILE_H
