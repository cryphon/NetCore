#ifndef ICMP_H
#define ICMP_H

#include "sysimp.h"
#include "sk_buff.h" 
#include "tuntap.h"
#include "ipv6.h"


// ICMPv6 type definitions
#define ICMPV6_ROUTER_SOLICITATION     133
#define ICMPV6_ROUTER_ADVERTISEMENT    134
#define ICMPV6_NEIGHBOR_SOLICITATION   135
#define ICMPV6_NEIGHBOR_ADVERTISEMENT  136
#define ICMPV6_REDIRECT                137
#define MIN(a,b) ((a) < (b) ? (a) : (b))


// ICMPv6 header structure
struct icmpv6_hdr {
    uint8_t type;                   // Type = 133 for RA
    uint8_t code;                   // Code = 0 
    uint16_t checksum;              // Checksum
} __attribute__((packed));

struct icmpv6_prefix {
    uint8_t type;                   // Type = 3 for prefix info
    uint8_t len;                    // Length = 4 (in units of octets)
    uint8_t prefix_len;             // Prefix Length (typically 64)
    uint8_t flags;                  // L & A flags
    uint32_t valid_lifetime;        // Valid Lifetime
    uint32_t preferred_lifetime;    // Preferred Lifetime
    uint32_t reserved;               // Reserved field
    uint8_t prefix[16];             // The prefix
} __attribute__((packed));


// Router Advertisement specific structure
struct icmpv6_ra {
    struct icmpv6_hdr hdr;          // Header metadata
    uint8_t hop_limit;              // Hop limit
    uint8_t flags;                  // M & O flags
    uint16_t router_lifetime;       // Router Lifetime
    uint32_t reachable_time;        // Reachable Time
    uint32_t retrans_timer;         // Retransmission Timer
    struct icmpv6_prefix options; // Prefix, MTU, etc...
} __attribute__((packed));



void send_icmpv6_ra();
void handle_icmpv6(struct sk_buff* skb);

#endif  // ICMP_H
