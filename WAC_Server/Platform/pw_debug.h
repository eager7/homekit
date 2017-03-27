/******************************************************************************
File Name   : Debug.h
Description : 调试打印信息宏文件
COPYRIGHT (C) TCL

  Date               Modification                    Name
-----------------------------------------------------------------------------
  20120221           Created                         Patrick
******************************************************************************/

#ifndef DEBUG_H_
#define DEBUG_H_

//#include <QDebug>

//#define __cplusplus
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/*--------------------------------*/
#define UI_RED(x)       "\e[31;1m"x"\e[0m"
#define UI_GREEN(x)     "\e[32;1m"x"\e[0m"
#define UI_YELLOW(x)    "\e[33;1m"x"\e[0m"
#define UI_BLUE(x)      "\e[34;1m"x"\e[0m"
#define UI_PURPLE(x)    "\e[35;1m"x"\e[0m"

#define DBG_OK   "["DBG_GREEN("  OK  ")"]"
#define DBG_FAILED "["DBG_RED("Failed")"]"
/*****************************************************************************/
/*****************************************************************************/
#define PW_LOG(b, ARGS...) \
			do {printf(("\e[34;1m""[%d]" b "\n""\e[0m"), __LINE__, ## ARGS);} while(0)

#define PW_INFO(b, ARGS...) \
			do {printf(("\e[33;1m""[%d]" b "\n""\e[0m"), __LINE__, ## ARGS);} while(0)

#define PW_ERROR(b, ARGS...) \
			do {printf(("\e[31;1m""[%d]" b "\n""\e[0m"), __LINE__, ## ARGS);} while(0)

#define PW_WARN(b, ARGS...) \
			do {printf(("\e[35;1m""[%d]" b "\n""\e[0m"), __LINE__, ## ARGS);} while(0)

#define PW_OK(b, ARGS...) \
			do {printf(("\e[32;1m""[%d]" b "\n""\e[0m"), __LINE__, ## ARGS);}while(0)

#ifdef __cplusplus   //
}
#endif  /* __cplusplus */

#endif
