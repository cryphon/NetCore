#ifndef UTILS_H
#define UTILS_H

#define CMD_BUFF_MAX 100

int cmd(char* cmd, ...);
int parse_ip(char* addr);


#define print_debug(str, ...)                       \
    printf(str" - %s:%u\n", ##__VA_ARGS__, __FILE__, __LINE__);
#endif
