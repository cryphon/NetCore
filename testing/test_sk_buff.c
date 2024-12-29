#include "sysimp.h"
#include "sk_buff.h"


#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_skb_allocation() {
    struct sk_buff* skb = skb_alloc(2048);

    CU_ASSERT_PTR_NOT_NULL(skb->data);
    CU_ASSERT_EQUAL(skb->len, 0);

    // Add data after allocation to verify buffer functionality
    const char *test_data = "Hello, sk_buff!";
    memcpy(skb->data, test_data, strlen(test_data) + 1);  // +1 for null terminator
    skb->len = strlen(test_data);

    CU_ASSERT_EQUAL(skb->len, strlen(test_data));
    CU_ASSERT_STRING_EQUAL((char *)skb->data, test_data);

    skb_free(skb);
}


void test_skb_free() {
    struct sk_buff* skb = skb_alloc(128);
    skb_free(skb);

    CU_ASSERT_PTR_NULL(skb->data);
    CU_ASSERT_EQUAL(skb->len, 0);

    // Test double free
    skb_free(skb);
    CU_ASSERT_PTR_NULL(skb->data);
    CU_ASSERT_EQUAL(skb->len, 0);
}


int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("sk_buff Tests", 0, 0);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
;
    CU_add_test(suite, "skb Allocation", test_skb_allocation);
    CU_add_test(suite, "skb Free", test_skb_free);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}

