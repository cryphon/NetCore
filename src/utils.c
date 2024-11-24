#include "sysimp.h"
#include "utils.h"

int cmd(char* cmd, ...) {

    va_list args;
    char buff[CMD_BUFF_MAX];

    va_start(args, cmd);
    vsnprintf(cmd, CMD_BUFF_MAX, cmd, args);
    va_end(args);

    return system(buff);
}
