/******************************************************************************
 *        File:WACMain.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *       other: make platform_makefile=Platform/Platform.include.mk
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "pw_debug.h"
#include "WACServerAPI.h"
#include "utils.h"

#define IS_UNCONFIGURED      true
#define SUPPORT_AIRPLAY      true
#define SUPPORT_AIRPRINT     false
#define SUPPORT_WIFI_24      true
#define SUPPORT_WIFI_5       false
#define SUP_WAKEON_WIRELESS  false
#define FIRMWARE_REVISION    "1.0"
#define HARDWARE_REVISION    "1.0"
#define SERIAL_NUM           "000001"
#define DEV_NAME             "PATRICK_WAC"
#define MODEL_NAME           "PATRICK_WAC_MODEL"
#define MANUFACTURER_NAME    "TONLY"
#define BUNDLE_SEED_ID       "C6P64J2MZX"
#define EA_PROTOCOL          "com.issc.datapath"

#define NIC "wlp2s0"
const char *eaProtocols[1] = {EA_PROTOCOL};

/******************************************************************************
* Function:    getHostMAC
* Description: 获取device的mac地址
* Input:       void
* Return:      char * 指向mac地址的字符串
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static int getHostMAC(uint8_t *mac_addr)
{
	int sock_mac;
	struct ifreq ifr_mac;

	if (NULL == mac_addr)
	{
		PW_ERROR("mac_addr = NULL");
		return -1;
	}

	sock_mac = socket( AF_INET, SOCK_STREAM, 0 );
	if (-1 == sock_mac)
	{
		PW_ERROR("create socket failed ...mac\n");
		return -1;
	}

	memset(&ifr_mac, 0, sizeof(ifr_mac));
	strncpy(ifr_mac.ifr_name, NIC, sizeof(ifr_mac.ifr_name)-1);

	if ((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
	{
		PW_ERROR("mac ioctl error\n");
		close(sock_mac);
		return -1;
	}

	mac_addr[0] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0];
	mac_addr[1] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1];
	mac_addr[2] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2];
	mac_addr[3] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3];
	mac_addr[4] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4];
	mac_addr[5] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5];
	close(sock_mac);

	int i = 0;
	printf("local mac: ");
	for (i = 0; i < 6; i++)
	{
		printf("%02X ", mac_addr[i]);
	}
	printf("\n");

	return 0;
}

/******************************************************************************
* Function:    initPlatformParam
* Description: 初始化WACPlatformParameters_t结构体，提供WAC运行参数
* Input:       void
* Return:      WACPlatformParameters_t *
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static WACPlatformParameters_t *initPlatformParam(void)
{
	DBG_vPrintln(T_TRUE, "Function < initPlatformParam > run \n");

	WACPlatformParameters_t *platformParams = malloc(sizeof (WACPlatformParameters_t));
	if (!platformParams)
	{
		PW_ERROR("malloc platformParams failed \n");
		return NULL;
	}

	int ret = getHostMAC(platformParams->macAddress);    //填充device mac地址
	if (0 != ret)
	{
		PW_ERROR("get Host MAC failed \n");
        FREE(platformParams);
		return NULL;
	}

	platformParams->isUnconfigured = T_TRUE;
	platformParams->supportsAirPlay = T_FALSE;
	platformParams->supportsAirPrint = T_FALSE;
	platformParams->supports2_4GHzWiFi = T_TRUE;
	platformParams->supports5GHzWiFi = T_TRUE;
	platformParams->supportsWakeOnWireless = T_TRUE;

	platformParams->firmwareRevision = FIRMWARE_REVISION;
	platformParams->hardwareRevision = HARDWARE_REVISION;
	platformParams->serialNumber = SERIAL_NUM;

	platformParams->name = DEV_NAME;
	platformParams->model = MODEL_NAME;
	platformParams->manufacturer = MANUFACTURER_NAME;

	platformParams->supportedExternalAccessoryProtocols = (char **)eaProtocols;;
	platformParams->numSupportedExternalAccessoryProtocols = 1;
	platformParams->preferredAppBundleSeedIdentifier = BUNDLE_SEED_ID;

	return platformParams;
}

/******************************************************************************
* Function:    initWACContext
* Description: 初始化WACContext_t结构体，提供给WAC运行参数
* Input:       void
* Return:      WACContext_t *
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static WACContext_t *initWACContext(void)
{
	DBG_vPrintln(T_TRUE, "Function < initWACContext > run \n");

	WACContext_t *context = malloc(sizeof (WACContext_t));
	if (!context) {
		PW_ERROR("malloc WACContext_t failed \n");
		return NULL;
	}

	context->httpServer = NULL;
	context->httpHeader = NULL;
	context->mfiSAPRef = NULL;
	context->dnsServiceRef = NULL;
	context->callback = NULL;

	context->platformParams = initPlatformParam(); //初始化WACPlatformParameters_t这个结构体
	if (NULL == context->platformParams) {
		PW_ERROR("init PlatformParam failed \n");
        FREE (context);
		return NULL;
	}

	return context;
}

/******************************************************************************
* Function:    destroyWACContext
* Description: 初始化WACContext_t结构体，提供给WAC运行参数
* Input:       WACContext_t *context  需要销毁的context
* Return:      WACContext_t *
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
static int destroyWACContext(WACContext_t *context)
{
	PW_INFO("Function < initWACContext > run \n");

	if (context)
	{
		if (context->platformParams)
		{
			free (context->platformParams);
			context->platformParams = NULL;
		}
		free (context);
		context = NULL;
		return 0;
	}

	PW_ERROR("WACContext_t pointer is null \n");
	return -1;
}

static void WACCallback( WACContext_t *context, WACCallbackMessage_t message )
{
	DBG_vPrintln(T_TRUE, "Function < WACPlatformCallback > run \n");
	(void)context;
	switch (message)
	{
	case WACCallbackMessage_Initializing:
        DBG_vPrintln(T_TRUE, "WACCallbackMessage_Initializing \n");
		break;

	case WACCallbackMessage_Ready:
        DBG_vPrintln(T_TRUE, "WACCallbackMessage_Ready \n");
		break;

	case WACCallbackMessage_ConfigStart:
        DBG_vPrintln(T_TRUE, "WACCallbackMessage_ConfigStart \n");
		break;

	case WACCallbackMessage_ConfigReceived:
        DBG_vPrintln(T_TRUE, "WACCallbackMessage_ConfigReceived \n");
		break;

	case WACCallbackMessage_ConfigComplete:
        DBG_vPrintln(T_TRUE, "WACCallbackMessage_ConfigComplete \n");
		break;

	case WACCallbackMessage_Stopped:
        WAR_vPrintln(T_TRUE, "WACCallbackMessage_Stopped \n");
		break;

	case WACCallbackMessage_Error:
        ERR_vPrintln(T_TRUE, "WACCallbackMessage_Error \n");
		break;

	default:
        DBG_vPrintln(T_TRUE, "default \n");
		break;
	}
}

/******************************************************************************
* Function:    main
* Description: 主苹果WAC进程启动函数
* Input:       void 后期需要改成argc argv标准输入函数
* Return:      int
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
int main (void)
{
	DBG_vPrintln(T_TRUE, "Apple WAC run ......\n");

	WACContext_t *inContext = NULL;
	inContext = initWACContext();;
	if (NULL == inContext) {
		PW_ERROR("init inContext failed \n");
		assert(inContext);
	}
	//启动WAC
	WACServerStart( inContext, WACCallback);

	//停止，等待用户操作
	pause();

	//停止WAC
	WACServerStop( inContext );
    destroyWACContext(inContext);

	PW_OK("game over \n");
}




