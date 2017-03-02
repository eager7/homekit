//
// Created by pct on 17-1-12.
//
#include <signal.h>
#include <profile.h>
#include "light_bulb.h"
#include "bonjour.h"

volatile sig_atomic_t bRunning = 1;
static void vQuitSignalHandler (int sig)
{
    DBG_vPrintln(T_TRUE, "Got signal %d", sig);
    bRunning = 0;

    return;
}

#define NAME "Dimmr Light"
int main(void)
{
    DBG_vPrintln(T_TRUE, "home kit light bulb test");

    signal(SIGINT,  vQuitSignalHandler);/* Install signal handlers */
    signal(SIGTERM, vQuitSignalHandler);

    tsProfile *psLightBulb = psLightBulbProfileInit(NAME, 0x221034235132, "12345678", "TopBand", "Light1.1");
    CHECK_RESULT(eBonjourInit(psLightBulb, "119-76-391", NAME), E_BONJOUR_STATUS_OK, -1);

    while(bRunning){
        sleep(1);
    }
    eBonjourFinished(psLightBulb);
    eLightBulbProfileRelease(psLightBulb);

    return 0;
}