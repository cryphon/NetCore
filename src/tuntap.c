#include "tuntap.h"
#include "sysimp.h"
#include "utils.h"
#include "sk_buff.h"
#include "unistd.h"

static int tun_fd;
static char* dev;

char* tap_addr_v4 = "10.0.0.5";
char* tap_route_v4 = "10.0.0.0/24";
char* tap_addr_v6 = "2001:db8::1/64";
char* tap_route_v6 = "2001:db8::/64";


// set interface address route
static int set_route(char* dev, char* cidr) {
    return cmd("ip route add dev %s %s", dev, cidr);
}

// set interface address
static int set_addr(char* dev, char* cidr) {
    return cmd("ip address add dev %s local %s", dev, cidr);
}

// set interface up
static int set_up(char* dev) {
    return cmd("ip link set dev %s up", dev);
}

int tun_read(char* buff, int len) {
    return read(tun_fd, buff, len);
}

int tun_write(char* buff, int len) {
    return write(tun_fd, buff, len);
}

int tun_write_skb(struct sk_buff* skb) {
    ssize_t written;
    size_t total = 0;
    size_t length = skb->len;


    while (total < length) {
        written = write(tun_fd, skb->data + total, length - total);
        
        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            printf("Error writing to TUN device: %s\n", strerror(errno));
            return -1;
        }
        
        total += written;
    }

    printf("Successfully wrote %zu bytes\n", total);
    return total;
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
    tun_fd = tun_alloc(dev, IFF_TAP); 

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
    // IPv4 setup
    if(set_route(dev, tap_route_v4) != 0) {
        perror("[ERROR]: interface set_route V4 failed");
        free(dev);
        return;
    }

    if(set_addr(dev, tap_addr_v4) != 0) {
        perror("[ERROR]: interface set_addr V4 failed");
        free(dev);
        return;
    }

    // IPv6 setup
    if(set_route(dev, tap_route_v6) != 0) {
        perror("[ERROR]: interface set_route V6 failed");
        free(dev);
        return;
    }

    if(set_addr(dev, tap_addr_v6) != 0) {
        perror("[ERROR]: interface set_addr V6 failed");
        free(dev);
        return;
    }

}




