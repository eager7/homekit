/******************************************************************************
 *        File: PlatformMFiAuth.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pw_debug.h>

#include "rsakey.h"

#include "PlatformMFiAuth.h"


rsakey_t *rsakey;


/******************************************************************************
* Function:    utils_read_file
* Description: 打开filename，从filename中读出字符串，送给dst
* Input:       char **dst, const char *filename
* Return:      int 失败返回-1
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static int utils_read_file(char **dst, const char *filename)
{
	FILE *stream;
	int filesize;
	char *buffer;
	int read_bytes;

	/* Open stream for reading */
	stream = fopen(filename, "rb");
	if (!stream) {
		return -1;
	}

	/* Find out file size */
	fseek(stream, 0, SEEK_END);
	filesize = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	/* Allocate one extra byte for zero */
	buffer = malloc(filesize+1);
	if (!buffer) {
		fclose(stream);
		return -2;
	}

	/* Read data in a loop to buffer */
	read_bytes = 0;
	do {
		int ret = fread(buffer+read_bytes, 1,
		                filesize-read_bytes, stream);
		if (ret == 0) {
			break;
		}
		read_bytes += ret;
	} while (read_bytes < filesize);

	/* Add final null byte and close stream */
	buffer[read_bytes] = '\0';
	fclose(stream);

	/* If read didn't finish, return error */
	if (read_bytes != filesize) {
		free(buffer);
		return -3;
	}

	/* Return buffer */
	*dst = buffer;
	return filesize;
}


/******************************************************************************
* Function:    PlatformMFiAuthInitialize
* Description: Performs any platform-specific initialization needed.
* 				MFI初始化，例如重启或复位I2C芯片
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformMFiAuthInitialize(void)
{
	PW_INFO("Function < PlatformMFiAuthInitialize > run \n");

	OSStatus status = kNoErr;
	char *pemstr = NULL;

	if (utils_read_file(&pemstr, "./airport.key") < 0)
	{
		PW_ERROR("read airport.key failed! \n");
		status = kUnknownErr;
		goto exit;
	}

	/* Initialize RSA key handler */
	rsakey = rsakey_init_pem(pemstr);
	if (!rsakey)
	{
		PW_ERROR("rsakey_init_pem failed! \n");
		status = kUnknownErr;
	}

	if (pemstr)
	{
		free(pemstr);
		pemstr = NULL;
	}
exit:
	return status;
}


/******************************************************************************
* Function:    PlatformMFiAuthFinalize
* Description: Performs any platform-specific cleanup needed.
* 			   MFI初始化，例如关闭I2C或者禁止CP芯片
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
void PlatformMFiAuthFinalize( void )
{
	PW_INFO("Function < PlatformMFiAuthFinalize > run \n");

	if (rsakey)
	{
		rsakey_destroy(rsakey);
		rsakey = NULL;
	}

	return ;
}


/******************************************************************************
* Function:    PlatformMFiAuthCreateSignature
* Description: Create an RSA signature from the specified SHA-1 digest
* 				using the Apple Authentication Coprocessor
* 				通过指定的SHA-1，使用苹果鉴权芯片创建一个RSA签名
* Input:    const void *inDigestPtr：
* 				Pointer to 20-byte SHA-1 digest.
*           size_t     inDigestLen：
*               Number of bytes in the digest. Must be 20.
*           uint8_t    **outSignaturePtr：
*               Receives malloc()'d ptr to RSA signature. Caller must free() on success
*           size_t     *outSignatureLen：
*               Receives number of bytes in RSA signature.
*
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformMFiAuthCreateSignature( const void *inDigestPtr,
                                         size_t     inDigestLen,
                                         uint8_t    **outSignaturePtr,
                                         size_t     *outSignatureLen )
{
	PW_INFO("Function < PlatformMFiAuthCreateSignature > run \n");

	//PW_LOG("inDigestPtr = %p \n", inDigestPtr);
	//PW_LOG("inDigestLen = %d \n", inDigestLen);
	//PW_LOG("outSignaturePtr = %d \n", (int)outSignaturePtr);
	//PW_LOG("outSignatureLen = %d \n", (int)outSignatureLen);
	(void)inDigestPtr;
	(void)inDigestLen;
	(void)outSignaturePtr;
	(void)outSignatureLen;

	return kNoErr;
}


/******************************************************************************
* Function:    PlatformMFiAuthCopyCertificate
* Description: Copy the certificate from the Apple Authentication Coprocessor.
*              从苹果鉴权芯片中复制信息
* Input:       uint8_t **outCertificatePtr,   size_t *outCertificateLen
* 				三
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformMFiAuthCopyCertificate( uint8_t **outCertificatePtr,
		                                 size_t *outCertificateLen )
{
	PW_INFO("Function < PlatformMFiAuthCopyCertificate > run \n");

	//PW_LOG("outCertificatePtr = %d \n", (int)outCertificatePtr);
	//PW_LOG("outCertificateLen = %d \n", (int)outCertificateLen);
	(void)outCertificatePtr;
	(void)outCertificateLen;

	return kNoErr;
}


