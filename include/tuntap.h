#ifndef TUNTAP_H
#define TUNTAP_H

#include <linux/if.h>
#include "sk_buff.h"

void tun_init(void);
int tun_read(char* buff, int len);
int tun_write(char* buff, int len);
int tun_write_skb(struct sk_buff* skb);

#endif  // TUNTAP_H
