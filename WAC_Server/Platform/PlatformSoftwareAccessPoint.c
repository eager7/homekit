/******************************************************************************
 *        File: PlatformSoftwareAccessPoint.c
 * Description:
 *  Created on: 2015-1-9
 *      Author: patrick
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pw_debug.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netpacket/packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <netinet/in.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/utsname.h>
#include "PlatformSoftwareAccessPoint.h"


static int addIeToBeacon( const uint8_t *inIE, size_t inIELen )
{
	(void)inIE;
	(void)inIELen;
	PW_INFO("Function < addIeToBeacon > run \n");

    int fd;
    struct ifreq ifr;
    struct sockaddr_ll ll;
    if( ( fd = socket( PF_PACKET, SOCK_RAW,	htons( ETH_P_ALL ) ) ) < 0 )
    {
        perror( "socket(PF_PACKET) failed" );
        if( getuid() != 0 )
            fprintf( stderr, "This program requires root privileges.\n" );
        return 0;
    }
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, "wlp2s0", sizeof(ifr.ifr_name));
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
        perror( "ioctl(SIOCGIFINDEX) failed" );
        close(fd);
        printf("ioctl(SIOCGIFINDEX) failed\n");
        return 0;
    }
    memset(&ll, 0, sizeof(ll));
    ll.sll_family = PF_PACKET;
    ll.sll_ifindex = ifr.ifr_ifindex;
    ll.sll_protocol = htons(ETH_P_ALL);
    if (bind(fd, (struct sockaddr *) &ll, sizeof(ll)) < 0) {
        perror( "bind[PF_PACKET] failed" );
        close(fd);
        printf("bind[PF_PACKET] failed\n");
        return 0;
    }
    unsigned long has = 1;
    ioctl(fd, FIONBIO , &has);

    int retval;
    fd_set rfds;
    struct timeval tv;
    int needswitch;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 2000/1000;
    tv.tv_usec = 0;

    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (FD_ISSET(fd, &rfds))
    {
        char buf[2048] = {0};
        read(fd, buf, sizeof(buf));
        for (int i = 0; i < 2048; ++i) {
            printf("%02x,", buf[i]);
        }
        return retval;
    }else{
        printf("-------------------------------------\n");
    }

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
		printf("0x%02x,", inIE[i]);
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

	//int ret = system("killall easyconfigd");
	//ret = system("sh -x sbin/ap_sta_switch.sh ap");
	//printf("---------------------------------------- switch ap:%d\n", ret);

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

	//int ret = system("killall udhcpd");
	//ret = system("sh -x sbin/ap_sta_switch.sh sta");
	//printf("---------------------------------------- switch sta:%d \n", ret);

	return kNoErr;
}


