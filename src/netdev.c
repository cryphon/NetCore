#include "sysimp.h"

#define DEBUG_ETH
#include "netdev.h"
#include "tuntap.h"
#include "utils.h"
#include "sk_buff.h"
#include "ethernet.h"
#include "ipv6.h"
#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct netdev* loop;
struct netdev* device;

static struct netdev* netdev_alloc(char* name, char* addr, char* hwaddr, uint32_t mtu) {
    struct netdev* dev = malloc(sizeof(struct netdev));
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

void netdev_init() {
    loop = netdev_alloc("loop", "127.0.0.1", "00:00:00:00:00:00", 1500);
    device = netdev_alloc("testtest", "10.0.0.4", "00:0c:6d:50:25", 1500);
}

static int netdev_recv(struct sk_buff* skb) {
    struct eth_hdr* hdr = eth_hdr(skb);

    switch(hdr->ether_type) {
        case 0x86DD:
            // IPv6
            ipv6_recv(skb);
            break;
        default:
            printf("default behaviour\n");
    }
    return 0;

}

void* netdev_recvqueue_loop() {
    struct sk_buff* skb = skb_alloc(BUFF_MAX_LEN);
    printf("Listening on device %s\n", device->name);

    while (1) {
        memset(skb->data, 0, BUFF_MAX_LEN);

        int bytes_read = tun_read((char*)skb->data, BUFF_MAX_LEN);
        if (bytes_read < 0) {
            perror("Error reading from TUN device");
            continue;
        }

        printf("\nRaw packet data (first 64 bytes):\n");
        for (int i = 0; i < MIN(bytes_read, 64); i++) {
            if (i % 16 == 0) printf("%04x: ", i);
            printf("%02x ", skb->data[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");

        netdev_recv(skb);

    }
}
