#ifndef TUNTAP_H
#define TUNTAP_H


#include <linux/if.h>

void tun_init(void);
int tun_read(char* buff, int len);
int tun_write(char* buff, int len);

#endif  // TUNTAP_H
