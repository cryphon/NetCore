#ifndef ICMP_H
#define ICMP_H

#include "sysimp.h"
#include "sk_buff.h" 


// ICMPv6 type definitions
#define ICMPV6_ROUTER_SOLICITATION     133
#define ICMPV6_ROUTER_ADVERTISEMENT    134
#define ICMPV6_NEIGHBOR_SOLICITATION   135
#define ICMPV6_NEIGHBOR_ADVERTISEMENT  136
#define ICMPV6_REDIRECT                137
#define MIN(a,b) ((a) < (b) ? (a) : (b))


// ICMPv6 header structure
struct icmpv6_hdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
} __attribute__((packed));

// Router Advertisement specific structure
struct icmpv6_router_advert {
    struct icmpv6_hdr hdr;
    uint8_t hop_limit;
    uint8_t flags;
    uint16_t router_lifetime;
    uint32_t reachable_time;
    uint32_t retrans_timer;
} __attribute__((packed));



void decode_icmpv6_packet(uint8_t* icmpv6_hdr) {
    struct icmpv6_hdr* hdr = (struct icmpv6_hdr*)icmpv6_hdr;

    printf("ICMPv6 Packet Details:\n");
    printf(" - Type: %d ", hdr->type);

    // Interpret common ICMPv6 types
    switch(hdr->type) {
        case ICMPV6_ROUTER_SOLICITATION:
            printf("(Router Solicitation)\n");
            break;
        case ICMPV6_ROUTER_ADVERTISEMENT:
            printf("(Router Advertisement)\n");

            // Cast to Router Advertisement structure
            struct icmpv6_router_advert* ra = (struct icmpv6_router_advert*)hdr;

            printf(" - Hop Limit: %d\n", ra->hop_limit);
            printf(" - Flags: 0x%02x\n", ra->flags);
            printf(" - Router Lifetime: %d seconds\n", ntohs(ra->router_lifetime));
            printf(" - Reachable Time: %u ms\n", ntohl(ra->reachable_time));
            printf(" - Retransmission Timer: %u ms\n", ntohl(ra->retrans_timer));
            break;
        case ICMPV6_NEIGHBOR_SOLICITATION:
            printf("(Neighbor Solicitation)\n");
            break;
        case ICMPV6_NEIGHBOR_ADVERTISEMENT:
            printf("(Neighbor Advertisement)\n");
            break;
        case ICMPV6_REDIRECT:
            printf("(Redirect)\n");
            break;
        default:
            printf("(Unknown)\n");
    }

    printf(" - Code: %d\n", hdr->code);
    printf(" - Checksum: 0x%04x\n", ntohs(hdr->checksum));
}

#endif  // ICMP_H
