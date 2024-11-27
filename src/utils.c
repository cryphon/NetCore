#include "sysimp.h"
#include "utils.h"

int cmd(char* cmd, ...) {

    char command[CMD_BUFF_MAX]; // Ensure this size is sufficient commands
    va_list args;
    va_start(args, cmd);
    vsnprintf(command, sizeof(command), cmd, args);
    va_end(args);

    return system(command);
}


int parse_ip(char* addr) {
    struct in_addr ip_addr;

    //parse IP addr string
    int new_addr = inet_pton(AF_INET, addr, &ip_addr);
    if(new_addr != 1) {
        fprintf(stderr, "[ERROR]: Invalid ip format\n");
        return -1;
    }
    return new_addr;
}
