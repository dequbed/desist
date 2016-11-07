#include "route.h"

int32_t route_flush(int32_t rt_proto, char* interface)
{
    uint32_t len = 26 + 10 + strlen(interface);
    char* cmd = malloc(len);
    snprintf(cmd, len, "ip route flush proto %d dev %s", rt_proto, interface);
    return system(cmd);
}

int32_t route_add(int32_t rt_proto, char* source, char* destination, char* mtu)
{
    uint32_t len = 65 + strlen(source) + strlen(destination) + strlen(mtu);
    char* cmd = malloc(len);
    snprintf(cmd, len, "ip route add `ip route get %s from %s | head -1` proto %d mtu %s",
             destination,
             source,
             rt_proto,
             mtu);
    return system(cmd);
}

int32_t route_del(int32_t rt_proto, char* address, char* prefix)
{
    uint32_t len = 28 + strlen(address) + strlen(prefix);
    char* cmd = malloc(len);
    snprintf(cmd, len, "ip route del %s/%s proto 42", address, prefix);
    return system(cmd);
}

int32_t route_flush_cache(void)
{
    return system("ip route flush cache");
}

int32_t route(char* cmd)
{
    uint32_t len = strlen("ip route ") + strlen(cmd) + 1;
    char* fcmd = malloc(len);
    snprintf(fcmd, len, "ip route %s", cmd);
    return system(fcmd);
}
