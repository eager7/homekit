/******************************************************************************
 *        File: PlatformSoftwareAccessPoint.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pw_debug.h>

#include "PlatformSoftwareAccessPoint.h"


static void addIeToBeacon( const uint8_t *inIE, size_t inIELen )
{
	(void)inIE;
	(void)inIELen;
	PW_INFO("Function < addIeToBeacon > run \n");
}




/******************************************************************************
* Function:    PlatformSoftwareAccessPointStart
* Description: This function does not have to block until the Software Access Point
* 				is fully initialized. The platform should also start the DHCP server
* 				at this time
* Input:       inIE,
* 			   The information element created by the WAC engine that needs to be
*              added to the platforms software access point's beacon information element.
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformSoftwareAccessPointStart( const uint8_t *inIE, size_t inIELen )
{
	PW_INFO("Function < PlatformSoftwareAccessPointStart > run \n");
	unsigned int i = 0;
	printf("inIE = ");
	for (i = 0; i < inIELen; i++)
	{
		printf("%c", inIE[i]);
	}
	printf("\n");

#if 0
	system("sudo ifconfig wlan0 down");
	system("sudo iwconfig wlan0 mode ad-hoc");
	system("sudo iwconfig wlan0 essid patrick_ap");
	system("sudo iwconfig wlan0 channel 11");
	system("sudo iwconfig wlan0 key 123456789");
	system("sudo ifconfig wlan0 up");
#endif

	int ret = system("killall easyconfigd");
	ret = system("sh -x sbin/ap_sta_switch.sh ap");
	printf("---------------------------------------- switch ap:%d\n", ret);

	addIeToBeacon(inIE, inIELen);


	return kNoErr;
}


/******************************************************************************
* Function:    PlatformSoftwareAccessPointStop
* Description: This function does not have to block until the Software Access Point is fully initialized
* 				The platform should also stop the DHCP server at this time.
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformSoftwareAccessPointStop(void)
{
	PW_INFO("Function < PlatformSoftwareAccessPointStop > run \n");

#if 0
	system("sudo ifconfig wlan0 down");
	system("sudo iwconfig wlan0 mode Managed");
	system("sudo ifconfig wlan0 up");
#endif

	int ret = system("killall udhcpd");
	ret = system("sh -x sbin/ap_sta_switch.sh sta");
	printf("---------------------------------------- switch sta:%d \n", ret);

	return kNoErr;
}


