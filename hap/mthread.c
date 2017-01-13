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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <mthread.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_THREADS (0)
#define DBG_LOCKS   (0)
#define DBG_QUEUE   (0)

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static void vthread_signal_handler(int sig);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern int verbosity;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
/** Signal handler to receive THREAD_SIGNAL.
 *  This is just used to interrupt system calls such as recv() and sleep().
 */
static void vthread_signal_handler(int sig)
{
    DBG_vPrintln(DBG_THREADS, "Signal %d received\n", sig);
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teThreadStatus eThreadStart(tprThreadFunction prThreadFunction, tsThread *psThreadInfo, teThreadDetachState eDetachState)
{
    psThreadInfo->eState = E_THREAD_STOPPED;
    psThreadInfo->eThreadDetachState = eDetachState;

    static int iFirstTime = 1;
    if (iFirstTime) {
        /* Set up sigmask to receive configured signal in the main thread. 
        * All created threads also get this signal mask, so all threads
        * get the signal. But we can use pthread_signal to direct it at one.
        */
        struct sigaction sa;
        sa.sa_handler = vthread_signal_handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        if (sigaction(THREAD_SIGNAL, &sa, NULL) == -1) {
            ERR_vPrintln(T_TRUE, "sigaction:%s\n", strerror(errno));
        } else {
            DBG_vPrintln(DBG_THREADS, "Signal action registered\n\r");
            iFirstTime = 0;
        }
    }

    CHECK_STATUS(pthread_create(&psThreadInfo->pThread_Id, NULL, prThreadFunction, psThreadInfo), 0, E_THREAD_ERROR_FAILED);

    if (eDetachState == E_THREAD_DETACHED)
    {
        DBG_vPrintln(DBG_THREADS, "Detach Thread %p - ", psThreadInfo);
        CHECK_STATUS(pthread_detach(psThreadInfo->pThread_Id), 0, E_THREAD_ERROR_FAILED);
    }
    DBG_vPrintln(DBG_THREADS, "Create Thread %p\n", psThreadInfo);
    
    return  E_THREAD_OK;
}

teThreadStatus eThreadGroupStart(int iNumThreads, tprThreadFunction prThreadFunction, tsThread *psThreadInfo, teThreadDetachState eDetachState)
{
    int i = 0, j = 0;
    for(i = 0; i < iNumThreads; i++)
    {
        if(E_THREAD_OK != eThreadStart(prThreadFunction, &psThreadInfo[i], eDetachState))
        {
            goto err;
        }
    }
    return E_THREAD_OK;
err:
    if(E_THREAD_JOINABLE == eDetachState)
    {
        for(j = 0; j < i; j++)
        {
            eThreadStop(&psThreadInfo[j]);
        }
    }
    else
    {
        for(j = 0; j < i; j++)
        {
            vThreadFinish(&psThreadInfo[j]);
        }   
    }
    return E_THREAD_ERROR_FAILED;
}

teThreadStatus eThreadGroupStop(int iNumThreads, tsThread *psThreadInfo, teThreadDetachState eDetachState)
{
    int j = 0;
    if(E_THREAD_JOINABLE == eDetachState)
    {
        for(j = 0; j < iNumThreads; j++)
        {
            eThreadStop(&psThreadInfo[j]);
        }
    }
    else
    {
        for(j = 0; j < iNumThreads; j++)
        {
            vThreadFinish(&psThreadInfo[j]);
        }   
    }
    return E_THREAD_ERROR_FAILED;
}

teThreadStatus eThreadStop(tsThread *psThreadInfo)
{
    DBG_vPrintln(DBG_THREADS, "Stopping Thread %p\n", psThreadInfo);
    
    psThreadInfo->eState = E_THREAD_STOPPING;
    if (0 != psThreadInfo->pThread_Id)
    {
        /* Send signal to the thread to kick it out of any system call it was in */
        pthread_kill(psThreadInfo->pThread_Id, THREAD_SIGNAL);
        DBG_vPrintln(DBG_THREADS, "Signaled Thread %p\n", psThreadInfo);
        
        if (psThreadInfo->eThreadDetachState == E_THREAD_JOINABLE)
        {
            /* Thread is joinable */
            CHECK_STATUS(pthread_join(psThreadInfo->pThread_Id, NULL), 0, E_THREAD_ERROR_FAILED);
        }
        else
        {
            DBG_vPrintln(DBG_THREADS, "This thread:%p is a detached thread, not need join it!\n", psThreadInfo);
            return E_THREAD_ERROR_FAILED;
        }
    }
    
    DBG_vPrintln(DBG_THREADS, "Stopped Thread %p\n", psThreadInfo);
    psThreadInfo->eState = E_THREAD_STOPPED;
    
    return  E_THREAD_OK;
}

void vThreadFinish(tsThread *psThreadInfo)
{
    DBG_vPrintln(DBG_THREADS, "Finish Thread %p\n", psThreadInfo);

    psThreadInfo->eState = E_THREAD_STOPPED;    
    /* Cleanup function is called when pthread quits */
    pthread_exit(NULL);
}

teThreadStatus eThreadYield(void)
{
    CHECK_STATUS(sched_yield(), 0, E_THREAD_ERROR_FAILED);
    return E_THREAD_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
teThreadStatus eLockCreate(pthread_mutex_t *psLock)
{
    /* Create a recursive mutex, as we need to allow the same thread to lock mutexes a number of times */
    pthread_mutexattr_t     attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

    CHECK_STATUS(pthread_mutex_init(psLock, &attr), 0, E_THREAD_ERROR_FAILED);

    DBG_vPrintln(DBG_LOCKS, "Lock Create: %p\n", psLock);
    return E_THREAD_OK;
}

teThreadStatus eLockDestroy(pthread_mutex_t *psLock)
{
    pthread_mutex_destroy(psLock);
    DBG_vPrintln(DBG_LOCKS, "Lock Destroy: %p\n", psLock);
    return E_THREAD_OK;
}

teThreadStatus eLockLock(pthread_mutex_t *psLock)
{
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx locking: %p\n", pthread_self(), psLock);
    CHECK_STATUS(pthread_mutex_lock(psLock), 0, E_THREAD_ERROR_FAILED);
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx locked: %p\n", pthread_self(), psLock);
    
    return E_THREAD_OK;
}

teThreadStatus eLockLockTimed(pthread_mutex_t *psLock, uint32 u32WaitTimeout)
{    
    struct timeval sNow;
    struct timespec sTimeout;
    
    gettimeofday(&sNow, NULL);
    sTimeout.tv_sec = sNow.tv_sec + u32WaitTimeout;
    sTimeout.tv_nsec = sNow.tv_usec * 1000;
    
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx time locking: %p\n", pthread_self(), psLock);

    switch (pthread_mutex_timedlock(psLock, &sTimeout))
    {
        case (0):
            DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx: time locked: %p\n", pthread_self(), psLock);
            return E_THREAD_OK;
            break;
            
        case (ETIMEDOUT):
            DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx: time out locking: %p\n", pthread_self(), psLock);
            return E_THREAD_ERROR_TIMEOUT;
            break;

        default:
            DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx: error locking: %p\n", pthread_self(), psLock);
            return E_THREAD_ERROR_FAILED;
            break;
    }
}

teThreadStatus eLockTryLock(pthread_mutex_t *psLock)
{
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx try locking: %p\n", pthread_self(), psLock);
    CHECK_STATUS(pthread_mutex_trylock(psLock), 0, E_THREAD_ERROR_FAILED);
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx locked: %p\n", pthread_self(), psLock);
    
    return E_THREAD_OK;
}

teThreadStatus eLockunLock(pthread_mutex_t *psLock)
{    
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx unlocking: %p\n", pthread_self(), psLock);
    CHECK_STATUS(pthread_mutex_unlock(psLock), 0, E_THREAD_ERROR_FAILED);
    DBG_vPrintln(DBG_LOCKS, "Thread 0x%lx unlocked: %p\n", pthread_self(), psLock);
    
    return E_THREAD_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////
teThreadStatus eQueueCreate(tsQueue *psQueue, uint32 u32Length)
{
    psQueue->apvBuffer = malloc(sizeof(void *) * u32Length);
    memset(psQueue->apvBuffer, 0, sizeof(void *) * u32Length);
    if (!psQueue->apvBuffer){
        return E_THREAD_ERROR_NO_MEM;
    }
    
    psQueue->u32Length = u32Length;
    psQueue->u32Front = 0;
    psQueue->u32Rear = 0;
    
    CHECK_STATUS(pthread_mutex_init(&psQueue->mutex, NULL), 0, E_THREAD_ERROR_FAILED);
    CHECK_STATUS(pthread_cond_init(&psQueue->cond_space_available, NULL), 0, E_THREAD_ERROR_FAILED);
    CHECK_STATUS(pthread_cond_init(&psQueue->cond_data_available, NULL), 0, E_THREAD_ERROR_FAILED);
    
    return E_THREAD_OK;
}

teThreadStatus eQueueDestroy(tsQueue *psQueue)
{
    if (NULL == psQueue->apvBuffer){
        return E_THREAD_ERROR_FAILED;
    }
    free(psQueue->apvBuffer);
    
    eLockDestroy(&psQueue->mutex);
    pthread_cond_destroy(&psQueue->cond_space_available);
    pthread_cond_destroy(&psQueue->cond_data_available);

    return E_THREAD_OK;
}

teThreadStatus eQueueEnqueue(tsQueue *psQueue, void *pvData)
{
    eLockLock(&psQueue->mutex);
    while (((psQueue->u32Rear + 1)%psQueue->u32Length) == psQueue->u32Front){
        pthread_cond_wait(&psQueue->cond_space_available, &psQueue->mutex);
    }
    psQueue->apvBuffer[psQueue->u32Rear] = pvData;
    
    psQueue->u32Rear = (psQueue->u32Rear+1) % psQueue->u32Length;
    WAR_vPrintln(DBG_QUEUE, "mQueueEnqueue:%d done\n", psQueue->u32Rear);
    
    eLockunLock(&psQueue->mutex);
    pthread_cond_broadcast(&psQueue->cond_data_available);
    return E_THREAD_OK;
}

teThreadStatus eQueueDequeue(tsQueue *psQueue, void **ppvData)
{
    eLockLock(&psQueue->mutex);
    while (psQueue->u32Front == psQueue->u32Rear){
        pthread_cond_wait(&psQueue->cond_data_available, &psQueue->mutex);
    }
    *ppvData = psQueue->apvBuffer[psQueue->u32Front];
    
    psQueue->u32Front = (psQueue->u32Front + 1) % psQueue->u32Length;
    WAR_vPrintln(DBG_QUEUE, "mQueueDequeue:%d done\n", psQueue->u32Front);
    eLockunLock(&psQueue->mutex);
    pthread_cond_broadcast(&psQueue->cond_space_available);
    
    return E_THREAD_OK;
}

teThreadStatus eQueueDequeueTimed(tsQueue *psQueue, uint32 u32WaitTimeMil, void **ppvData)
{
    eLockLock(&psQueue->mutex);
    while (psQueue->u32Front == psQueue->u32Rear)
    {
        struct timeval sNow;
        struct timespec sTimeout;
        
        memset(&sNow, 0, sizeof(struct timeval));
        gettimeofday(&sNow, NULL);
        sTimeout.tv_sec = sNow.tv_sec + (u32WaitTimeMil/1000);
        sTimeout.tv_nsec = (sNow.tv_usec + ((u32WaitTimeMil % 1000) * 1000)) * 1000;
        if (sTimeout.tv_nsec > 1000000000)
        {
            sTimeout.tv_sec++;
            sTimeout.tv_nsec -= 1000000000;
        }
        DBG_vPrintln(DBG_QUEUE, "Dequeue timed: now    %lu s, %lu ns\n", sNow.tv_sec, sNow.tv_usec * 1000);
        DBG_vPrintln(DBG_QUEUE, "Dequeue timed: until  %lu s, %lu ns\n", sTimeout.tv_sec, sTimeout.tv_nsec);

        switch (pthread_cond_timedwait(&psQueue->cond_data_available, &psQueue->mutex, &sTimeout))
        {
            case (0):
                //Get Data signal, Copy Data next
                break;
            
            case (ETIMEDOUT):
                eLockunLock(&psQueue->mutex);
                pthread_cond_broadcast(&psQueue->cond_space_available);//Avoid queue full
                return E_THREAD_ERROR_TIMEOUT;
                break;
            
            default:
                eLockunLock(&psQueue->mutex);
                pthread_cond_broadcast(&psQueue->cond_space_available);//Avoid queue full
                return E_THREAD_ERROR_FAILED;
        }
    }
    
    *ppvData = psQueue->apvBuffer[psQueue->u32Front];
    
    psQueue->u32Front = (psQueue->u32Front + 1) % psQueue->u32Length;
    eLockunLock(&psQueue->mutex);
    pthread_cond_broadcast(&psQueue->cond_space_available);
    return E_THREAD_OK;
}

