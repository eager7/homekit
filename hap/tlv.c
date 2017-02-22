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
#define DBG_TLV 0

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
static uint8 *pu8TlvRecordGetData(tsTlvMessage *psTlvMessage, teTlvValue eType)
{
    CHECK_POINTER(psTlvMessage, NULL);
    DBG_vPrintln(DBG_TLV, "pu8TlvRecordGetData:%p,%d", psTlvMessage, eType);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMessage->sTlvRecord[i].u8Type == eType){
            DBG_vPrintln(DBG_TLV, "Get Record:[%d]", i);
            PrintArray(DBG_TLV, psTlvMessage->sTlvRecord[i].psValue, psTlvMessage->sTlvRecord[i].u16Len);
            return psTlvMessage->sTlvRecord[i].psValue;
        }
    }
    ERR_vPrintln(T_TRUE, "Can't Find Tlv Record");
    return NULL;
}
static uint16 u16TlvRecordGetLen(tsTlvMessage *psTlvMessage, teTlvValue eType)
{
    DBG_vPrintln(DBG_TLV, "u16TlvRecordGetLen:%d", eType);
    CHECK_POINTER(psTlvMessage, 0);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMessage->sTlvRecord[i].u8Type == eType){
            return psTlvMessage->sTlvRecord[i].u16Len;
        }
    }
    ERR_vPrintln(T_TRUE, "Can't Find Tlv Record");
    return 0;
}

static teTlvStatus eTlvRecordAdd(teTlvValue eType, uint8 *puValueData, uint16 u16ValueLen, tsTlv *psTlvRecord)
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
static teTlvStatus eTlvMessageAddRecord(teTlvValue eType, uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg)
{
    DBG_vPrintln(DBG_TLV, "eTlvMessageAddRecord:%d-%d", eType, u16Len);
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMsg->sTlvRecord[i].u16Len == 0){
            DBG_vPrintln(DBG_TLV, "Add Record:%d", i);
            eTlvRecordAdd(eType, psBuffer, u16Len, &psTlvMsg->sTlvRecord[i]);
            psTlvMsg->u8RecordNum ++;
            break;
        }
    }
    return E_TLV_STATUS_OK;
}
static teTlvStatus eTlvMessageGetData(tsTlvMessage *psTlvMsg, uint8 **psBuffer, uint16 *pu16Len)
{
    DBG_vPrintln(DBG_TLV, "eTlvMessageGetData");
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    CHECK_POINTER(pu16Len, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    uint8 *psRet = NULL;
    uint16 u16Len = 0;
    uint16 offset = 0;
    for (int i = 0; i < TLV_NUM; ++i) {
        if(psTlvMsg->sTlvRecord[i].u16Len != 0){
            u16Len += psTlvMsg->sTlvRecord[i].u16Len;
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
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsTlvPackage *psTlvPackageNew()
{
    tsTlvPackage *psTlvPackage = NULL;
    psTlvPackage = (tsTlvPackage*)malloc(sizeof(tsTlvPackage));
    CHECK_POINTER(psTlvPackage,NULL);
    memset(psTlvPackage, 0, sizeof(tsTlvPackage));
    psTlvPackage->psTlvRecordGetData = pu8TlvRecordGetData;
    psTlvPackage->pu16TlvRecordGetLen = u16TlvRecordGetLen;
    psTlvPackage->efTlvMessageAddRecord = eTlvMessageAddRecord;
    psTlvPackage->eTlvMessageGetData = eTlvMessageGetData;
    return psTlvPackage;
}
teTlvStatus eTlvPackageRelease(tsTlvPackage *psTlvPackage)
{
    psTlvPackage->psTlvRecordGetData = NULL;
    psTlvPackage->pu16TlvRecordGetLen = NULL;
    psTlvPackage->efTlvMessageAddRecord = NULL;
    psTlvPackage->eTlvMessageGetData = NULL;
    for (int i = 0; i < TLV_NUM; ++i) {
        FREE(psTlvPackage->sMessage.sTlvRecord[i].psValue);
    }
    FREE(psTlvPackage);
    return E_TLV_STATUS_OK;
}

tsTlvPackage *psTlvPackageFormat(uint8 *psBuffer, uint16 u16Len)
{
    CHECK_POINTER(psBuffer, NULL);
    tsTlvPackage *psTlvPackage = psTlvPackageNew();
    CHECK_POINTER(psTlvPackage, NULL);
    uint16 u16OffSet = 0;
    for(int i = 0; (i < TLV_NUM)&&(u16Len > u16OffSet); i++){
        psTlvPackage->sMessage.sTlvRecord[i].u8Type      = psBuffer[u16OffSet];
        u16OffSet += TLV_TYPE_LEN;
        while(psBuffer[u16OffSet] == TLV_FRAGMENTED){
            psTlvPackage->sMessage.sTlvRecord[i].u16Len += TLV_FRAGMENTED;
            uint8* psTemp = (uint8*)realloc(psTlvPackage->sMessage.sTlvRecord[i].psValue, psTlvPackage->sMessage.sTlvRecord[i].u16Len);
            CHECK_POINTER(psTemp, NULL);
            psTlvPackage->sMessage.sTlvRecord[i].psValue = psTemp;
            u16OffSet += TLV_LEN_LEN;
            memcpy(&psTlvPackage->sMessage.sTlvRecord[i].psValue[psTlvPackage->sMessage.sTlvRecord[i].u16Offset], &psBuffer[u16OffSet], TLV_FRAGMENTED);
            psTlvPackage->sMessage.sTlvRecord[i].u16Offset += TLV_FRAGMENTED;
            u16OffSet += TLV_FRAGMENTED + TLV_TYPE_LEN;
        }
        psTlvPackage->sMessage.sTlvRecord[i].u16Len += psBuffer[u16OffSet];
        uint8* psTemp = (uint8*)realloc(psTlvPackage->sMessage.sTlvRecord[i].psValue, psTlvPackage->sMessage.sTlvRecord[i].u16Len);
        CHECK_POINTER(psTemp, NULL);
        psTlvPackage->sMessage.sTlvRecord[i].psValue = psTemp;
        memcpy(&psTlvPackage->sMessage.sTlvRecord[i].psValue[psTlvPackage->sMessage.sTlvRecord[i].u16Offset], &psBuffer[u16OffSet+1], psBuffer[u16OffSet]);
        u16OffSet += psBuffer[u16OffSet];
        u16OffSet += TLV_LEN_LEN;
        psTlvPackage->sMessage.u8RecordNum ++;
    }
    INF_vPrintln(DBG_TLV, "Num:%d", psTlvPackage->sMessage.u8RecordNum);
    for (int k = 0; k < psTlvPackage->sMessage.u8RecordNum; ++k) {
        DBG_vPrintln(DBG_TLV, "Array[%d]", k);
        PrintArray(DBG_TLV, psTlvPackage->sMessage.sTlvRecord[k].psValue, psTlvPackage->sMessage.sTlvRecord[k].u16Len);
    }

    return psTlvPackage;
}

