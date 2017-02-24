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

#include "ip.h"
#include "http_handle.h"
#include "light_bulb.h"
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
extern tsController sController;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
tsIpMessage *psIpResponseNew()
{
    tsIpMessage *psIpMsg = (tsIpMessage*)malloc(sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    memset(psIpMsg, 0, sizeof(tsIpMessage));
    psIpMsg->psTlvPackage = psTlvPackageNew();
    if(psIpMsg->psTlvPackage == NULL){
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}
tsIpMessage *psIpMessageFormat(uint8 *psBuffer, uint16 u16Len)
{
    tsIpMessage *psIpMsg = (tsIpMessage*)malloc(sizeof(tsIpMessage));
    CHECK_POINTER(psIpMsg, NULL);
    memset(psIpMsg, 0, sizeof(tsIpMessage));
    eHttpParser(E_HTTP_POST, psBuffer, u16Len, &psIpMsg->sHttp);
    psIpMsg->psTlvPackage = psTlvPackageFormat(psIpMsg->sHttp.acContentData, psIpMsg->sHttp.u16ContentLen);
    if(psIpMsg->psTlvPackage == NULL){
        ERR_vPrintln(T_TRUE, "psTlvPackageFormat Failed");
        FREE(psIpMsg); return NULL;
    }
    return psIpMsg;
}

teIpStatus eIpMessageRelease(tsIpMessage *psIpMsg)
{
    eTlvPackageRelease(psIpMsg->psTlvPackage);
    FREE(psIpMsg);
    return E_IP_STATUS_OK;
}

teIpStatus eHapHandlePackage(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    tsHttpEntry sHttpEntry;
    eHttpParser(E_HTTP_POST, psBuffer, (uint16)iLen, &sHttpEntry);

    DBG_vPrintln(DBG_IP, "Data Len:%d", sHttpEntry.u16ContentLen);
    if(strstr((char*)sHttpEntry.acDirectory, "pair-setup")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Setup");
        eHandlePairSetup(psBuffer, iLen, iSocketFd, psBonjour);
    } else if(strstr((char*)sHttpEntry.acDirectory, "pair-verify")){
        DBG_vPrintln(DBG_IP, "IOS Device Pair Verify");
        eHandlePairVerify(psBuffer, iLen, iSocketFd, psBonjour);
        eHandleAccessoryRequest(iSocketFd, psBonjour);
    } else if (strstr((char*)sHttpEntry.acDirectory, "identify")){
        close(iSocketFd);
    } else {
        DBG_vPrintln(DBG_IP, "Handle Controller Request");
    }
    return E_IP_STATUS_OK;
}

teIpStatus eHandleAccessoryPackage(uint8 *psData, uint16 u16Len, uint8 **psResp, uint16 *pu16Len)
{
    CHECK_POINTER(psData, E_IP_STATUS_ERROR);
    CHECK_POINTER(psResp, E_IP_STATUS_ERROR);
    CHECK_POINTER(pu16Len, E_IP_STATUS_ERROR);

    DBG_vPrintln(DBG_IP, "eHandleAccessoryPackage:%s\n", psData);
    //tsHttpEntry sHttpEntry;
    //memset(&sHttpEntry, 0, sizeof(tsHttpEntry));
    //eHttpParser(psData, u16Len, &sHttpEntry);

    if (strstr((char*)psData, "/accessories")) {
        //Publish the characteristics of the accessories
        NOT_vPrintln(DBG_IP, "Ask for accessories info\n");
        json_object *temp = psGetAccessoryInfoJson(&sLightBulb.sAccessory);
        char *t = "HTTP/1.1 200 OK\n\r"
                "Content-Type: application/hap+json\n\r"
                "Content-Length: 743\n\r"
                "\n\r"
                "{\"accessories\":[{\"aid\":1,\"services\":[{\"iid\":1,\"type\":\"3E\",\"characteristics\":[{\"value\":\"12345 Light\",\"perms\":[\"pr\"],\"type\":\"23\",\"iid\":2,\"format\":\"string\"},{\"value\":\"ET\",\"perms\":[\"pr\"],\"type\":\"20\",\"iid\":3,\"format\":\"string\"},{\"value\":\"Light\",\"perms\":[\"pr\"],\"type\":\"21\",\"iid\":4,\"format\":\"string\"},{\"value\":\"12345678\",\"perms\":[\"pr\"],\"type\":\"30\",\"iid\":5,\"format\":\"string\"},{\"perms\":[\"pw\"],\"type\":\"14\",\"iid\":6,\"format\":\"bool\"}]},{\"iid\":7,\"type\":\"43\",\"characteristics\":[{\"value\":\"Light\",\"perms\":[\"pr\"],\"type\":\"23\",\"iid\":8,\"format\":\"string\"},{\"value\":true,\"perms\":[\"pr\",\"pw\",\"ev\"],\"type\":\"25\",\"iid\":9,\"format\":\"bool\"},{\"value\":50,\"minValue\":0,\"maxValue\":100,\"minStep\":1,\"perms\":[\"pr\",\"pw\",\"ev\"],\"type\":\"8\",\"iid\":10,\"unit\":\"percentage\",\"format\":\"int\"}]}]}]}";

        //*pu16Len = (uint16)strlen(t);
        *pu16Len = (uint16)strlen(json_object_get_string(temp));
        eHttpMessageFormat(E_HTTP_STATUS_SUCCESS_OK, "application/hap+json", json_object_get_string(temp), (uint16)strlen(json_object_get_string(temp)), psResp);
        //eHttpMessageFormat(E_HTTP_STATUS_SUCCESS_OK, "application/hap+json", t, (uint16)strlen(t), psResp);

    }
    return E_IP_STATUS_OK;
}