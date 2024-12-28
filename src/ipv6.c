#include "ipv6.h"
#include "sk_buff.h"
#include "sysimp.h"
#include "icmp.h"

void ipv6_recv(struct sk_buff* skb) {
    // (skip 4 bytes for TUN header)
    struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);

    uint8_t* curr_hdr = (uint8_t*)(ipv6 + 1);
    uint8_t next_hdr = ipv6->next_hdr;



    if (next_hdr == 0x3A) { // ICMPv6
        printf("ICMPv6 header starts at offset: %ld\n", curr_hdr - skb->data);
        printf("First 8 bytes of ICMPv6 header: ");
        for (int i = 0; i < 8; i++) {
            printf("%02x ", curr_hdr[i]);
        }
        printf("\n");

        decode_icmpv6_packet(curr_hdr);
    } else {
        printf("Unknown payload type: 0x%02x\n", next_hdr);
    }

    skb_reset(skb);

}
