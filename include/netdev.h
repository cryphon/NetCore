#ifndef NETDEV_H
#define NETDEV_H
#include "sysimp.h"

#define BUFF_MAX_LEN 1600
#define MAX_ADDR_LEN 32

typedef struct netdev {      // Virutual Network Device
    char name[IFNAMSIZ];        // Name of interface
    int fd;                     // fd for device (e.g. /dev/net/tun0)
    int mtu;                    // Maximum Transmission Unit size
    uint8_t hw_addr[6];         // MAC address (if Ethernet-like)
    uint32_t ip_addr;           // IPV4 address (in network byte order)
    uint8_t ip_addr_len;        // length of ip addr
} netdev;

void netdev_init();
int netdev_transmit();
void* netdev_recvqueue_loop();



#endif // NETDEV_H

