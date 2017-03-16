/****************************************************************************
 *
 * MODULE:             controller.h
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
#ifndef HOMEKIT_CONTROLLER_H
#define HOMEKIT_CONTROLLER_H
#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
#include "list.h"
#include "accessory.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    tsCharacteristic sCharacter;
    struct dl_list list;
} tsEventNotify;

typedef struct {
    pthread_mutex_t mutex;
    int iSocketFd;
    uint64 u64NumberReceive;
    uint64 u64NumberSend;
    uint8  auControllerToAccessoryKey[32];
    uint8  auAccessoryToControllerKey[32];
    tsEventNotify sEventNotify;
    struct dl_list list;
} tsController;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern tsController sControllerHead;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/*****************************************************************************
** Prototype    : eControllerInit
** Description  : 初始化控制器结构体，包括内部的互斥锁，链表
** Input        :
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus  eControllerInit();
/*****************************************************************************
** Prototype    : psControllerListAdd
** Description  : 在控制器链表中添加一个新的对象，此行为通常在另一个控制设备接入时发生，如pad接入
** Input        : psControllerList, the list head
** Output       : none
** Return Value : if success, return the object's pointer, else return NULL

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
tsController *psControllerListAdd(tsController *psControllerList);
/*****************************************************************************
** Prototype    : psControllerListAdd
** Description  : 在控制器链表中删除一个对象，此行为通常发生在设备断开后，如配置设置时，或者超出通信范围时
** Input        : psControllerList, the list head
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus  eControllerListDel(tsController *psController);
/*****************************************************************************
** Prototype    : eControllerFinished
** Description  : 程序结束时调用此函数，释放分配的内存和锁
** Input        : none
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eControllerFinished();
/*****************************************************************************
** Prototype    : eControllerListAddNotify
** Description  : 在一个控制器结构体中添加通知链表节点，用来处理IOS设备发来的ev事件
** Input        : psController, the IOS device's structure
 *                psCharacter, the structure will be copied into event info
** Output       : none
** Return Value : if success, return E_HAP_STATUS_OK, else return E_HAP_STATUS_ERROR

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
teHapStatus eControllerListAddNotify(tsController *psController, tsCharacteristic *psCharacter);
/*****************************************************************************
** Prototype    : bIsControllerNotify
** Description  : 查询一个控制器的某个属性是否支持上报功能
** Input        : psController, the IOS device's structure
 *                psCharacter, the structure will be copied into event info
** Output       : none
** Return Value : if success, return T_TRUE, else return T_FALSE

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
bool_t bIsControllerNotify(tsController *psController, tsCharacteristic *psCharacter);
/*****************************************************************************
** Prototype    : vBroadcastMessage
** Description  : 将数据进行加密后发送到指定控制器上
** Input        : psController, the IOS device's structure
 *                psBuffer, the data will be sent
 *                sLength, the length of buffer
** Output       : none
** Return Value : if success, return T_TRUE, else return T_FALSE

** History      :
** Date         : 2017/2/27
** Author       : PCT
*****************************************************************************/
void vBroadcastMessage(tsController *psController, uint8 *psBuffer, size_t sLength);
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
#if defined __cplusplus
}
#endif
#endif //HOMEKIT_CONTROLLER_H
