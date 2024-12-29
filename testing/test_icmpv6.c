#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "icmp.h"

// Helper function to create and verify a Router Advertisement packet
void test_create_router_advertisement(void) {
    size_t total_size = 4 + sizeof(struct ipv6_hdr) + sizeof(struct icmpv6_ra);
    struct sk_buff* skb = skb_alloc(total_size);
    CU_ASSERT_PTR_NOT_NULL_FATAL(skb);
    
    // Create the RA packet
    struct ipv6_hdr* ipv6 = (struct ipv6_hdr*)(skb->data + 4);
    struct icmpv6_ra* ra = (struct icmpv6_ra*)(skb->data + 4 + sizeof(struct ipv6_hdr));
    
    // Set up TUN header
    uint32_t tun_pi = htonl(0x000086DD);  // IPv6 ethertype
    memcpy(skb->data, &tun_pi, 4);
    
    // Set up IPv6 header
    ipv6->vtf = htonl(0x60000000);      // Version 6
    ipv6->payload_len = htons(sizeof(struct icmpv6_ra));
    ipv6->next_hdr = 58;                // ICMPv6
    ipv6->hop_limit = 255;
    
    // Set up Router Advertisement
    ra->hdr.type = ICMPV6_ROUTER_ADVERTISEMENT;
    ra->hdr.code = 0;
    ra->hop_limit = 64;
    ra->flags = 0x80;  // Managed flag set
    ra->router_lifetime = htons(1800);   // 30 minutes
    ra->reachable_time = 0;
    ra->retrans_timer = 0;
    
    // Set up Prefix Information option
    ra->options.type = 3;
    ra->options.len = 4;
    ra->options.prefix_len = 64;
    ra->options.flags = 0xC0;            // L & A flags set
    ra->options.valid_lifetime = htonl(86400);      // 24 hours
    ra->options.preferred_lifetime = htonl(14400);   // 4 hours
    
    // Run verifications
    CU_ASSERT_EQUAL(ntohl(ipv6->vtf) >> 28, 6);  // Verify IPv6 version
    CU_ASSERT_EQUAL(ipv6->next_hdr, 58);         // Verify ICMPv6
    CU_ASSERT_EQUAL(ipv6->hop_limit, 255);       // Verify hop limit
    
    CU_ASSERT_EQUAL(ra->hdr.type, ICMPV6_ROUTER_ADVERTISEMENT);
    CU_ASSERT_EQUAL(ra->hdr.code, 0);
    CU_ASSERT_EQUAL(ra->hop_limit, 64);
    CU_ASSERT_EQUAL(ra->flags, 0x80);
    CU_ASSERT_EQUAL(ntohs(ra->router_lifetime), 1800);
    
    CU_ASSERT_EQUAL(ra->options.type, 3);
    CU_ASSERT_EQUAL(ra->options.len, 4);
    CU_ASSERT_EQUAL(ra->options.prefix_len, 64);
    CU_ASSERT_EQUAL(ra->options.flags, 0xC0);
    CU_ASSERT_EQUAL(ntohl(ra->options.valid_lifetime), 86400);
    CU_ASSERT_EQUAL(ntohl(ra->options.preferred_lifetime), 14400);
    
    // Cleanup
    free(skb->data);
    free(skb);
}

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("ICMPv6 RA Tests", init_suite, clean_suite);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(suite, "test router advertisement creation", test_create_router_advertisement)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    unsigned int failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    
    return failures;
}
