/******************************************************************************
 *        File: PlatformBonjour.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <pw_debug.h>

#include "PlatformBonjour.h"

#define proName "mdnsd"    //要查找的进程名



/******************************************************************************
* Function:    checkProcessRunning
* Description: 检查参数给定的进程是否存在
* Input:       char *procName
* Return:      int   若存在，则返回0，否则返回-1
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static int checkProcessRunning(char *procName)
{
	PW_INFO("Function < checkProcessRunning > run \n");

	FILE *pFile;
	char cmd[128], buff[512], *p;
	int pidnum;

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, "ps -aux | grep %s | grep sbin",procName);
    pFile = popen(cmd, "r");                   //shell(ps -ef | grep xxxx)
	if (NULL == pFile)
	{
		PW_ERROR("shell (ps -ef | grep xxxx) failed \n");
		return -1;
	}

	memset(buff, 0, sizeof(buff));
	char *ret = fgets(buff, 512, pFile);
	printf("%s", ret);

	p = strtok(buff, " ");
	//p = strtok(NULL, " "); //这句是否去掉，取决于当前系统中ps后，进程ID号是否是第一个字段
	pclose(pFile);

	if((p == NULL) || strlen(p) == 0)
	{
		PW_ERROR("(p == NULL) || strlen(p) == 0 ) \n");
		return -1;
	}

	if (kill(pidnum,0) == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


/******************************************************************************
* Function:    PlatformInitializemDNSResponder
* Description:
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformInitializemDNSResponder( void )
{
	PW_INFO("Function < PlatformInitializemDNSResponder > run \n");

	int ret = checkProcessRunning(proName);
	if (0 == ret)
	{
		PW_OK("process: exist!\n");
	}
	else
	{
		PW_WARN("process: not exist!\n");
	}

	return kNoErr;
}


/******************************************************************************
* Function:    PlatformMayStopmDNSResponder
* Description:
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformMayStopmDNSResponder( void )
{
	return kNoErr;
}



