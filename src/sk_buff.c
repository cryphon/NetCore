#include "sk_buff.h"
#include "sysimp.h"

struct sk_buff* skb_alloc(uint32_t size) {
    // Allocate mem for the sk_buff struct
    struct sk_buff* skb = (struct sk_buff*)malloc(sizeof(struct sk_buff));
    if(!skb) {
        perror("Failed to allocate sk_buff struct");
        return NULL;
    }

    //Allocate the buffer for packet data
    skb->head = (unsigned char*)malloc(size);
    if(!skb->head) {
        perror("Failed to allocate sk_buff data buffer");
        free(skb);
        return NULL;
    }

    // Init buffer ptrs
    skb->data = skb->head;          // Data starts at the beginning
    skb->tail = skb->head;          // Tail also starts at the beginning
    skb->end = skb->head + size;    // End points to the end of the allocated buffer

    // Init metatdata
    skb->len = 0;
    skb->protocol = 0;             // TODO: change n to typedef defines protocols
    skb->network_hdr = NULL;
    skb->transport_hdr = NULL;
    skb->payload = NULL;
    skb->payload_len = 0;
    skb->flags = 0;
    skb->next = 0;
    skb->dev = NULL;

    return skb;

}

void skb_free(struct sk_buff *skb) {
    if (skb->data != NULL) {
        free(skb->data);
        skb->data = NULL;
        skb->len = 0;
    }
}


int skb_add_data(struct sk_buff* skb, const unsigned char* data, uint32_t len) {
    if(!skb || !data || len == 0) {
        return -1; // invalid params
    }

    if ((skb->tail - skb->head + len) > (skb->end - skb->head)) {
        printf("Insufficient space: tail offset = %ld, len = %u, buffer size = %ld\n",
                skb->tail - skb->head, len, skb->end - skb->head);
        return -1;
    }

    // cpy data to buff
    memcpy(skb->tail, data, len);

    skb->tail += len;
    skb->len += len;

    return 0;
}


uint8_t* skb_push(struct sk_buff* skb, unsigned int len) {
    skb->data -= len;
    skb->len += len;

    return skb->data;
}

void skb_reset(struct sk_buff* skb) {
    if(!skb) {
        return;
    }

    // Reset buff ptrs and metadata
    skb->data = skb->head;
    skb->tail = skb->head;
    skb->len = 0;
    skb->network_hdr = NULL;
    skb->transport_hdr = NULL;
    skb->payload = NULL;
    skb->payload_len = 0;
    skb->flags = 0;
}


uint8_t* skb_head(struct sk_buff* skb) {
   return skb->head; 
}
