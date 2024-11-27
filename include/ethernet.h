#ifndef ETHERNET_H
#define ETHERNET_H

#include "sysimp.h"

#ifdef DEBUG_ETH
#define eth_dbg(msg, hdr)                                               \
    do {                                                                \
        print_debug("eth "msg" ("                                       \
                    "dmac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, " \
                    "smac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, " \
                    "ethertype: %.4hx)",                               \
                    hdr->dmac[0], hdr->dmac[1], hdr->dmac[2], hdr->dmac[3], \
                    hdr->dmac[4], hdr->dmac[5], hdr->smac[0], hdr->smac[1], \
                    hdr->smac[2], hdr->smac[3], hdr->smac[4], hdr->smac[5], hdr->ether_type); \
    } while (0)
#else
#define eth_dbg(msg, hdr)
#endif

struct eth_hdr {
    uint16_t ether_type;
    unsigned char dmac[6];
    unsigned char smac[6];
    unsigned char payload[];
} __attribute__((packed));

static inline struct eth_hdr* eth_hdr(struct sk_buff* skb) {
    // Retrieve the start of the buffer
    uint8_t *head = skb_head(skb);

    // Check for and skip padding if the first two bytes are 0x00
    if (head[0] == 0x00 && head[1] == 0x00) {
        head += 2; // Skip padding bytes
    }

    // Cast the adjusted head to eth_hdr
    struct eth_hdr *hdr = (struct eth_hdr*)head;

    // Convert ether_type to host byte order
    hdr->ether_type = ntohs(hdr->ether_type);

    return hdr;
}
#endif  // ETHERNET_H
