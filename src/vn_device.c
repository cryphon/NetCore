#include "sysimp.h"
#include "vn_device.h"
#include "utils.h"

struct vn_device* loop;
struct vn_device* vn_device;

static struct vn_device* vn_device_alloc(char* addr, char* hwaddr, uint32_t mtu) {
    struct vn_device* dev = malloc(sizeof(struct vn_device));

    dev->ip_addr = parse_ip(addr); 

    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hw_addr[0],
                                                    &dev->hw_addr[1],
                                                    &dev->hw_addr[2],
                                                    &dev->hw_addr[3],
                                                    &dev->hw_addr[4],
                                                    &dev->hw_addr[5]);

    dev->ip_addr_len = 6;
    dev->mtu = mtu;
    return dev;
}

void vn_device_init() {
    loop = vn_device_alloc("127.0.0.1", "00:00:00:00:00:00", 1500);
    vn_device = vn_device_alloc("10.0.0.4", "00:0c:6d:50:25", 1500);
}

