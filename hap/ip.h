/****************************************************************************
 *
 * MODULE:             ip.h
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
#ifndef HOMEKIT_IP_H
#define HOMEKIT_IP_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "pairing.h"
#include "http_handle.h"
#include "bonjour.h"
#include "tlv.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_IP_STATUS_OK = 0,
    E_IP_STATUS_ERROR = 0,
} teIpStatus;

typedef uint8* (*tpfIpGetBinaryData)(tsTlvMessage *psTlv);
typedef uint8* (*tpfIpGetBinaryData)(tsTlvMessage *psTlv);
typedef struct {
    tsHttpEntry sHttp;
    tsTlvPackage *psTlvPackage;
    tpfIpGetBinaryData pfIpGetBinaryData;
}tsIpMessage;
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
tsIpMessage *psIpResponseNew();
tsIpMessage *psIpMessageFormat(uint8 *psBuffer, uint16 u16Len);
teIpStatus  eIpMessageRelease(tsIpMessage *psIpMsg);
teIpStatus eHapHandlePackage(tsProfile *psProfile, tsBonjour *psBonjour, uint8 *psBuffer, int iLen, int iSocketFd);
teIpStatus eHandleAccessoryPackage(tsProfile *psProfile, const uint8 *psData, uint16 u16Len, uint8 **psResp,
                                   uint16 *pu16Len);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_IP_H
