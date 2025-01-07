#include "sysimp.h"
#include "icmp.h"


// Router Advertisement specific structure
struct ndp_ra {
    struct icmpv6_hdr hdr;          // Header metadata
    uint8_t hop_limit;              // Hop limit
    uint8_t flags;                  // M & O flags
    uint16_t router_lifetime;       // Router Lifetime
    uint32_t reachable_time;        // Reachable Time
    uint32_t retrans_timer;         // Retransmission Timer
    struct icmpv6_prefix options; // Prefix, MTU, etc...
} __attribute__((packed));



