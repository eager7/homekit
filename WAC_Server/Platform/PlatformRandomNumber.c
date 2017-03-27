/******************************************************************************
 *        File: PlatformRandomNumber.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pw_debug.h>

#include "PlatformRandomNumber.h"



// 生成 len指定长度的的随机数
static int *randSpecialLen(int len)
{
	PW_INFO("Function < randSpecialLen > \n");

	int *randNum = (int *)malloc(len);
	if (NULL == randNum)
	{
		PW_ERROR("malloc rand num failed \n");
		return NULL;
	}

	struct timeval tpstart;
	gettimeofday(&tpstart,NULL);
	srand(tpstart.tv_usec);

	int seq = 0;
	for (seq = 0; seq < len; seq++)
	{
		randNum[seq] = rand()%255;
	};

	return randNum;
}


/******************************************************************************
* Function:    PlatformCryptoStrongRandomBytes
* Description: 生成一个inByteCount指定长度的随机数，送给inBuffer指针
* 			   文档讲inBuffer指向的内存需要我自己去malloc，但是本函数被调用之前，inBuffer
* 				传过来的是一个数据，内存已经maloc好了。
* 				The buffer to fill with random bytes. inBuffer must be malloc'd by the caller
* Input:       void *inBuffer, size_t inByteCount
* Return:      OSStatus
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformCryptoStrongRandomBytes( void *inBuffer, size_t inByteCount )
{
	PW_INFO("Function < PlatformCryptoStrongRandomBytes > run \n");

#if MALLOC_BY_FUN

	inBuffer = (void *)randSpecialLen(inByteCount);
	if (NULL == inBuffer)
	{
		PW_ERROR("get rand num failed \n");
		return kUnknownErr;
	}
	return kNoErr;
#else

	void *randBuffer = (void *)randSpecialLen(inByteCount);   //获取返回指针
	if (NULL != randBuffer)
	{
		memcpy(inBuffer, randBuffer, inByteCount);            //将数据拷贝到inBuffer数组中
		free(randBuffer);
		randBuffer = NULL;
		return kNoErr;
	}

	PW_ERROR("get rand num failed \n");
	return kUnknownErr;

#endif
}

