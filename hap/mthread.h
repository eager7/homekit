/****************************************************************************
 *
 * MODULE:             thread lib interface
 *
 * COMPONENT:          mthreads.h
 *
 * REVISION:           $Revision: 1.0 $
 *
 * DATED:              $Date: 2016-12-02 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2016. All rights reserved
 *
 ***************************************************************************/


#ifndef __M_THREADS_H__
#define __M_THREADS_H__

#if defined __cplusplus   
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define THREAD_SIGNAL SIGUSR1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum/** Enumerated type of thread status's */
{
    E_THREAD_OK,
    E_THREAD_ERROR_FAILED,
    E_THREAD_ERROR_TIMEOUT,
    E_THREAD_ERROR_NO_MEM,
} teThreadStatus;

/** Enumerated type for thread detached / joinable states */
typedef enum
{
    E_THREAD_JOINABLE,      /**< Thread is created so that it can be waited on and joined */
    E_THREAD_DETACHED,      /**< Thread is created detached so all resources are automatically free'd at exit. */
} teThreadDetachState;

typedef volatile enum
{
    E_THREAD_STOPPED,   /**< Thread stopped */
    E_THREAD_RUNNING,   /**< Thread running */
    E_THREAD_STOPPING,  /**< Thread signaled to stop */
} teState;              /**< Enumerated type of thread states */

/** Structure to represent a thread */
typedef struct
{
    volatile teState eState;
    teThreadDetachState eThreadDetachState; /**< Detach state of the thread */
    pthread_t pThread_Id;                   /**< Implementation specfific private structure */
    void *pvThreadData;                     /**< Pointer to threads data parameter */
} tsThread;

typedef void *(*tprThreadFunction)(void *psThreadInfoVoid); /** the func of thread */

typedef struct
{
    void **apvBuffer;                       /* the buffer of data store */
    uint32 u32Length;                       /*the length of queue */
    uint32 u32Front;                        /* the front pointer of queue */
    uint32 u32Rear;                         /* the last pointer of queue */

    pthread_mutex_t mutex;                  /* lock to operator queue */
    pthread_cond_t cond_space_available;    /* the indicate of queue space can be used */
    pthread_cond_t cond_data_available;     /* the indicate of queue data can be used */
} tsQueue;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teThreadStatus eThreadStart(tprThreadFunction prThreadFunction, tsThread *psThreadInfo, teThreadDetachState eDetachState);
teThreadStatus eThreadStop(tsThread *psThreadInfo);
/** Function to start a thread group*/
teThreadStatus eThreadGroupStart(int iNumThreads, tprThreadFunction prThreadFunction, tsThread *psThreadInfo, teThreadDetachState eDetachState);
teThreadStatus eThreadGroupStop(int iNumThreads, tsThread *psThreadInfo, teThreadDetachState eDetachState);
/** Function to be called within the thread when it is finished to clean up memory */
void vThreadFinish(tsThread *psThreadInfo);
teThreadStatus eThreadYield(void);/*Function to yield the CPU to another thread*/

teThreadStatus eLockCreate(pthread_mutex_t *psLock);
teThreadStatus eLockDestroy(pthread_mutex_t *psLock);
teThreadStatus eLockLock(pthread_mutex_t *psLock);
teThreadStatus eMLockLockTimed(pthread_mutex_t *psLock, uint32 u32WaitTimeout);
teThreadStatus eLockTryLock(pthread_mutex_t *psLock);
teThreadStatus eLockunLock(pthread_mutex_t *psLock);

teThreadStatus eQueueCreate(tsQueue *psQueue, uint32 u32Length); 
teThreadStatus eQueueDestroy(tsQueue *psQueue);
teThreadStatus eQueueEnqueue(tsQueue *psQueue, void *pvData);
teThreadStatus eQueueDequeue(tsQueue *psQueue, void **ppvData);
teThreadStatus eQueueDequeueTimed(tsQueue *psQueue, uint32 u32WaitTimeMil, void **ppvData);

#if defined __cplusplus
}
#endif

#endif /* __THREADS_H__ */


