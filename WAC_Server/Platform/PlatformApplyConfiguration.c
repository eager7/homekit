/******************************************************************************
 *        File: PlatformApplyConfiguration.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pw_debug.h>

#include "PlatformApplyConfiguration.h"




/******************************************************************************
* Function:    PlatformJoinDestinationWiFiNetwork
* Description:
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformJoinDestinationWiFiNetwork( const char * const inSSID,
											 const uint8_t * const inWiFiPSK,
											 size_t inWiFiPSKLen )
{
	PW_INFO("Function < PlatformJoinDestinationWiFiNetwork > run \n");
	size_t loop = 0;

	PW_LOG("=======================================================");
	PW_LOG("inSSID = %s \n", inSSID);

	PW_LOG("PASSWORD = ");
	for (loop = 0; loop < inWiFiPSKLen; loop++)
	{
		printf("%d ", inWiFiPSK[loop]);
	}
	printf("\n");
	PW_LOG("=======================================================");

	return kNoErr;
}


/******************************************************************************
* Function:    PlatformApplyName
* Description:
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformApplyName( const char * const inName )
{
	PW_INFO("Function < PlatformApplyName > run \n");
	PW_LOG("inName = %s \n", inName);

	return kNoErr;
}


/******************************************************************************
* Function:    PlatformApplyName
* Description:
* Input:
* Return:      OSStatus
* Others:
*       Author          Date                        Purpose
*----------------------------------------------------------------------------
*       patrick         20150109                    comments
******************************************************************************/
OSStatus PlatformApplyAirPlayPlayPassword( const char * const inPlayPassword )
{
	PW_INFO("Function < PlatformApplyAirPlayPlayPassword > run \n");
	PW_LOG("inPlayPassword = %s \n", inPlayPassword);

	return kNoErr;
}




