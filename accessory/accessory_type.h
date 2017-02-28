/****************************************************************************
 *
 * MODULE:             accessory_type.h
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

#ifndef __H_ACCESSORY_TYPE_H_
#define __H_ACCESSORY_TYPE_H_

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
    E_HAP_STATUS_OK,
    E_HAP_STATUS_DUPLICATE,
    E_HAP_STATUS_ERROR,
    E_HAP_STATUS_MEMORY_ERROR,
    E_HAP_STATUS_SOCKET_ERROR,
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
} teServiceType;   /* Type for IP */
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
} teCharacteristicType;

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
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_ACCESSORY_TYPE_H_*/


