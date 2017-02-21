/****************************************************************************
 *
 * MODULE:             bonjour.h
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
#ifndef HOMEKIT_BONJOUR_H
#define HOMEKIT_BONJOUR_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "profile.h"
#include "accessory.h"
#include "mthread.h"
#include <dns_sd.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BONJOUR_SERVER_TYPE "_hap._tcp."
#define MULTICAST_DNS_ADDRESS "224.0.0.251"
#define MULTICAST_DNS_PORT 5353

#define ACCESSORY_SERVER_LISTEN 5
#define ACCESSORY_SERVER_PORT 0
#define MAX_NUMBER_CLIENT 8
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_BONJOUR_STATUS_OK,
    E_BONJOUR_STATUS_ERROR,
} teBonjStatus;

typedef enum {
    E_BONJOUR_FEATURE_FLAG_PAIRING = 0x01,  /* Supports HAP Pairing. This flag is required for all HomeKit accessories. */
} teFeatureFlag;

typedef enum {
    E_BONJOUR_TXT_STATUS_FLAG_UNPAIRED = 0x01,          /* Accessory has not been paired with any controllers. */
    E_BONJOUR_TXT_STATUS_FLAG_UNCONFIGURED_WIFI = 0x02, /* Accessory has not been configured to join a Wi-Fi network. */
    E_BONJOUR_TXT_STATUS_FLAG_DETECTED_PROBLEM = 0x04,  /* A problem has been detected on the accessory. */
} teStatusFlag;

typedef struct {
    /* c#---Current configuration number,
     * Must update when an accessory, service, or characteristic is added or removed on the accessory server.
     * Accessories must increment the config number after a firmware update.
     * This must have a range of 1-4294967295 and wrap to 1 when it overflows.*/
    uint64  u64CurrentCfgNumber;
    /* ff---Feature flags (e.g. "0x3" for bits 0 and 1). Required if non-zero */
    uint8   u8FeatureFlag;
    /* id---Device ID of the accessory.The Device ID must be formatted as XX:XX:XX:XX:XX:XX,This value is also used as the accessory's Pairing Identifier. */
    uint64  u64DeviceID;
    char    psDeviceID[17]; /* Store XX:XX:XX:XX:XX:XX */
    /* md---Model name of the accessory, e.g."Device1,1" */
    char    *psModelName;
    /* pv---Protocol version string <major>.<minor>, Required if value is not "1.0".
     * The client should check this before displaying an accessory to the user.
     * If the major version is greater than the major version the client software was built to support,
     * it should hide the accessory from the user. */
    char    auProtocolVersion[2];
    /* s#---Current state number, This must have a value of "1" */
    uint32  u32CurrentStaNumber;
    /* sf---Status flags, Value should be an unsigned integer. Required if non-zero */
    uint8   u8StatusFlag;
    /* ci---Accessory Category Identifier. Required. Indicates the category that best describes the primary function of the accessory,This must have a range of 1-65535.  */
    teAccessoryType  eAccessoryCategoryID;
} tsBonjourText;

typedef struct {
    char *psServiceName;            /* homekit bonjour server name, _hap._tcp. */
    char *psHostName;               /* accessory host name, can be NULL */
    //char *psInstanceName;           /* same as accessory name */
    char *pcSetupCode;              /* Setup Code */

    int  iSocketFd;                 /* Accessory server socket */
    uint16 u16Port;                 /* Accessory server socket port */
    tsThread sBonjourThread;               /* server socket's thread */
    DNSServiceRef psDnsRef;         /* mDNS pointer */
    TXTRecordRef  txtRecord;        /* txt record */
    tsBonjourText sBonjourText;     /* txt record struct */
} tsBonjour;


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
teBonjStatus eBonjourInit(tsProfile *psProfile, char *pcSetupCode, char *psModel);
teBonjStatus eBonjourFinished(tsProfile *psProfile);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_BONJOUR_H
