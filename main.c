//
// Created by pct on 17-1-12.
//
#include "utils.h"
#include "light_bulb.h"
#include "bonjour.h"

int main(void)
{
    DBG_vPrintln(T_TRUE, "home kit light bulb test");
    eLightBulbProfileInit("DimmerLight", "12345678", "TopBand", "1234");
    eBonjourInit(&sLightBulb);

    while(1){
        sleep(1);
    }

    eBonjourFinished(&sLightBulb);
    eLightBulbProfileFinished();

    return 0;
}