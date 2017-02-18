/****************************************************************************
 *
 * MODULE:             tlv.h
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
#ifndef HOMEKIT_TLV_H
#define HOMEKIT_TLV_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include <srp.h>
#include "http_handle.h"
#include "bonjour.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TLV_TYPE_LEN 1
#define TLV_LEN_LEN 1
#define TLV_HEADER (TLV_TYPE_LEN + TLV_LEN_LEN)
#define TLV_FRAGMENTED 255

#define TLV_NUM 10
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_TLV_STATUS_OK = 0x00,
    E_TLV_STATUS_ERROR,
} teTlvStatus;

typedef enum {
    E_TLV_VALUE_TYPE_METHOD         = 0x00,
    E_TLV_VALUE_TYPE_IDENTIFIER     = 0x01,
    E_TLV_VALUE_TYPE_SALT           = 0x02,
    E_TLV_VALUE_TYPE_PUBLIC_KEY     = 0x03,
    E_TLV_VALUE_TYPE_PROOF          = 0x04,
    E_TLV_VALUE_TYPE_ENCRYPTED_DATA = 0x05,
    E_TLV_VALUE_TYPE_STATE          = 0x06,
    E_TLV_VALUE_TYPE_ERROR          = 0x07,
    E_TLV_VALUE_TYPE_RETRY_DELAY    = 0x08,
    E_TLV_VALUE_TYPE_CERTIFICATE    = 0x09,
    E_TLV_VALUE_TYPE_SIGNATURE      = 0x0A,
    E_TLV_VALUE_TYPE_PERMISSIONS    = 0x0B,
    E_TLV_VALUE_TYPE_FRAGMENT_DATA  = 0x0C,
    E_TLV_VALUE_TYPE_FRAGMENT_LAST  = 0x0D,
    E_TLV_VALUE_TYPE_SEPARATOR      = 0x0F,
} teTlvValue;

typedef enum {
    E_TLV_ERROR_UNKNOW          = 0x01,
    E_TLV_ERROR_AUTHENTICATION  = 0x02,
    E_TLV_ERROR_BACKOFF         = 0x03,
    E_TLV_ERROR_MAXPEERS        = 0x04,
    E_TLV_ERROR_MAXTRIES        = 0x05,
    E_TLV_ERROR_UNAVAILABLE     = 0x06,
    E_TLV_ERROR_BUSY            = 0x07,
} teTlvError;

typedef struct {
    uint8   u8Type;
    uint16  u16Len;
    uint16  u16Offset;
    uint8   *psValue;
} tsTlv;

typedef teTlvStatus  (*tpeTlvMsgGetBinaryData)(struct _tsTlvMessage *psTlvMessage, uint8 **psBuffer, uint16 *pu16Len);
typedef uint8* (*tpfTlvMsgGetRecordData)(struct _tsTlvMessage *psTlvMessage, teTlvValue eType);
typedef uint16 (*tpu16TlvMsgGetRecordLength)(struct _tsTlvMessage *psTlvMessage, teTlvValue eType);
typedef teTlvStatus (*tefTlvMsgAddRecord)(teTlvValue eType, uint8 *psBuffer, uint16 u16Len, struct _tsTlvMessage *psTlvMessage);
typedef struct _tsTlvMessage{
    tsTlv sTlvRecord[TLV_NUM];
    uint8 u8RecordNum;
    tpeTlvMsgGetBinaryData eTlvMsgGetBinaryData;
    tpfTlvMsgGetRecordData psTlvMsgGetRecordData;
    tpu16TlvMsgGetRecordLength pu16TlvMsgGetRecordLength;
    tefTlvMsgAddRecord efTlvMsgAddRecord;
} tsTlvMessage;
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
tsTlvMessage *psTlvMessageNew();
teTlvStatus eTlvMessageFormat(uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg);
teTlvStatus eTlvMessageAddRecord(teTlvValue eType, uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg);
teTlvStatus eTlvMessageRelease(tsTlvMessage *psTlvMsg);
teTlvStatus eTlvMsgGetBinaryData(tsTlvMessage *psTlvMsg, uint8 **psBuffer, uint16 *pu16Len);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_TLV_H
