//
// Created by pct on 17-1-12.
//
#include <signal.h>
#include <profile.h>
#include "light_bulb.h"
#include "window_covering.h"
#include "speaker.h"
#include "bonjour.h"

volatile sig_atomic_t bRunning = 1;
static void vQuitSignalHandler (int sig)
{
    DBG_vPrintln(T_TRUE, "Got signal %d", sig);
    bRunning = 0;

    return;
}

int main(void)
{
    DBG_vPrintln(T_TRUE, "home kit light bulb test");

    signal(SIGINT,  vQuitSignalHandler);/* Install signal handlers */
    signal(SIGTERM, vQuitSignalHandler);

    tsProfile *psProfileDevice = psWindowCoveringProfileInit("WindowCovering", 0x221034235134, "12345678", "TopBand", "Model1.0");
    //tsProfile *psProfileDevice = psSpeakerProfileInit("Speaker", 0x221034235134, "12345678", "TopBand", "Model1.0");
    CHECK_RESULT(eBonjourInit(psProfileDevice, "119-76-391"), E_HAP_STATUS_OK, -1);

    while(bRunning){
        sleep(1);
    }
    eBonjourFinished();
    eWindowCoveringProfileRelease(psProfileDevice);

    return 0;
}