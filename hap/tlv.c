/****************************************************************************
 *
 * MODULE:             tlv.c
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
#include "tlv.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_TLV 1

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
static uint8 *pu8TlvGetRecordData(struct _tsTlvMessage *psTlvMessage, teTlvValue eType)
{
    CHECK_POINTER(psTlvMessage, NULL);
    DBG_vPrintln(DBG_TLV, "pu8TlvGetRecordData:%p,%d", psTlvMessage, eType);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMessage->sTlvRecord[i].u8Type == eType){
            DBG_vPrintln(DBG_TLV, "Get Record:[%d]", i);
            return psTlvMessage->sTlvRecord[i].psValue;
        }
    }
    ERR_vPrintln(T_TRUE, "Can't Find Tlv Record");
    return NULL;
}

static uint16 u16TlvMsgGetRecordLength(struct _tsTlvMessage *psTlvMessage, teTlvValue eType)
{
    DBG_vPrintln(DBG_TLV, "u16TlvMsgGetRecordLength:%d", eType);
    CHECK_POINTER(psTlvMessage, 0);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMessage->sTlvRecord[i].u8Type == eType){
            return psTlvMessage->sTlvRecord[i].u16Len;
        }
    }
    ERR_vPrintln(T_TRUE, "Can't Find Tlv Record");
    return 0;
}
static teTlvStatus eTlvRecordFormat(teTlvValue eType, uint8 *puValueData, uint16 u16ValueLen, tsTlv *psTlvRecord)
{
    CHECK_POINTER(puValueData, E_TLV_STATUS_ERROR);

    unsigned int num = (unsigned int)(u16ValueLen / TLV_FRAGMENTED);
    if(u16ValueLen % TLV_FRAGMENTED){
        num ++;
    }
    psTlvRecord->u16Len += (uint16)(u16ValueLen + TLV_HEADER * num);
    uint8 *psReturnData = (uint8*)realloc(psTlvRecord->psValue, psTlvRecord->u16Len * sizeof(uint8));
    CHECK_POINTER(psReturnData, E_TLV_STATUS_ERROR);
    psTlvRecord->psValue = psReturnData;

    int i = 0;
    for (i = 0; i < num - 1; ++i) {
        psReturnData[psTlvRecord->u16Offset] = eType;
        psReturnData[psTlvRecord->u16Offset + TLV_TYPE_LEN] = TLV_FRAGMENTED;
        memcpy(&psReturnData[psTlvRecord->u16Offset + TLV_HEADER], &puValueData[TLV_FRAGMENTED*i], TLV_FRAGMENTED);
        psTlvRecord->u16Offset += TLV_FRAGMENTED + TLV_HEADER;
    }
    psReturnData[psTlvRecord->u16Offset] = eType;
    psReturnData[psTlvRecord->u16Offset + TLV_TYPE_LEN] = (uint8)(u16ValueLen - TLV_FRAGMENTED * i);
    memcpy(&psReturnData[psTlvRecord->u16Offset + TLV_HEADER], &puValueData[TLV_FRAGMENTED * i], (size_t)(u16ValueLen - TLV_FRAGMENTED * i));
    psTlvRecord->u16Offset += u16ValueLen - TLV_FRAGMENTED * i + TLV_HEADER;

    return E_TLV_STATUS_OK;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsTlvMessage *psTlvMessageNew()
{
    tsTlvMessage *psTlvMsg = NULL;
    psTlvMsg = (tsTlvMessage*)malloc(sizeof(tsTlvMessage));
    CHECK_POINTER(psTlvMsg,NULL);
    memset(psTlvMsg, 0, sizeof(tsTlvMessage));
    psTlvMsg->efTlvMsgAddRecord = eTlvMessageAddRecord;
    psTlvMsg->eTlvMsgGetBinaryData = eTlvMsgGetBinaryData;
    return psTlvMsg;
}

teTlvStatus eTlvMessageFormat(uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg)
{
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    DBG_vPrintln(DBG_TLV, "eTlvMessageFormat:%p", psTlvMsg);
    uint16 u16OffSet = 0;
    for(int i = 0; (i < TLV_NUM)&&(u16Len > u16OffSet); i++){
        psTlvMsg->psTlvMsgGetRecordData     = pu8TlvGetRecordData;
        psTlvMsg->pu16TlvMsgGetRecordLength = u16TlvMsgGetRecordLength;
        psTlvMsg->sTlvRecord[i].u8Type      = psBuffer[u16OffSet];
        INF_vPrintln(1,"f:%p,i:%d,t:%d",pu8TlvGetRecordData, i, psTlvMsg->sTlvRecord[i].u8Type);
        u16OffSet += TLV_TYPE_LEN;
        while(psBuffer[u16OffSet] == TLV_FRAGMENTED){
            psTlvMsg->sTlvRecord[i].u16Len += TLV_FRAGMENTED;
            uint8* psTemp = (uint8*)realloc(psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
            CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
            psTlvMsg->sTlvRecord[i].psValue = psTemp;
            u16OffSet += TLV_LEN_LEN;
            memcpy(&psTlvMsg->sTlvRecord[i].psValue[psTlvMsg->sTlvRecord[i].u16Offset], &psBuffer[u16OffSet], TLV_FRAGMENTED);
            psTlvMsg->sTlvRecord[i].u16Offset += TLV_FRAGMENTED;
            u16OffSet += TLV_FRAGMENTED + TLV_TYPE_LEN;
        }
        psTlvMsg->sTlvRecord[i].u16Len += psBuffer[u16OffSet];
        uint8* psTemp = (uint8*)realloc(psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
        CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
        psTlvMsg->sTlvRecord[i].psValue = psTemp;
        memcpy(&psTlvMsg->sTlvRecord[i].psValue[psTlvMsg->sTlvRecord[i].u16Offset], &psBuffer[u16OffSet+1], psBuffer[u16OffSet]);
        u16OffSet += psBuffer[u16OffSet];
        u16OffSet += TLV_LEN_LEN;
        psTlvMsg->u8RecordNum ++;
    }
    INF_vPrintln(DBG_TLV, "Num:%d", psTlvMsg->u8RecordNum);
    for (int k = 0; k < psTlvMsg->u8RecordNum; ++k) {
        PrintArray(DBG_TLV, psTlvMsg->sTlvRecord[k].psValue, psTlvMsg->sTlvRecord[k].u16Len);
    }

    return E_TLV_STATUS_OK;
}

teTlvStatus eTlvMessageAddRecord(teTlvValue eType, uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg)
{
    DBG_vPrintln(DBG_TLV, "eTlvMessageAddRecord:%d-%d", eType, u16Len);
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMsg->sTlvRecord[i].u16Len == 0){
            DBG_vPrintln(DBG_TLV, "Add Record:%d", i);
            eTlvRecordFormat(eType, psBuffer, u16Len, &psTlvMsg->sTlvRecord[i]);
            psTlvMsg->u8RecordNum ++;
            break;
        }
    }
    return E_TLV_STATUS_OK;
}

teTlvStatus eTlvMessageRelease(tsTlvMessage *psTlvMsg)
{
    for (int i = 0; i < TLV_NUM; ++i) {
        FREE(psTlvMsg->sTlvRecord[i].psValue);
    }
    return E_TLV_STATUS_OK;
}

teTlvStatus eTlvMsgGetBinaryData(tsTlvMessage *psTlvMsg, uint8 **psBuffer, uint16 *pu16Len)
{
    DBG_vPrintln(DBG_TLV, "eTlvMsgGetBinaryData");
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    CHECK_POINTER(pu16Len, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    uint8 *psRet = NULL;
    uint16 u16Len = 0;
    uint16 offset = 0;
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMsg->sTlvRecord[i].u16Len != 0){
            u16Len += psTlvMsg->sTlvRecord[i].u16Len;
            printf("Len:%d\n", u16Len);
            uint8 *psTemp = (uint8*)realloc(psRet, u16Len);
            CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
            psRet = psTemp;
            memcpy(&psRet[offset], psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
            offset += psTlvMsg->sTlvRecord[i].u16Len;
        }
    }
    *pu16Len = u16Len;
    *psBuffer = psRet;
    return E_TLV_STATUS_OK;
}