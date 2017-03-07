/****************************************************************************
 *
 * MODULE:             ip.c
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

#include <profile.h>
#include "ip.h"
#include "http_handle.h"
#include "light_bulb.h"
#include "bonjour.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_IP 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


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
/***        Local    Functions                                            ***/
/****************************************************************************/
static uint16 u16IpGetBinaryData()
{
    return 0;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsIpMessage *psIpResponseNew()
{
    tsIpMessage *psIpMsg = (tsIpMessage*)calloc(1, sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    psIpMsg->psTlvPackage = psTlvPackageGenerate();
    if(psIpMsg->psTlvPackage == NULL){
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}

tsIpMessage *psIpMessageFormat(const uint8 *psBuffer, uint16 u16Len)
{
    tsIpMessage *psIpMsg = (tsIpMessage*)calloc(1, sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    psIpMsg->psHttp = psHttpParser(psBuffer, u16Len);
    psIpMsg->psTlvPackage = psTlvPackageParser(psIpMsg->psHttp->acContentData, psIpMsg->psHttp->u16ContentLen);
    if(psIpMsg->psTlvPackage == NULL){
        ERR_vPrintln(T_TRUE, "psTlvPackageParser Failed");
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}

teHapStatus eIpMessageRelease(tsIpMessage *psIpMsg)
{
    eTlvPackageRelease(psIpMsg->psTlvPackage);
    FREE(psIpMsg->psHttp);
    FREE(psIpMsg);
    return E_HAP_STATUS_OK;
}