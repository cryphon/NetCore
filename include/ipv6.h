#ifndef IPV6_H
#define IPV6_H

#include <stdint.h>
#include "sysimp.h"

struct ipv6_hdr {
    uint32_t vtf;           // Version, Traffic Class, Flow Label
    uint16_t payload_len;   // Payload Length
    uint8_t next_hdr;       // Next Header
    uint8_t hop_limit;      // Hop Limit
    uint8_t src[16];        // Source Address
    uint8_t dest[16];       // Destination Address
} __attribute__((packed));

static inline struct ipv6_hdr* ipv6_hdr(struct sk_buff* skb) {
    return (struct ipv6_hdr*)(skb->data + 4);  // Skip TUN header
}

#endif // IPV6_H

