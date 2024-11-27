#include "sysimp.h"

#define DEBUG_ETH
#include "vn_device.h"
#include "tuntap.h"
#include "utils.h"
#include "sk_buff.h"
#include "ethernet.h"

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

static int vn_device_recv(struct sk_buff* skb) {
    struct eth_hdr* hdr = eth_hdr(skb);
}

void* vn_device_recqueue_loop() {
    struct sk_buff* skb = skb_alloc(BUFF_MAX_LEN);

    printf("listening on device %s\n", vn_device->name);
    while (1) {
        if(tun_read((char*)skb->data, BUFF_MAX_LEN) < 0) {
            perror("Error reading from TUN device");
            free(skb);
            continue;
        }

        // assume first two bits are padded
        skb->data += 2;

        struct eth_hdr* hdr = (struct eth_hdr*) skb->data;
        eth_dbg("Received frame", hdr);

        // Additional processing
    }
}

