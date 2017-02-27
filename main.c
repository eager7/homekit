//
// Created by pct on 17-1-12.
//
#include <signal.h>
#include <profile.h>
#include "light_bulb.h"
#include "bonjour.h"
#include "poly1305.h"

volatile sig_atomic_t bRunning = 1;
static void vQuitSignalHandler (int sig)
{
    DBG_vPrintln(T_TRUE, "Got signal %d", sig);
    bRunning = 0;

    return;
}
#if 0
typedef enum{
    true = 1,
    false = 0,
}bool;
bool poly1305_verify(const unsigned char *expected, const unsigned char *mac) {
    for (int i = 0; i < 16; i++) {
        if (expected[i] != mac[i])
            return false;
    }
    return true;
}

void
poly1305_auth(unsigned char mac[16], const unsigned char *m, size_t bytes, const unsigned char key[32]) {
    poly1305_context ctx;
    poly1305_init(&ctx, key);
    poly1305_update(&ctx, m, bytes);
    poly1305_finish(&ctx, mac);
}

int poly1305_power_on_self_test() {
    const unsigned char expected[16] = {0xdd,0xb9,0xda,0x7d,0xdd,0x5e,0x52,0x79,0x27,0x30,0xed,0x5c,0xda,0x5f,0x90,0xa4};
    unsigned char key[32];
    unsigned char mac[16];
    unsigned char msg[73];
    size_t i;

    for (i = 0; i < sizeof(key); i++)
        key[i] = (unsigned char)(i + 221);
    for (i = 0; i < sizeof(msg); i++)
        msg[i] = (unsigned char)(i + 121);
    poly1305_auth(mac, msg, sizeof(msg), key);

    printf("sample mac is ");
    for (i = 0; i < sizeof(mac); i++)
        printf("%02x", mac[i]);
    printf(" (%s)\n", poly1305_verify(expected, mac) ? "correct" : "incorrect");
}
#endif

#define NAME "Dimmer Light"
int main(void)
{
    DBG_vPrintln(T_TRUE, "home kit light bulb test");
    //printf("power on: %d\n", poly1305_power_on_self_test());

    signal(SIGINT,  vQuitSignalHandler);/* Install signal handlers */
    signal(SIGTERM, vQuitSignalHandler);

    tsProfile *psLightBulb = psLightBulbProfileInit(NAME, 0x221034235129, "12345678", "TopBand", "Light");
    CHECK_RESULT(eBonjourInit(psLightBulb, "119-76-391", NAME), E_BONJOUR_STATUS_OK, -1);

    while(bRunning){
        sleep(1);
    }
    eBonjourFinished(psLightBulb);
    eLightBulbProfileRelease(psLightBulb);

    return 0;
}