#include "neigh.h"

int32_t neigh_flush(char* interface)
{
    uint32_t len = 19 + strlen(interface);
    char* cmd = malloc(len);
    snprintf(cmd, len, "ip neigh flush dev %s", interface);
    return system(cmd);
}
