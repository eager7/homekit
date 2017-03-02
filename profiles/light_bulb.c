/****************************************************************************
 *
 * MODULE:             light_bulb.c
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
#include "light_bulb.h"
#include "profile.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_PROFILE (1)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static teProfileStatus eLightBulbOn(tsAccessory *psAccessory);
static teProfileStatus eLightBulbOff(tsAccessory *psAccessory);
static teProfileStatus eLightBulbHandle(tsAccessory *psAccessory, teProfileCmd eProfileCmd);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


tsProfileHandle sLightBulbHandle[] = {
        {E_PROFILE_CMD_LIGHT_BULB_ON, eLightBulbOn},
        {E_PROFILE_CMD_LIGHT_BULB_OFF, eLightBulbOff},
};
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teProfileStatus eLightBulbHandle(tsAccessory *psAccessory, teProfileCmd eProfileCmd)
{
    for(int i = 0; i < sizeof(sLightBulbHandle)/sizeof(tsProfileHandle); i++){
        if((sLightBulbHandle[i].eProfileCmd == eProfileCmd)&&(sLightBulbHandle[i].peHandleFunc != NULL)){
            return sLightBulbHandle[i].peHandleFunc(psAccessory);
        }
    }
    return E_PROFILE_ERROR;
}

static teProfileStatus eLightBulbOn(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_PROFILE, "eLightBulbOn");
    return E_PROFILE_OK;
}
static teProfileStatus eLightBulbOff(tsAccessory *psAccessory)
{
    DBG_vPrintln(DBG_PROFILE, "eLightBulbOff");
    return E_PROFILE_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tsProfile *psLightBulbProfileInit(char *psName, uint64 u64DeviceID, char *psSerialNumber, char *psManufacturer, char *psModel)
{
    tsProfile *psProfile = psProfileNew(psName, u64DeviceID, psSerialNumber, psManufacturer, psModel,
                                        E_HAP_TYPE_LIGHT_BULB, NULL);
    return psProfile;
}

teProfileStatus eLightBulbProfileRelease(tsProfile *psProfile)
{
    eProfileRelease(psProfile);
    return E_PROFILE_OK;
}
