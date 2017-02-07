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
#include <openssl/sha.h>
//#include <sqlite3.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG
#define LOG DEBUG
#else
#define LOG 0
#endif

#define mLogInitSetPid(pid)    openlog(pid, LOG_PID|LOG_CONS, LOG_USER)
#define UI_RED(x)       "\e[31;1m"x"\e[0m"
#define UI_GREEN(x)     "\e[32;1m"x"\e[0m"
#define UI_YELLOW(x)    "\e[33;1m"x"\e[0m"
#define UI_BLUE(x)      "\e[34;1m"x"\e[0m"
#define UI_PURPLE(x)    "\e[35;1m"x"\e[0m"
#define DBG_vPrintln(a,b,ARGS...)  \
    do {if (a) {printf(UI_BLUE   ("[%d]" b "\n"), __LINE__, ## ARGS);}} while(0)
#define INF_vPrintln(a,b,ARGS...)  \
    do {if (a) {if(0){syslog(LOG_DEBUG|LOG_USER, "[%d]" b "\n", __LINE__, ## ARGS);} else {printf(UI_YELLOW ("[%d]" b "\n"), __LINE__, ## ARGS);}}} while(0)
#define NOT_vPrintln(a,b,ARGS...)  \
    do {if (a) {if(0){syslog(LOG_DEBUG|LOG_USER, "[%d]" b "\n", __LINE__, ## ARGS);} else {printf(UI_GREEN  ("[%d]" b "\n"), __LINE__, ## ARGS);}}} while(0)
#define WAR_vPrintln(a,b,ARGS...)  \
    do {if (a) {if(0){syslog(LOG_DEBUG|LOG_USER, "[%d]" b "\n", __LINE__, ## ARGS);} else {printf(UI_PURPLE ("[%d]" b "\n"), __LINE__, ## ARGS);}}} while(0)
#define ERR_vPrintln(a,b,ARGS...)  \
    do {if (a) {if(0){syslog(LOG_DEBUG|LOG_USER, "[%d]" b "\n", __LINE__, ## ARGS);} else {printf(UI_RED    ("[%d]" b "\n"), __LINE__, ## ARGS);}}} while(0)
#define PERR_vPrintln(x) ERR_vPrintf(1,x ":%s\n", strerror(errno))

#define MIBF    256
#define MDBF    1024
#define MABF    2048
#define PACKED __attribute__((__packed__))

#define CHECK_RESULT(fun,value,ret) do{ if(value!=fun)return ret;}while(0)
#define CHECK_STATUS(fun,value,ret) do{ if(value!=fun){ERR_vPrintln(T_TRUE, "Error:%s", strerror(errno));return ret;}}while(0)
#define CHECK_POINTER(value,ret) do{ if(value==NULL){ERR_vPrintln(T_TRUE, "Pointer is NULL");return ret;}}while(0)
#define FREE(p) do{ if(p){free(p); p=NULL;} }while(0)

#define PrintArray(a, x, len) do{if(a){for (int j = 0; j < len; ++j) {printf("0x%02x,", x[j]);}printf("\n");}}while(0)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef long long           int64;

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
