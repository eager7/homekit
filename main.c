//
// Created by pct on 17-1-12.
//
#include <signal.h>
#include "utils.h"
#include "light_bulb.h"
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

    eLightBulbProfileInit("DimmerLight", "12345678", "TopBand", "1234");
    CHECK_RESULT(eBonjourInit(&sLightBulb, "119-76-391"), E_BONJOUR_STATUS_OK, -1);

    while(bRunning){
        sleep(1);
    }

    eBonjourFinished(&sLightBulb);
    eLightBulbProfileFinished();

    return 0;
}