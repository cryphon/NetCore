#include "sysimp.h"
#include "tuntap.h"
#include "vn_device.h"

#define BUFLEN 2048

int main(void) {

    tun_init();
    vn_device_init();

    return 0;
}

