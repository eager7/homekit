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
#include "accessory_type.h"
#include "mthread.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define MAX_SERVICES_NUM 100
#define MAX_CHARACTERISTICS_NUM 100
#define LEN_DEVICE_ID 17

#define UUID_SERVICE_CHARACTER (u64UUID_SERVICES++)
#define UUID_ACCESSORY (u64UUID_ACCESSORY++)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_TYPE_BOOL,    //JsonFormat:bool,Boolean value expressed as one of the following: true, false, 0 (false), and 1 (true)
    E_TYPE_UINT8,   //JsonFormat:uint8,Unsigned 8-bit integer.
    E_TYPE_UINT16,  //JsonFormat:uint16,Unsigned 32-bit integer.
    E_TYPE_UINT32,  //JsonFormat:uint32,Unsigned 32-bit integer.
    E_TYPE_UINT64,  //JsonFormat:uint64,Unsigned 64-bit integer
    E_TYPE_INT,     //JsonFormat:int,Signed 32-bit integer
    E_TYPE_FLOAT,   //JsonFormat:float,Signed 64-bit floating point number
    E_TYPE_STRING,  //JsonFormat:string,Base64-encoded set of one or more TLV8's
    E_TYPE_TLV8,    //JsonFormat:tlv8,Base64-encoded set of one or more TLV8's
    E_TYPE_DATA,    //JsonFormat:data,Base64-encoded data blob.
    E_TYPE_NULL,
} teTypeFormat;

typedef enum {
    E_UNIT_NULL,        /* no this option */
    E_UNIT_CELSIUS,     /* JsonFormat:celsius, The unit is only "degrees Celsius" */
    E_UNIT_PERCENTAGE,  /* JsonFormat:percentage, The unit is in percentage "%" */
    E_UNIT_ARC_DEGREES,  /* JsonFormat:arc degrees, The unit is in arc degrees */
    E_UNIT_LUX,         /* JsonFormat:lux, The unit is in lux. */
    E_UNIT_SECONDS,     /* JsonFormat:seconds, The unit is in seconds. */
} teUnitType;

typedef enum {
    E_PERM_PAIRED_READ          = 0x01, /* JsonFormat:pr, This characteristic can only be read by paired controllers */
    E_PERM_PAIRED_WRITE         = 0x02, /* JsonFormat:pw, This characteristic can only be written by paired controllers */
    E_PERM_EVENT_NOT            = 0x04, /* JsonFormat:ev, Boolean indicating if event notifications are enabled for this characteristic. */
    E_PERM_ADDITIONAL           = 0x08, /* JsonFormat:aa, This characteristic supports additional authorization data */
    E_PERM_TIMED_WRITE          = 0x10, /* JsonFormat:tw, This characteristic supports timed write procedure */
    E_PERM_HIDDEN               = 0x20, /* JsonFormat:hd, This characteristic is hidden from the user */
} tePermissions;

typedef union {
    bool_t  bData;
    int     iData;
    uint8   u8Data;
    uint16  u16Data;
    uint32  u32Data;
    uint64  u64Data;
    float   fData;
    char    *psData;
} tuChartValue;

typedef tuChartValue tuThreshold;

typedef struct {
    bool_t bEnable;
    tuThreshold uData;
} tsThreshold;

typedef struct {
    bool_t bEnable;
    tuChartValue uValue;
} tsChartValue;

typedef enum {
    E_NUM_OF_SERVICE_ACCESSORY_INFORMATION         = 7,
    E_NUM_OF_SERVICE_FAN                           = 3,
    E_NUM_OF_SERVICE_GARAGE_DOOR_OPENER            = 4,
    E_NUM_OF_SERVICE_LIGHT_BULB                    = 5,
    E_NUM_OF_SERVICE_LOCK_MAGAGEMENT               = 8,
    E_NUM_OF_SERVICE_LOCK_MECHANISM                = 2,
    E_NUM_OF_SERVICE_OUTLET                        = 3,
    E_NUM_OF_SERVICE_SWITCH                        = 2,
    E_NUM_OF_SERVICE_THERMOSTAT                    = 10,
} teCharsOfService;

typedef struct {
    teCharacteristicType    eType;              //JsonFormat:type
    uint64                  u64IID;             //JsonFormat:iid, Instance Id
    tuChartValue            uValue;             //JsonFormat:value
    uint8                   u8Perms;            //JsonFormat:perms, Need convert to string
    bool_t                  bEventNot;          //JsonFormat:ev
    char                    *psDescription;     //JsonFormat:description
    teTypeFormat            eFormat;            //JsonFormat:format, need convert to string
    teUnitType              eUnit;              //JsonFormat:unit, need convert to string

    tsThreshold             sMinimumValue;      //JsonFormat:minValue
    tsThreshold             sMaximumValue;      //JsonFormat:maxValue
    tsThreshold             sSetupValue;        //JsonFormat:minStep
    tsThreshold             sMaximumLen;        //JsonFormat:maxLen
    tsThreshold             sMaximumDataLen;    //JsonFormat:maxDataLen
    uint8                   *psValidValues;     //JsonFormat:valid-values
    uint8                   *psValidValuesRange;//JsonFormat:valid-values-range
} tsCharacteristic;

typedef struct _tsService{
    teServiceType      eType;                   //JsonFormat:type
    uint64             u64IID;                  //JsonFormat:iid
    uint8              u8NumCharacteristics;
    tsCharacteristic   *psCharacteristics;      //JsonFormat:characteristics
    bool_t             bHiddenServices;         //JsonFormat:hidden
    bool_t             bPrimaryServices;        //JsonFormat:primary
    uint8              u8NumLinkerServices;
    struct _tsService  *psLinkedServices;       //JsonFormat:linked
} tsService;

typedef struct{
    pthread_mutex_t         mutex;
    uint64                  u64AIDs;        //Accessory Instance Id, json-aid
    uint64                  u64DeviceID;    //The unique id of the accessory
    teAccessoryCategories   eAccessoryType;
    uint8                   u8NumServices;
    tsService               *psService;     //json-services
} tsAccessory;

typedef struct {
    uint8 u8NumAccessory;
    tsAccessory *psAccessory;
} tsAccessorySet;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern uint64 u64UUID_SERVICES;
extern uint64 u64UUID_ACCESSORY;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/*****************************************************************************
** Prototype    : psAccessoryGenerate
** Description  : create a new object for accessory device
** Input        : psName, the accessory's name
 *                u64DeviceID, the unique id of accessory, used by bonjour
 *                psSerialNumber, the serial number of accessory, distributed by manufacturer
 *                psManufacturer, the factory of making accessory, such as Topband
 *                psModel, the model of accessory, such as X7
 *                eType, the type defined in teAccessoryType
** Output       : None
** Return Value : if success, return a struct of tsAccessory, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsAccessory *psAccessoryGenerate(const char *psName, uint64 u64DeviceID, const char *psSerialNumber,
                                 const char *psManufacturer, const char *psModel, teAccessoryCategories eType);
/*****************************************************************************
** Prototype    : eAccessoryRelease
** Description  : free the memory allocated by psAccessoryNew
** Input        : psAccessory, the object allocated by psAccessoryGenerate
** Output       : None
** Return Value : always return E_HAP_STATUS_OK

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eAccessoryRelease(tsAccessory *psAccessory);
/*****************************************************************************
** Prototype    : eServiceAddCharacter
** Description  : add a new character into a service
** Input        : psService, the service will be operated
 *                sCharaIn, the character will be inserted
** Output       : ppCharaOut, the character been inserted
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR
 * if the character existed, return E_HAP_STATUS_DUPLICATE

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eServiceAddCharacter(tsService *psService, tsCharacteristic sCharaIn, tsCharacteristic **ppCharaOut);
/*****************************************************************************
** Prototype    : eAccessoryAddService
** Description  : add a new service into a accessory
** Input        : psAccessory, the accessory will be operated
 *                eType, the type of service
 *                u64IID, the uuid of service
** Output       : ppsService, the service been inserted
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR
 * if the character existed, return E_HAP_STATUS_DUPLICATE

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eAccessoryAddService(tsAccessory *psAccessory, teServiceType eType, uint64 u64IID, tsService **ppsService);
/*****************************************************************************
** Prototype    : psAccessoryGetCharacterByIID
** Description  : search a character by iid
** Input        : psAccessory, the accessory will be operated
 *                u64AID, the uuid of accessory
 *                u64IID, the uuid of service
** Output       : none
** Return Value : if success, return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsCharacteristic * psAccessoryGetCharacterByIID(const tsAccessory *psAccessory, uint64 u64AID, uint64 u64IID);
/*****************************************************************************
** Prototype    : psAccessoryGetCharacterByType
** Description  : search a character by type
** Input        : psAccessory, the accessory will be operated
 *                u64AID, the uuid of accessory
 *                u64IID, the uuid of service
** Output       : none
** Return Value : if success, return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsCharacteristic *psAccessoryGetCharacterByType(const tsAccessory *psAccessory, uint64 u64AID, teCharacteristicType eType);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_ACCESSORY_H_*/

