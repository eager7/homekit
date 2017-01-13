/****************************************************************************
 *
 * MODULE:             bonjour.c
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
#include "bonjour.h"
#include "mthread.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_BONJOUR 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static teBonjStatus eBonjourMdnsInit(void);
static void *pvBonjourThreadHandle(void *psThreadInfoVoid);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static tsBonjour sBonjour;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teBonjStatus eBonjourInit(tsProfile *psProfile)
{
    memset(&sBonjour, 0, sizeof(sBonjour));
    sBonjour.psServiceName = BONJOUR_SERVER_TYPE;
    sBonjour.psHostName = "chang.tao#123456.local";
    sBonjour.psInstanceName = "chang.tao#123456.";
    sBonjour.u16Port = 1200;
    sBonjour.psTextRecord = "c#=1.ff=0.id=03:d2:24:a1:bd:75.md=test.pv=1/.0.s#=1.sf=0.ci=5.";

    if(eBonjourMdnsInit() != E_BONJOUR_STATUS_OK){
        return E_BONJOUR_STATUS_ERROR;
    }

    sBonjour.sThread.pvThreadData = psProfile;
    CHECK_RESULT(eThreadStart(pvBonjourThreadHandle, &sBonjour.sThread, E_THREAD_DETACHED), E_THREAD_OK, E_BONJOUR_STATUS_ERROR);

    return E_BONJOUR_STATUS_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teBonjStatus eBonjourMdnsInit(void)
{
    sBonjour.iSocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sBonjour.iSocketFd == -1){
        ERR_vPrintln(T_TRUE, "socket create failed:[%s]", strerror(errno));
        return E_BONJOUR_STATUS_ERROR;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(MULTICAST_DNS_PORT);
    int ret = bind(sBonjour.iSocketFd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        ERR_vPrintln(T_TRUE, "bind address failed:[%s]", strerror(errno));
        close(sBonjour.iSocketFd);
        return E_BONJOUR_STATUS_ERROR;
    }

    struct ip_mreq multi_addr;
    memset(&multi_addr, 0, sizeof(multi_addr));
    multi_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_DNS_ADDRESS);
    multi_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    int re = 1;
    setsockopt(sBonjour.iSocketFd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re));
    setsockopt(sBonjour.iSocketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multi_addr, sizeof(multi_addr));

    return E_BONJOUR_STATUS_OK;
}

static void *pvBonjourThreadHandle(void *psThreadInfoVoid)
{
    tsThread *psThreadInfo = (tsThread *)psThreadInfoVoid;
    tsProfile *psProfile = (tsProfile*)psThreadInfo->pvThreadData;
    psThreadInfo->eState = E_THREAD_RUNNING;

    while(psThreadInfo->eState == E_THREAD_RUNNING){

    }
    DBG_vPrintln(DBG_BONJOUR, "pvBonjourThreadHandle Exit");
    vThreadFinish(psThreadInfo);

    return NULL;
}