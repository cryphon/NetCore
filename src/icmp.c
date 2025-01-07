#include "ndp.h"
#include "icmp.h"

void send_icmpv6_ra() {
    // reads from Router Soliciation (RS) and sends Router Advertisement (RA)
    size_t total_size = 4 + sizeof(struct ipv6_hdr) + sizeof(struct ndp_ra) + sizeof(struct icmpv6_prefix);


    struct sk_buff* skb = skb_alloc(total_size);
    if(!skb) {
        printf("Failed to allocate skb in icmpv6_ra\n");
        return;
    }
    
    struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);

    // setup header
    ipv6->vtf = htonl(0x60000000);      // Version 6
    ipv6->payload_len = htons(sizeof(struct ndp_ra) + sizeof(struct icmpv6_prefix));
    ipv6->next_hdr = 58;                // ICMPv6
    ipv6->hop_limit = 255;              // Hop Limit

    // Source address (link-local address)
    uint8_t src[16] = { 0xfe, 0x80, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x01};
    memcpy(ipv6->src, src, 16);

    // destination (all-nodes multicast)
    uint8_t dst[16] = { 0xff, 0x02, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x01};
    memcpy(ipv6->dest, dst, 16);


    // Set up Router Advertisement
    struct ndp_ra* ra = (struct ndp_ra*)(skb->data + sizeof(struct ipv6_hdr));
    ra->hdr.type = ICMPV6_ROUTER_ADVERTISEMENT;
    ra->hdr.code = 0;
    ra->hdr.checksum = 0;  // Will be calculated later
    ra->hop_limit = 64;
    ra->flags = 0x80;  // Managed flag set
    ra->router_lifetime = htons(1800);  // 30 minutes
    ra->reachable_time = 0;
    ra->retrans_timer = 0;


            printf(" - Hop Limit: %d\n", ra->hop_limit);
            printf(" - Flags: 0x%02x\n", ra->flags);
            printf(" - Router Lifetime: %d seconds\n", ntohs(ra->router_lifetime));
            printf(" - Reachable Time: %u ms\n", ntohl(ra->reachable_time));
            printf(" - Retransmission Timer: %u ms\n", ntohl(ra->retrans_timer));

    // setup Prefix Information option
    struct icmpv6_prefix* prefix = &ra->options;
    prefix->type = 3;
    prefix->len = 4;
    prefix->prefix_len = 64;
    prefix->flags = 0xC0; // L & A flags set
    prefix->valid_lifetime = htonl(86400);
    prefix->preferred_lifetime = htonl(14400);
    prefix->reserved = 0;

    // set Prefix (e.g., fd00:/64)
    uint8_t prefix_addr[16] = {0xfd, 0x00};
    memcpy(prefix->prefix, prefix_addr, 16);


    // Add TUN header
    uint32_t tun_pi = htonl(0x000086DD);  // IPv6 ethertype
    memcpy(skb->data, &tun_pi, 4);

    // Set the packet length
    skb->len = total_size;
    skb->tail = skb->data + total_size;





    // calculate ICMPv6 checksum
    size_t icmp_len = sizeof(struct ndp_ra) + sizeof(struct icmpv6_prefix);
    uint32_t sum = 0;

    // pseudo pointer 
    uint16_t* ptr;

    // src and dst addresses 
    ptr = (uint16_t*)ipv6->src;
    for(int i = 0; i < 8; i++) {
        sum += ntohs(ptr[i]);
    }

    ptr = (uint16_t*)ipv6->dest;
    for(int i = 0; i < 8; i++) {
        sum += ntohs(ptr[i]);
    }

    // Upper layer packet length and next_hdr
    sum += htons(icmp_len);
    sum += htons(58);

    // Then the ICMPv6 packet
    ptr = (uint16_t*)ra;
    for(size_t i = 0; i < icmp_len/2; i++) {
        if(i != 1) {
            // skip checksum field
            sum += ntohs(ptr[i]);
        }
    }
    
    // Fold to 16 bits to reduce size
    while(sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    ra->hdr.checksum = htons(~sum);

    tun_write_skb(skb);
}

void handle_icmpv6(struct sk_buff* skb) {

    struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);
    uint8_t* curr_hdr = (uint8_t*)(ipv6 + 1);


    //decode_icmpv6_packet(curr_hdr);

    struct icmpv6_hdr* hdr = (struct icmpv6_hdr*)curr_hdr;
    switch(hdr->type) {
        case 128:
            //echo 
            break;
        case ICMPV6_ROUTER_SOLICITATION:
            // router solicitation
            printf("router solicitation\n");
            send_icmpv6_ra();
            break;
    }
}
