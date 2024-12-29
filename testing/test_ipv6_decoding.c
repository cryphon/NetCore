#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/ipv6.h"  // Path to your header

void test_ipv6_parsing() {
    uint8_t ipv6_packet[40] = {
        0x00, 0x00, 0x86, 0xdd,  // Simulated TUN metadata
        0x60, 0x00, 0x00, 0x00,  // IPv6 header (starts at offset 4)
        0x00, 0x14,
        0x3B,
        0x40,
        0x20, 0x01, 0x0d, 0xb8,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x20, 0x01, 0x0d, 0xb8,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x02
    };



    struct ipv6_hdr *hdr = (struct ipv6_hdr *)(ipv6_packet + 4);  // Skip TUN metadata
    
    // explicitely copy 4 bytes into vtf
    hdr->vtf = (ipv6_packet[4] << 24) |
           (ipv6_packet[5] << 16) |
           (ipv6_packet[6] << 8)  |
            ipv6_packet[7];

    CU_ASSERT_EQUAL((hdr->vtf >> 28) & 0xF, 6);  // Check IPv6 version
    CU_ASSERT_EQUAL(ntohs(hdr->payload_len), 20);
    CU_ASSERT_EQUAL(hdr->next_hdr, 0x3B);
    CU_ASSERT_EQUAL(hdr->hop_limit, 64);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("IPv6 Parser Tests", 0, 0);
    
    CU_add_test(suite, "Test IPv6 Header Parsing", test_ipv6_parsing);
    
    CU_basic_set_mode(CU_BRM_NORMAL);  // Suppress version message
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}

