/****************************************************************************
 *
 * MODULE:             accessory.h
 *
 * COMPONENT:          homekit interface
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

#ifndef __H_ACCESSORY_H_
#define __H_ACCESSORY_H_

#if defined __cplusplus   
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "characteristic.h"
#include "service.h"
#include "accessory_type.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_ACCESSORY_OK = 0x00,
    E_ACCESSORY_ERROR = 0x01,
} teAcyStatus;

typedef struct {
    char auName[MIBF];
    char auSerialNumber[MIBF];
    char auManufacturer[MIBF];
    char auModel[MIBF];
} tsAccessoryInfo;

typedef struct {
    tsServices *psService;
    tsCharacteristics sName;
    tsCharacteristics sIdentify;
    tsCharacteristics sManufacturer;
    tsCharacteristics sModel;
    tsCharacteristics sSerialNumber;
} tsAccessoryInformation;

typedef struct{
    uint64 u64AIDs; //Accessory Instance Id
    tsServices *psServices;
    teAccessoryType eAccessoryType;
    tsAccessoryInformation sAccessoryInformation;
    int64 idCount;
    //func ;
} tsAccessroy;
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
teAcyStatus eCreateNewAccessory(char *psName, char *psSerialNumber, char *psManufacturer, char *psModel);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_ACCESSORY_H_*/

