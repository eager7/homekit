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
    CHECK_STATUS(eBonjourInit(&sLightBulb, "523-12-643"), E_BONJOUR_STATUS_OK, -1);
    

    while(1){
        sleep(1);
    }

    eBonjourFinished(&sLightBulb);
    eLightBulbProfileFinished();

    return 0;
}