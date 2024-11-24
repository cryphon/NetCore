#include "tuntap.h"
#include "sysimp.h"

static int tun_fd;
static char* dev;

char* tap_addr = "10.0.0.5";
char* tap_route = "10.0.0.0/24";



// from TUN/TAP docs
static int tun_alloc(char* dev, int flags) {
    struct ifreq ifr;
    int fd, err;

    if((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Opening /dev/net/tun");
        return fd;
    }

    // clear the ifreq struct
    memset(&ifr, 0, sizeof(ifr));

    /*  Flags:
     *      IFF_TUN     - TUN device (no Ethernet headers)
     *      IFF_TAP     - TAP device
     *      IFF_NO_PI   - no packet info
     */

    ifr.ifr_flags = IFF_TUN;
    if(*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    // create device
    if((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        perror("ictl(TUNSETIFF)");
        close(fd);
        return err;
    }

    // upon success, write back device name
    strcpy(dev, ifr.ifr_name);
    return fd;
}


void tun_init(void) {
    dev = calloc(10, 1);
    tun_fd = tun_alloc(dev, IFF_TUN | IFF_NO_PI); 
}




