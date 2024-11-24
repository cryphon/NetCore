#include "tuntap.h"
#include "sysimp.h"
#include "utils.h"

static int tun_fd;
static char* dev;

char* tap_addr = "10.0.0.5";
char* tap_route = "10.0.0.0/24";


// set interface address route
static int set_route(char* dev, char* cidr) {
    return cmd("ip route add dev %s %s", dev, cidr);
}

// set interface address
static int set_addr(char* dev, char* cidr) {
    return cmd("ip address add dev %s local %s", dev, cidr);
}

static int set_up(char* dev) {
    return cmd("ip link set dev %s up", dev);
    }



// from TUN/TAP docs
static int tun_alloc(char* dev, int flags) {
    struct ifreq ifr;
    int fd, err;

    if((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Opening /dev/net/tun failed");
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
        strncpy(ifr.ifr_name, dev, IFNAMSIZ - 1);
    }

    // create device
    if((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        perror("[ERROR]: ictl(TUNSETIFF) failed");
        close(fd);
        return err;
    }

    // upon success, write back device name
    strncpy(dev, ifr.ifr_name, 9);
    dev[9] = '\0';
    return fd;
}


void tun_init(void) {
    dev = calloc(10, 1);  
    tun_fd = tun_alloc(dev, IFF_TUN | IFF_NO_PI); 

    if (tun_fd < 0) {
        perror("[ERROR]: Failed to allocate TUN device");
        free(dev);
        return;
    }

    if(set_up(dev) != 0) {
        perror("[ERROR]: Interface set_up failed");
        free(dev);
        return;
    }

    if(set_route(dev, tap_route) != 0) {
        perror("[ERROR]: interface set_route failed");
        free(dev);
        return;
    }

    if(set_addr(dev, tap_addr) != 0) {
        perror("[ERROR]: interface set_addr failed");
        free(dev);
        return;
    }
}




