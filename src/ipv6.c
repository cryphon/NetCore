#include "ipv6.h"
#include "sk_buff.h"
#include "sysimp.h"
#include "icmp.h"

void ipv6_recv(struct sk_buff* skb) {
    // (skip 4 bytes for TUN header)
    struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);

    uint8_t* curr_hdr = (uint8_t*)(ipv6 + 1);
    uint8_t next_hdr = ipv6->next_hdr;

    switch(next_hdr) {
        case 58:    // ICMPv6
            handle_icmpv6(skb);
            break;
        default:
            printf("unknown ipv6");
    }

    skb_reset(skb);

}
