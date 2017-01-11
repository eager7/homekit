/****************************************************************************
 *
 * MODULE:             utils.h
 *
 * COMPONENT:          Utils interface
 *
 * REVISION:           $Revision:  1.0$
 *
 * DATED:              $Date: 2016-12-06 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2016. All rights reserved
 *
 ***************************************************************************/

#ifndef __H_UTILS_H_
#define __H_UTILS_H_

#if defined __cplusplus   
extern "C" {
#endif
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <syslog.h>
#include <json-c/json.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <syslog.h>

#include <sqlite3.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define mLogInitSetPid(pid)    openlog(pid, LOG_PID|LOG_CONS, LOG_USER)
#define UI_RED(x)       "\e[31;1m"x"\e[0m"
#define UI_GREEN(x)     "\e[32;1m"x"\e[0m"
#define UI_YELLOW(x)    "\e[33;1m"x"\e[0m"
#define UI_BLUE(x)      "\e[34;1m"x"\e[0m"
#define UI_PURPLE(x)    "\e[35;1m"x"\e[0m"
#define DBG_vPrintf(a,b,ARGS...)  \
    do {if (a) {if(daemonize){syslog(LOG_DEBUG|LOG_USER, "[%d]" b, __LINE__, ## ARGS);} else {printf(UI_BLUE   ("[%d]" b), __LINE__, ## ARGS);}}} while(0)
#define INF_vPrintf(a,b,ARGS...)  \
    do {if (a) {if(daemonize){syslog(LOG_DEBUG|LOG_USER, "[%d]" b, __LINE__, ## ARGS);} else {printf(UI_YELLOW ("[%d]" b), __LINE__, ## ARGS);}}} while(0)
#define NOT_vPrintf(a,b,ARGS...)  \
    do {if (a) {if(daemonize){syslog(LOG_DEBUG|LOG_USER, "[%d]" b, __LINE__, ## ARGS);} else {printf(UI_GREEN  ("[%d]" b), __LINE__, ## ARGS);}}} while(0)
#define WAR_vPrintf(a,b,ARGS...)  \
    do {if (a) {if(daemonize){syslog(LOG_DEBUG|LOG_USER, "[%d]" b, __LINE__, ## ARGS);} else {printf(UI_PURPLE ("[%d]" b), __LINE__, ## ARGS);}}} while(0)
#define ERR_vPrintf(a,b,ARGS...)  \
    do {if (a) {if(daemonize){syslog(LOG_DEBUG|LOG_USER, "[%d]" b, __LINE__, ## ARGS);} else {printf(UI_RED    ("[%d]" b), __LINE__, ## ARGS);}}} while(0)
#define PERR_vPrintf(x) ERR_vPrintf(1,x ":%s\n", strerror(errno))

#define MIBF    256
#define MDBF    1024
#define MABF    2048
#define PACKED __attribute__((__packed__))

#define CHECK_RESULT(fun,value,ret) do{ if(value!=fun)return ret;}while(0)
#define CHECK_STATUS(fun,value,ret) do{ if(value!=fun){ERR_vPrintf(T_TRUE, "Error:%s\n", strerror(errno));return ret;}}while(0)
#define CHECK_POINTER(value,ret) do{ if(value==NULL){ERR_vPrintf(T_TRUE, "Pointer is NULL\n");return ret;}}while(0)
#define FREE(p) do{ if(p){free(p); p=NULL;} }while(0)
#define CALL(f, x) do{ if(f){ f(x); } }while(0)

#define Swap64(ll) (((ll) >> 56) |                          \
                    (((ll) & 0x00ff000000000000) >> 40) |   \
                    (((ll) & 0x0000ff0000000000) >> 24) |   \
                    (((ll) & 0x000000ff00000000) >> 8)  |   \
                    (((ll) & 0x00000000ff000000) << 8)  |   \
                    (((ll) & 0x0000000000ff0000) << 24) |   \
                    (((ll) & 0x000000000000ff00) << 40) |   \
                    (((ll) << 56)))  


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef enum
{
    T_FALSE = 0,
    T_TRUE  = 1,    
} bool_t;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern int daemonize;
extern int verbosity;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*__H_UTILS_H_*/
