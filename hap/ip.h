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
    tsTlvMessage sTlvMsg;
    tpfIpGetBinaryData pfIpGetBinaryData;
}tsIpMessage;

typedef struct {
    tsHttpEntry sHttp;
    tsTlvMessage sTlvMsg;
} tsIpResponse;
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
teIpStatus eHapHandlePackage(char *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour);
tsIpMessage *psIpMessageFormat(uint8 *psBuffer, uint16 u16Len);
tsIpMessage *psIpResponseNew();
teIpStatus eIpMessageRelease(tsIpMessage *psIpMsg);

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_IP_H
