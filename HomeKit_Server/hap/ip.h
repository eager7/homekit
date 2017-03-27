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
typedef uint8* (*tpfIpGetBinaryData)(tsTlvMessage *psTlv);
typedef teHapStatus (*tefIpSend)(tsTlvMessage *psTlv);
typedef struct {
    tsHttpEntry *psHttp;
    tsTlvPackage *psTlvPackage;
    tefIpSend efIpSend;
    tpfIpGetBinaryData pfIpGetBinaryData;
} tsIpMessage;
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
/*****************************************************************************
** Prototype    : psIpResponseNew
** Description  : 创建一个消息数据，用来向里面添加TLV格式数据
** Input        : none
** Output       : none
** Return Value : return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsIpMessage *psIpResponseNew();
/*****************************************************************************
** Prototype    : psIpMessageFormat
** Description  : 格式化tlv8形式的http协议包，可以直接通过返回的结构体依次取出tlv格式的数据包
** Input        : psBuffer, the data received by socket
 *                u16Len, the length of buffer
** Output       : none
** Return Value : return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsIpMessage *psIpMessageFormat(const uint8 *psBuffer, uint16 u16Len);
/*****************************************************************************
** Prototype    : eIpMessageRelease
** Description  : 释放上面两个函数分配的内存
** Input        : psIpMsg, the pointer of Ip message
** Output       : none
** Return Value : return the E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eIpMessageRelease(tsIpMessage *psIpMsg);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_IP_H
