#ifndef SK_BUFF_H
#define SK_BUFF_H
#include "sysimp.h"

struct sk_buff {

    // connector for queues
    struct sk_buff* next;

    /* point to ''physical device this packet belongs to.
    *  should be valid in both directions
    */
    struct vn_device* dev;


    // Buffer pointers
    unsigned char* head;        // Start of allocated buffer
    unsigned char* data;        // Start of data in buffer
    unsigned char* tail;        // End of data in buffer
    unsigned char* end;         // End of allocated buffer

    // Metadata
    uint32_t len;               // Length of data
    uint16_t protocol;          // Protocol type (e.g. TCP, IP)
    uint8_t* network_hdr;       // Network header ptr
    uint8_t* transport_hdr;      // Transport header ptr
    unsigned char* payload;     // Ptr to payload data
    uint32_t payload_len;       // Length of payload
    
    // Flags for other metadata
    uint32_t flags;
};
// allocate new sk_buff
struct sk_buff* skb_alloc(uint32_t size);

// free sk_buff
void skb_free(struct sk_buff* skb);
int skb_add_data(struct sk_buff* skb, const unsigned char* data, uint32_t len);
void skb_reset(struct sk_buff* skb);


#endif // SK_BUFF_H
