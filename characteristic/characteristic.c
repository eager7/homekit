/****************************************************************************
 *
 * MODULE:             Characteristic.c
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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "characteristic.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsCharacteristics asCharacteristics[] = {
    {E_CHARACTERISTIC_ON, "public.hap.characteristic.on", E_PERM_READ | E_PERM_WRITE | E_PERM_PAIRED_WRITE | E_PERM_NOTIFY, E_TYPE_BOOL, {0}, {0}, {0}, E_UNIT_NULL},
    {E_CHARACTERISTIC_BRIGHTNESS, "public.hap.characteristic.brightness", E_PERM_PAIRED_READ | E_PERM_PAIRED_WRITE | E_PERM_NOTIFY, E_TYPE_INT, {0}, {100}, {1}, E_UNIT_PERCENTAGE},
    {E_CHARACTERISTIC_NAME, "public.hap.characteristic.name", E_PERM_READ, E_TYPE_STRING, {0}, {0}, {0}, E_UNIT_NULL},
    {E_CHARACTERISTIC_SERIAL_NUMBER, "public.hap.characteristic.serial-number", E_PERM_PAIRED_READ, E_TYPE_STRING, {0}, {0}, {0}, E_UNIT_NULL},
    {E_CHARACTERISTIC_IDENTIFY, "public.hap.characteristic.identify", E_PERM_PAIRED_WRITE, E_TYPE_BOOL, {0}, {0}, {0}, E_UNIT_NULL},
};
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/


