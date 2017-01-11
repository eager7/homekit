/****************************************************************************
 *
 * MODULE:             Characteristic.h
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

#ifndef __H_CHARACTERISTIC_H_
#define __H_CHARACTERISTIC_H_

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
    E_CHARACTERISTIC_OK = 0,
    E_CHARACTERISTIC_ERROR = 1,
} teCharStatus;
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
} teCharacteristic;

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
    E_PERM_READ                 = 0x01, /*  */
    E_PERM_WRITE                = 0x02, /*  */
    E_PERM_PAIRED_READ          = 0x04, /* This characteristic can only be read by paired controllers */
    E_PERM_PAIRED_WRITE         = 0x08, /* This characteristic can only be written by paired controllers */
    E_PERM_EVENT_NOTIFYCATION   = 0x10, /* Boolean indicating if event notifications are enabled for this characteristic. */
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
    char            *psType;
    uint64          u64IID;     //Instance Id
    teTypeFormat    eFormat;
    tuCharValue     uValue;
    uint8           u8Permissions;
    char            *psDescription;
    teUnitType      eUnit;

    tsThreshold     sMinimumValue;
    tsThreshold     sMaximumValue;
    tsThreshold     sSetupValue;
    tsThreshold     sMaximumLen;
    tsThreshold     sMaximumDataLen;
} tsCharacteristics;

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
teCharStatus eCharCreateNew(tsCharacteristics *psCharacteristics);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_CHARACTERISTIC_H_*/



