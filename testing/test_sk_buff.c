#include "sysimp.h"
#include "sk_buff.h"

// Function to print the state of a sk_buff
void print_sk_buff(struct sk_buff* skb) {
    if (!skb) {
        printf("sk_buff is NULL.\n");
        return;
    }
    printf("sk_buff state:\n");
    printf("  Data length: %u\n", skb->len);
    printf("  Buffer start: %p\n", skb->head);
    printf("  Data start: %p\n", skb->data);
    printf("  Tail: %p\n", skb->tail);
    printf("  End: %p\n", skb->end);
    if (skb->network_hdr) {
        printf("  Network header: %p\n", skb->network_hdr);
    } else {
        printf("  Network header: NULL\n");
    }
    if (skb->transport_hdr) {
        printf("  Transport header: %p\n", skb->transport_hdr);
    } else {
        printf("  Transport header: NULL\n");
    }
    printf("  Flags: %u\n", skb->flags);
}

int main() {
    // Step 1: Allocate a sk_buff
    struct sk_buff* skb = skb_alloc(2048);
    if (!skb) {
        fprintf(stderr, "Failed to allocate sk_buff.\n");
        return 1;
    }
    printf("Allocated sk_buff.\n");
    print_sk_buff(skb);

    // Step 2: Add data to the sk_buff
    const char test_data[] = "Hello, sk_buff!";
    if (skb_add_data(skb, (unsigned char*)test_data, sizeof(test_data)) == 0) {
        printf("Added data to sk_buff.\n");
    } else {
        fprintf(stderr, "Failed to add data to sk_buff.\n");
        skb_free(skb);
        return 1;
    }
    print_sk_buff(skb);

    // Step 3: Reset the sk_buff
    skb_reset(skb);
    printf("Reset sk_buff.\n");
    print_sk_buff(skb);

    // Step 4: Free the sk_buff
    skb_free(skb);
    printf("Freed sk_buff.\n");

    return 0;
}

