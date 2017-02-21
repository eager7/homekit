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

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_HAP_OK,
    E_HAP_ERROR,
    E_HAP_MEMORY_ERROR,

} teHapStatus;

typedef enum {
    E_HAP_TYPE_UNKNOWN              = 0,
    E_HAP_TYPE_OTHER                = 1,
    E_HAP_TYPE_BRIDGE               = 2,
    E_HAP_TYPE_FAN                  = 3,
    E_HAP_TYPE_GARAGE_DOOR_OPENER   = 4,
    E_HAP_TYPE_LIGHT_BULB           = 5,
    E_HAP_TYPE_DOOR_LOOK            = 6,
    E_HAP_TYPE_OUTLET               = 7,
    E_HAP_TYPE_SWITCH               = 8,
    E_HAP_TYPE_THERMOSTAT           = 9,
    E_HAP_TYPE_SENSOR               = 10,
    E_HAP_TYPE_ALARM_SYSTEM         = 11,
    E_HAP_TYPE_DOOR                 = 12,
    E_HAP_TYPE_WINDOW               = 13,
    E_HAP_TYPE_WINDOW_COVERING      = 14,
    E_HAP_TYPE_PROGRAMMABLE_SWITCH  = 15,
    E_HAP_TYPE_IP_CAMERA            = 17,
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
    E_TYPE_BOOL,
    E_TYPE_INT,
    E_TYPE_FLOAT,
    E_TYPE_STRING,
    E_TYPE_DATE,
    E_TYPE_TLV8,
    E_TYPE_DATA,
    E_TYPE_ARRAY,
    E_TYPE_DICT,
    E_TYPE_NULL,
} teTypeFormat;

typedef enum {
    E_UNIT_NULL,        /* no this option */
    E_UNIT_CELSIUS,     /* The unit is only "degrees Celsius" */
    E_UNIT_PERCENTAGE,  /* The unit is in percentage "%" */
    E_UNIT_ARCDEGREES,  /* The unit is in arc degrees */
} teUnitType;

typedef enum {
    E_PERM_PAIRED_READ          = 0x04, /* This characteristic can only be read by paired controllers */
    E_PERM_PAIRED_WRITE         = 0x08, /* This characteristic can only be written by paired controllers */
    E_PERM_EVENT_NOTIFICATION   = 0x10, /* Boolean indicating if event notifications are enabled for this characteristic. */
    E_PERM_EVENT                = 0x20, /* This characteristic supports events. The HAP Characteristic object must contain the "ev" key if it supports events. */
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

typedef struct {
    char                    *psType;
    teCharacteristicUUID    eIID;     //Instance Id
    teTypeFormat            eFormat;
    tuCharValue             uValue;
    uint8                   u8Permissions;
    char                    *psDescription;
    teUnitType              eUnit;

    tsThreshold             sMinimumValue;
    tsThreshold             sMaximumValue;
    tsThreshold             sSetupValue;
    tsThreshold             sMaximumLen;
    tsThreshold             sMaximumDataLen;
} tsCharacteristic;

typedef enum {
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
    char               *psType;
    teServiceUUID      eIID; //Instance Id
    tsCharacteristic   *psCharacteristics;
} tsService;

typedef struct {
    char               *psType;
    teServiceUUID      eIID; //Instance Id
    tsCharacteristic   sCharacteristics[E_NUM_OF_SERVICE_ACCESSORY_INFORMATION];
} tsAccessoryInformation;

typedef struct{
    uint64                  u64AIDs;        //Accessory Instance Id
    teAccessoryType         eAccessoryType;
    tsAccessoryInformation  eInformation;
    tsService               sService;
} tsAccessory;
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
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_ACCESSORY_H_*/

