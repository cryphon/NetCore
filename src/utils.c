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
