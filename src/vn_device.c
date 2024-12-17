#include "sysimp.h"

#define DEBUG_ETH
#include "vn_device.h"
#include "tuntap.h"
#include "utils.h"
#include "sk_buff.h"
#include "ethernet.h"
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


// Check if the next header is an extension header
int is_extension_header(uint8_t next_hdr) {
    // Common extension headers in IPv6
    return next_hdr == 0 ||  // Hop-by-Hop Options
        next_hdr == 43 || // Routing Header
        next_hdr == 44 || // Fragment Header
        next_hdr == 51 || // Authentication Header
        next_hdr == 50 || // Encapsulation Security Payload
        next_hdr == 60;   // Destination Options
}

// Get the length of the current extension header
size_t extension_header_length(const uint8_t* ext_hdr) {
    // Length field is the second byte of the extension header
    // Value is in 8-octet units, excluding the first 8 bytes
    return 8 + (ext_hdr[1] * 8);
}

// Get the pointer to the next header field in the current extension header
uint8_t* next_header_ptr_in_extension(uint8_t* ext_hdr) {
    return &ext_hdr[0];
}




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

    switch(hdr->ether_type) {
        case 0x86DD:
            printf("Hello world\n");
            break;
        default:
            printf("default behaviour\n");
    }
    return 0;

}

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

void* vn_device_recvqueue_loop() {
    struct sk_buff* skb = skb_alloc(BUFF_MAX_LEN);
    printf("Listening on device %s\n", vn_device->name);

    while (1) {
        memset(skb->data, 0, BUFF_MAX_LEN);

        int bytes_read = tun_read((char*)skb->data, BUFF_MAX_LEN);
        if (bytes_read < 0) {
            perror("Error reading from TUN device");
            continue;
        }

        // Print the first 64 bytes (or less) of the packet
        printf("\nRaw packet data (first 64 bytes):\n");
        for (int i = 0; i < MIN(bytes_read, 64); i++) {
            if (i % 16 == 0) printf("%04x: ", i);
            printf("%02x ", skb->data[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");

        // Skip the TUN header (4 bytes)
        struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);
        
        printf("IPv6 Header:\n");
        printf(" - VTF (raw): 0x%08x\n", ntohl(ipv6->vtf));
        printf(" - Payload Length: %d\n", ntohs(ipv6->payload_len));
        printf(" - Next Header: 0x%02x\n", ipv6->next_hdr);
        printf(" - Hop Limit: %d\n", ipv6->hop_limit);

        printf(" - Source Address: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x", ipv6->src[i]);
            if (i % 2 == 1 && i < 15) printf(":");
        }
        printf("\n");

        printf(" - Destination Address: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x", ipv6->dest[i]);
            if (i % 2 == 1 && i < 15) printf(":");
        }
        printf("\n");

        uint8_t next_hdr = ipv6->next_hdr;
        uint8_t* current_header = (uint8_t*)(ipv6 + 1);

        if (next_hdr == 0x3A) { // ICMPv6
            printf("ICMPv6 header starts at offset: %ld\n", current_header - skb->data);
            printf("First 8 bytes of ICMPv6 header: ");
            for (int i = 0; i < 8; i++) {
                printf("%02x ", current_header[i]);
            }
            printf("\n");
            
            decode_icmpv6_packet(current_header);
        } else {
            printf("Unknown payload type: 0x%02x\n", next_hdr);
        }

        skb_reset(skb);
    }
}
