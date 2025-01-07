#include "sysimp.h"
#include "icmp.h"

// NDP Neighbor Solicit
struct ndp_neighbor_sol {
    struct icmpv6_hdr hdr;          // Header metadata
    uint32_t reserved;              // Reserved field (must be 0)
    uint8_t target_addr[16];        // Target IPv6 addr
    // Optional ICMPv6 options (e.g., Source Link-Layer Addr)
    uint8_t options[];              // Variable-length options (if any)
} __attribute__((packed));

// NDP Neighbor Advertisement
struct ndp_neighbor_adv {
    struct icmpv6_hdr hdr;          // Header metadata
    uint32_t flags_reserved;        // Flags and reserved fields     
    uint8_t target_addr[16];        // Target IPv6 addr
    // Optional ICMPv6 options
    uint8_t options[];              // Variable-length options (if any)
} __attribute__((packed));




