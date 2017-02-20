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

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_PROFILE_OK,
    E_PROFILE_ERROR,
} teProfileStatus;

typedef enum {
    E_PROFILE_CMD_LIGHT_BULB_ON,
    E_PROFILE_CMD_LIGHT_BULB_OFF,
} teProfileCmd;

typedef teProfileStatus (*fpeHandleFunc)(tsAccessory *psAccessory);
typedef teProfileStatus (*fpeCallbackFunc)(teProfileCmd eProfileCmd);
typedef uint8* (*fpsGetAccessoryAttData)(tsAccessory *psAccessory);

typedef struct {
    teProfileCmd eProfileCmd;
    fpeHandleFunc peHandleFunc;
} tsProfileHandle;

typedef struct {
    tsAccessory     sAccessory;
    fpeCallbackFunc peCallbackFunc;
    fpsGetAccessoryAttData psGetAccessoryAttData;
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
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_PROFILE_H
