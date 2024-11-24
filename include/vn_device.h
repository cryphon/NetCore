#ifndef VN_DEVICE_H
#define VN_DEVICE_H
#include "sysimp.h"

#define BUFF_MAX_LEN 1600
#define MAX_ADDR_LEN 32

typedef struct vn_device {      // Virutual Network Device
    char name[IFNAMSIZ];        // Name of interface
    int fd;                     // fd for device (e.g. /dev/net/tun0)
    int mtu;                    // Maximum Transmission Unit size
    uint8_t hw_addr[6];         // MAC address (if Ethernet-like)
    uint32_t ip_addr;           // IPV4 address (in network byte order)
    uint8_t ip_addr_len;        // length of ip addr
} vndev;

void vn_device_init();
int vn_device_transmit();
void* vn_device_recqueue_loop();

vndev* vndev_get(uint32_t sip);

#endif // VN_DEVICE_H

