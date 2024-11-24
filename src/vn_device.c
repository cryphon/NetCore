#include "sysimp.h"
#include "vn_device.h"
#include "tuntap.h"
#include "utils.h"

struct vn_device* loop;
struct vn_device* vn_device;

static struct vn_device* vn_device_alloc(char* name, char* addr, char* hwaddr, uint32_t mtu) {
    struct vn_device* dev = malloc(sizeof(struct vn_device));
    strncpy(dev->name, name, 15);
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
    loop = vn_device_alloc("loop", "127.0.0.1", "00:00:00:00:00:00", 1500);
    vn_device = vn_device_alloc("testtest", "10.0.0.4", "00:0c:6d:50:25", 1500);
}

void* vn_device_recqueue_loop() {
    char buffer[2048];

    printf("listening on device %s\n", vn_device->name);
    while (1) {
        int len = tun_read(buffer, sizeof(buffer));
        if (len < 0) {
            perror("Error reading from TUN device");
            continue;
        }

        printf("Received packet\n");
    }
    return NULL;}

