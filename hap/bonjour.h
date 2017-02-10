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
#define MAX_NUMBER_CLIENT 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_BONJOUR_STATUS_OK,
    E_BONJOUR_STATUS_ERROR,
} teBonjStatus;

typedef enum {
    E_BONJOUR_FEATURE_FLAG_PAIRING = 0x01,
} tsFeatureFlag;

typedef enum {
    E_BONJOUR_TXT_STATUS_FLAG_UNPAIRED = 0x01,
    E_BONJOUR_TXT_STATUS_FLAG_UNCONFIGURED_WIFI = 0x02,
    E_BONJOUR_TXT_STATUS_FLAG_DETECTED_PROBLEM = 0x04,
} tsTxtStatusFlag;

typedef struct {
    uint64  u64CurrentCfgNumber;    /* c#---Current configuration number */
    uint8   u8FeatureFlag;          /* ff---Required if non-zero */
    uint64  u64DeviceID;            /* id---The Device ID must be formatted as XX:XX:XX:XX:XX:XX */
    char    *psDeviceID;
    char    *psModelName;           /* md---Model name of the accessory */
    char    auProtocolVersion[2];   /* pv---Protocol version string <major>.<minor> */
    uint32  u32iCurrentStaNumber;   /* s#---Current state number,This must have a value of "1" */
    uint8   u8StatusFlag;           /* sf---Value should be an unsigned integer. Required if non-zero */
    teAccessoryType  eAccessoryCategoryID; /* ci---Accessory Category Identifier. Required. Indicates the category that best describes the primary function of the accessory */
} tsBonjourText;

typedef struct {
    char *psServiceName;            /* homekit bonjour server name, _hap._tcp. */
    char *psHostName;               /* accessory host name, can be NULL */
    char *psInstanceName;           /* same as accessory name */
    char *pcSetupCode;              /* Setup Code */

    int  iSocketFd;                 /* Accessory server socket */
    uint16 u16Port;                 /* Accessory server socket port */
    tsThread sThread;               /* server socket's thread */
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
teBonjStatus eBonjourInit(tsProfile *psProfile, char *pcSetupCode);
teBonjStatus eBonjourFinished(tsProfile *psProfile);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_BONJOUR_H
