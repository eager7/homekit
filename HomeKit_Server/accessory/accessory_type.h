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
#define UUID_SUFFIX "-0000-1000-8000-0026BB765291"
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_HAP_STATUS_OK,
    E_HAP_STATUS_DUPLICATE,
    E_HAP_STATUS_ERROR,
    E_HAP_STATUS_MAX_NUM_ERROR,
    E_HAP_STATUS_MEMORY_ERROR,
    E_HAP_STATUS_SOCKET_ERROR,
    E_HAP_STATUS_VERIFY_ERROR,
} teHapStatus;

typedef enum {
    E_HAP_STATUS_CODE_SUCCESS                  = 0,
    E_HAP_STATUS_CODE_REQUEST_DENIED           = -70401,
    E_HAP_STATUS_CODE_UNABLE_COMMUNICATION     = -70402,
    E_HAP_STATUS_CODE_RESOURCE_BUSY            = -70403,
    E_HAP_STATUS_CODE_READ_ONLY                = -70404,
    E_HAP_STATUS_CODE_WRITE_ONLY               = -70405,
    E_HAP_STATUS_CODE_NOT_NOTIFICATION         = -70406,
    E_HAP_STATUS_CODE_OUT_OF_RESOURCE          = -70407,
    E_HAP_STATUS_CODE_TIMED_OUT                = -70408,
    E_HAP_STATUS_CODE_NOT_EXIST                = -70409,
    E_HAP_STATUS_CODE_INVALID_VALUE_TO_WRITE   = -70410,
    E_HAP_STATUS_CODE_INSUFFICIENT_AUTHORIZATION = -70411,
} teHapCode;  /* json:status */

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
} teAccessoryCategories;

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
    E_SERVICE_SECURITY_SYSTEM               = 0x0000007E,
    E_SERVICE_CARBON_MONOXIDE_SENSOR        = 0x0000007F,
    E_SERVICE_CONTACT_SENSOR                = 0x00000080,
    E_SERVICE_DOOR                          = 0x00000081,
    E_SERVICE_HUMIDITY_SENSOR               = 0x00000082,
    E_SERVICE_LEAK_SENSOR                   = 0x00000083,
    E_SERVICE_LIGHT_SENSOR                  = 0x00000084,
    E_SERVICE_MOTION_SENSOR                 = 0x00000085,
    E_SERVICE_OCCUPANCY_SENSOR              = 0x00000086,
    E_SERVICE_SMOKE_SENSOR                  = 0x00000087,
    E_SERVICE_STATEFUL_PROGRAMMABLE_SWITCH  = 0x00000088,
    E_SERVICE_STATELESS_PROGRAMMABLE_SWITCH = 0x00000089,
    E_SERVICE_TEMPERATURE_SENSOR            = 0x0000008A,
    E_SERVICE_WINDOW                        = 0x0000008B,
    E_SERVICE_WINDOW_COVERING               = 0x0000008C,
    E_SERVICE_BATTERY_SERVICE               = 0x00000096,
    E_SERVICE_CARBON_DIOXIDE_SENSOR         = 0x00000097,
    E_SERVICE_CAMERA_RTP_STREAM_MANAGEMENT  = 0x00000110,
    E_SERVICE_MICROPHONE                    = 0x00000112,
    E_SERVICE_SPEAKER                       = 0x00000113,
    E_SERVICE_DOORBELL                      = 0x00000121,
    E_SERVICE_FAN_V2                        = 0x000000B7,
    E_SERVICE_SLAT                          = 0x000000B9,
    E_SERVICE_FILTER_MAINTENANCE            = 0x000000BA,
    E_SERVICE_AIR_PURIFIER                  = 0x000000BB,
    E_SERVICE_HEATER_COOLER                 = 0x000000BC,
    E_SERVICE_HUMIDIFIER_DEHUMIDIFIER       = 0x000000BD,
    E_SERVICE_SERVICE_LABEL                 = 0x000000CC,
} teServiceType;   /* Type for IP */

typedef enum {
    E_CHARACTERISTIC_ADMINISTRATOR_ONLY_ACCESS              = 0x00000001,
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
    E_CHARACTERISTIC_LOCK_CONTROL_POINT                     = 0x00000019,
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
    E_CHARACTERISTIC_SOFTWARE_VERSION                       = 0x00000054,
    E_CHARACTERISTIC_AIR_PARTICULATE_DENSITY                = 0x00000064,
    E_CHARACTERISTIC_AIR_PARTICULATE_SIZE                   = 0x00000065,
    E_CHARACTERISTIC_SECURITY_SYSTEM_CURRENT_STATE          = 0x00000066,
    E_CHARACTERISTIC_SECURITY_SYSTEM_TARGET_STATE           = 0x00000067,
    E_CHARACTERISTIC_BATTERY_LEVEL                          = 0x00000068,
    E_CHARACTERISTIC_CARBON_MONOXIDE_DETECTED               = 0x00000069,
    E_CHARACTERISTIC_CONTACT_SENSOR_STATE                   = 0x0000006A,
    E_CHARACTERISTIC_CURRENT_AMBIENT_LIGHT_LEVEL            = 0x0000006B,
    E_CHARACTERISTIC_CURRENT_HORIZONTAL_TILT_ANGLE          = 0x0000006C,
    E_CHARACTERISTIC_CURRENT_POSITION                       = 0x0000006D,
    E_CHARACTERISTIC_CURRENT_VERTICAL_TILT_ANGLE            = 0x0000006E,
    E_CHARACTERISTIC_HOLD_POSITION                          = 0x0000006F,
    E_CHARACTERISTIC_LEAK_DETECTED                          = 0x00000070,
    E_CHARACTERISTIC_OCCUPANCY_DETECTED                     = 0x00000071,
    E_CHARACTERISTIC_POSITION_STATE                         = 0x00000072,
    E_CHARACTERISTIC_PROGRAMMABLE_SWITCH_EVENT              = 0x00000073,
    E_CHARACTERISTIC_PROGRAMMABLE_SWITCH_OUTPUT_STATE       = 0x00000074,
    E_CHARACTERISTIC_STATUS_ACTIVE                          = 0x00000075,
    E_CHARACTERISTIC_SMOKE_DETECTED                         = 0x00000076,
    E_CHARACTERISTIC_STATUS_FAULT                           = 0x00000077,
    E_CHARACTERISTIC_STATUS_JAMMED                          = 0x00000078,
    E_CHARACTERISTIC_STATUS_LOW_BATTERY                     = 0x00000079,
    E_CHARACTERISTIC_STATUS_TAMPERED                        = 0x0000007A,
    E_CHARACTERISTIC_TARGET_HORIZONTAL_TILT_ANGLE           = 0x0000007B,
    E_CHARACTERISTIC_TARGET_POSITION                        = 0x0000007C,
    E_CHARACTERISTIC_TARGET_VERTICAL_TILT_ANGLE             = 0x0000007D,
    E_CHARACTERISTIC_SECURITY_SYSTEM_ALARM_TYPE             = 0x0000008E,
    E_CHARACTERISTIC_CHARGING_STATE                         = 0x0000008F,
    E_CHARACTERISTIC_CARBON_MONOXIDE_LEVEL                  = 0x00000090,
    E_CHARACTERISTIC_CARBON_MONOXIDE_PEAK_LEVEL             = 0x00000091,
    E_CHARACTERISTIC_CARBON_DIOXIDE_DETECTED                = 0x00000092,
    E_CHARACTERISTIC_CARBON_DIOXIDE_LEVEL                   = 0x00000093,
    E_CHARACTERISTIC_CARBON_DIOXIDE_PEAK_LEVEL              = 0x00000094,
    E_CHARACTERISTIC_AIR_QUALITY                            = 0x00000095,
    E_CHARACTERISTIC_STREAMING_STATUS                       = 0x00000120,
    E_CHARACTERISTIC_SUPPORTED_VIDEO_STREAM_CONFIGURATION   = 0x00000114,
    E_CHARACTERISTIC_SUPPORTED_AUDIO_STREAM_CONFIGURATION   = 0x00000115,
    E_CHARACTERISTIC_SUPPORTED_RTP_CONFIGURATION            = 0x00000116,
    E_CHARACTERISTIC_SETUP_ENDPOINTS                        = 0x00000118,
    E_CHARACTERISTIC_SELECTED_RTP_STREAM_CONFIGURATION      = 0x00000117,
    E_CHARACTERISTIC_VOLUME                                 = 0x00000119,
    E_CHARACTERISTIC_MUTE                                   = 0x0000011A,
    E_CHARACTERISTIC_NIGHT_VISION                           = 0x0000011B,
    E_CHARACTERISTIC_OPTICAL_ZOOM                           = 0x0000011C,
    E_CHARACTERISTIC_DIGITAL_ZOOM                           = 0x0000011D,
    E_CHARACTERISTIC_IMAGE_ROTATION                         = 0x0000011E,
    E_CHARACTERISTIC_IMAGE_MIRRORING                        = 0x0000011F,
    E_CHARACTERISTIC_ACCESSORY_FLAGS                        = 0x000000A6,
    E_CHARACTERISTIC_LOCK_PHYSICAL_CONTROLS                 = 0x000000A7,
    E_CHARACTERISTIC_CURRENT_AIR_PURIFIER_STATE             = 0x000000A9,
    E_CHARACTERISTIC_CURRENT_SLAT_STATE                     = 0x000000AA,
    E_CHARACTERISTIC_SLAT_TYPE                              = 0x000000C0,
    E_CHARACTERISTIC_FILTER_LIFT_LEVEL                      = 0x000000AB,
    E_CHARACTERISTIC_FILTER_CHANGE_INDICATION               = 0x000000AC,
    E_CHARACTERISTIC_RESET_FILTER_INDICATION                = 0x000000AD,
    E_CHARACTERISTIC_TARGET_AIR_PURIFIER_STATE              = 0x000000A8,
    E_CHARACTERISTIC_TARGET_FAN_STATE                       = 0x000000BF,
    E_CHARACTERISTIC_CURRENT_FAN_STATE                      = 0x000000AF,
    E_CHARACTERISTIC_ACTIVE                                 = 0x000000B0,
    E_CHARACTERISTIC_CURRENT_HEATER_COOLER_STATE            = 0x000000B1,
    E_CHARACTERISTIC_TARGET_HEATER_COOLER_STATE             = 0x000000B2,
    E_CHARACTERISTIC_CURRENT_HUMIDIFIER_DEHUMIDIFIER_STATE  = 0x000000B3,
    E_CHARACTERISTIC_TARGET_HUMIDIFIER_DEHUMIDIFIER_STATE   = 0x000000B4,
    E_CHARACTERISTIC_WATER_LEVEL                            = 0x000000B5,
    E_CHARACTERISTIC_SWING_MODE                             = 0x000000B6,
    E_CHARACTERISTIC_CURRENT_TILT_ANGLE                     = 0x000000C1,
    E_CHARACTERISTIC_TARGET_TILT_ANGLE                      = 0x000000C2,
    E_CHARACTERISTIC_OZONE_DENSITY                          = 0x000000C3,
    E_CHARACTERISTIC_NITROGEN_DIOXIDE_DENSITY               = 0x000000C4,
    E_CHARACTERISTIC_SULPHUR_DIOXIDE_DENSITY                = 0x000000C5,
    E_CHARACTERISTIC_PM25_DENSITY                           = 0x000000C6,
    E_CHARACTERISTIC_PM10_DENSITY                           = 0x000000C7,
    E_CHARACTERISTIC_VOC_DENSITY                            = 0x000000C8,
    E_CHARACTERISTIC_RELATIVE_HUMIDITY_DEHUMIDIFIER_THRESHOLD= 0x000000C9,
    E_CHARACTERISTIC_RELATIVE_HUMIDITY_HUMIDIFIER_THRESHOLD = 0x000000CA,
    E_CHARACTERISTIC_SERVICE_LABEL_INDEX                    = 0x000000CB,
    E_CHARACTERISTIC_SERVICE_LABEL_NAMESPACE                = 0x000000CD,
} teCharacteristicType;

typedef enum {
    E_POSITION_STATE_GOING_MIN = 0x00,
    E_POSITION_STATE_GOING_MAX = 0x01,
    E_POSITION_STATE_STOP      = 0x02,
} tePositionState;
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


