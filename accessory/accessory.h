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
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define MAX_SERVICES_NUM 100
#define MAX_CHARACTERISTICS_NUM 100
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_HAP_STATUS_OK,
    E_HAP_STATUS_DUPLICATE,
    E_HAP_STATUS_ERROR,
    E_HAP_STATUS_MEMORY_ERROR,
} teHapStatus;

typedef enum {
    E_HAP_TYPE_UNKNOWN              = 0,
    E_HAP_TYPE_OTHER                = 1,
    E_HAP_TYPE_BRIDGE               = 2,
    E_HAP_TYPE_FAN                  = 3,
    E_HAP_TYPE_GARAGE               = 4,
    E_HAP_TYPE_LIGHT_BULB           = 5,
    E_HAP_TYPE_DOOR_LOOK            = 6,
    E_HAP_TYPE_OUTLET               = 7,
    E_HAP_TYPE_SWITCH               = 8,
    E_HAP_TYPE_THERMOSTAT           = 9,
    E_HAP_TYPE_SENSOR               = 10,
    E_HAP_TYPE_SECURITY_SYSTEM      = 11,
    E_HAP_TYPE_DOOR                 = 12,
    E_HAP_TYPE_WINDOW               = 13,
    E_HAP_TYPE_WINDOW_COVERING      = 14,
    E_HAP_TYPE_PROGRAMMABLE_SWITCH  = 15,
    E_HAP_TYPE_IP_CAMERA            = 17,
    E_HAP_TYPE_VIDEO_DOOR_BEEL      = 18,
    E_HAP_TYPE_AIR_PURIFIER         = 19,
    E_HAP_TYPE_AIR_HEATER           = 20,
    E_HAP_TYPE_AIR_CONDITIONER      = 21,
    E_HAP_TYPE_AIR_HUMIDIFIER       = 22,
    E_HAP_TYPE_AIR_DEHUMIDIFIER     = 23,
    E_HAP_TYPE_AIR_RESERVED         = 24,
} teAccessoryType;

typedef enum {
    E_CHARACTERISTIC_ADMINNISTRATOR_ONLY_ACCESS             = 0x00000001,
    E_CHARACTERISTIC_AUDIO_FEEDBACK                         = 0x00000005,
    E_CHARACTERISTIC_BRIGHTNESS                             = 0x00000008,
    E_CHARACTERISTIC_COOLING_THRESHOLD_TEMPERATURE          = 0x0000000D,
    E_CHARACTERISTIC_CURRENT_DOOR_STATE                     = 0x0000000E,
    E_CHARACTERISTIC_CURRENT_HEATING_COOLING_STATE          = 0x0000000F,
    E_CHARACTERISTIC_CURRENT_RELATIVE_HUMIDITY              = 0x00000010,
    E_CHARACTERISTIC_CURRENT_TEMPERATURE                    = 0x00000011,
    E_CHARACTERISTIC_HEATING_THRESHOLD_TEMPERATURE          = 0x00000012,
    E_CHARACTERISTIC_HUE                                    = 0x00000013,
    E_CHARACTERISTIC_IDENTIFY                               = 0x00000014,
    E_CHARACTERISTIC_LOCK_CONTROL_POINT                     = 0x00000015,
    E_CHARACTERISTIC_LOCK_MANAGEMENT_AUTO_SECURITY_TIMEOUT  = 0x0000001A,
    E_CHARACTERISTIC_LOCK_LAST_KNOWN_ACTION                 = 0x0000001C,
    E_CHARACTERISTIC_LOCK_CURRENT_STATE                     = 0x0000001D,
    E_CHARACTERISTIC_LOCK_TARGET_STATE                      = 0x0000001E,
    E_CHARACTERISTIC_LOGS                                   = 0x0000001F,
    E_CHARACTERISTIC_MANUFACTURER                           = 0x00000020,
    E_CHARACTERISTIC_MODEL                                  = 0x00000021,
    E_CHARACTERISTIC_MOTION_DETECTED                        = 0x00000022,
    E_CHARACTERISTIC_NAME                                   = 0x00000023,
    E_CHARACTERISTIC_OBSTRUCTION_DETECTED                   = 0x00000024,
    E_CHARACTERISTIC_ON                                     = 0x00000025,
    E_CHARACTERISTIC_OUTLET_IN_USE                          = 0x00000026,
    E_CHARACTERISTIC_ROTATION_DIRECTION                     = 0x00000028,
    E_CHARACTERISTIC_ROTATION_SPEED                         = 0x00000029,
    E_CHARACTERISTIC_SATURATION                             = 0x0000002F,
    E_CHARACTERISTIC_SERIAL_NUMBER                          = 0x00000030,
    E_CHARACTERISTIC_TARGET_DOOR_STATE                      = 0x00000032,
    E_CHARACTERISTIC_TARGET_HEATING_COOLING_STATE           = 0x00000033,
    E_CHARACTERISTIC_TARGET_RELATIVE_HUMIDITY               = 0x00000034,
    E_CHARACTERISTIC_TARGET_TEMPERATURE                     = 0x00000035,
    E_CHARACTERISTIC_TEMPERATURE_DISPLAY_UNITS              = 0x00000036,
    E_CHARACTERISTIC_VERSION                                = 0x00000037,
    E_CHARACTERISTIC_FIRMWARE_VERSION                       = 0x00000052,
    E_CHARACTERISTIC_HARDWARE_VERSION                       = 0x00000053,
    E_CHARACTERISTIC_SOFTWART_VERSION                       = 0x00000054,
} teCharacteristicUUID;

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
    E_UNIT_ARCDEGREES,  /* JsonFormat:arcdegrees, The unit is in arc degrees */
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
    float   fData;
    char    *psData;
} tuCharValue;

typedef union {
    int     iValue;
    float   fValue;
} tuThreshold;

typedef struct {
    bool_t bEnable;
    tuThreshold uData;
} tsThreshold;

typedef enum {
    E_SERVICE_ACCESSORY_OBJECT              = 0x00000001,
    E_SERVICE_ACCESSORY_INFORMATION         = 0x0000003E,
    E_SERVICE_FAN                           = 0x00000040,
    E_SERVICE_GARAGE_DOOR_OPENER            = 0x00000041,
    E_SERVICE_LIGHT_BULB                    = 0x00000043,
    E_SERVICE_LOCK_MAGAGEMENT               = 0x00000044,
    E_SERVICE_LOCK_MECHANISM                = 0x00000045,
    E_SERVICE_OUTLET                        = 0x00000047,
    E_SERVICE_SWITCH                        = 0x00000049,
    E_SERVICE_THERMOSTAT                    = 0x0000004A,
} teServiceUUID;   /* UUID for IP */

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
    char                    *psType;            //JsonFormat:type
    teCharacteristicUUID    eIID;               //JsonFormat:iid, Instance Id
    tuCharValue             uValue;             //JsonFormat:value
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
    char               *psType;                 //JsonFormat:type
    teServiceUUID      eIID;                    //JsonFormat:iid
    uint8              u8NumCharacteristics;
    tsCharacteristic   *psCharacteristics;      //JsonFormat:characteristics
    bool_t             bHiddenServices;         //JsonFormat:hidden
    bool_t             bPrimaryServices;        //JsonFormat:primary
    uint8              u8NumLinkerServices;
    struct _tsService  *psLinkedServices;       //JsonFormat:linked
} tsService;

typedef struct{
    uint64                  u64AIDs;        //Accessory Instance Id, json-aid
    teAccessoryType         eAccessoryType;
    //tsAccessoryInformation  eInformation;
    uint8                   u8NumServices;
    tsService               *psService;     //json-services
} tsAccessory;

typedef struct {
    char               *psType;
    teServiceUUID      eIID; //Instance Id
    tsCharacteristic   sCharacteristics[E_NUM_OF_SERVICE_ACCESSORY_INFORMATION];
} tsAccessoryInformation;

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
teHapStatus eAccessoryInit(tsAccessory *psAccessory, char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel, teAccessoryType eType);
teHapStatus eAccessoryFinished(tsAccessory *psAccessory);
json_object* psGetAccessoryInfoJson(tsAccessory *psAccessory);
teHapStatus eAccessoryAddService(tsAccessory *psAccessory, char *psType, teServiceUUID eIID, tsService **ppsService);
teHapStatus eServiceAddCharacter(tsService *psService, tsCharacteristic sCharaIn, tsCharacteristic **ppCharaOut);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_ACCESSORY_H_*/

