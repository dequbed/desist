#include "common.h"

int32_t route_flush(int32_t rt_proto, char* interface);

int32_t route_add(int32_t rt_proto, char* source, char* destination, char* mtu);

int32_t route_del(int32_t rt_proto, char* source, char* destination);

int32_t route_replace(int32_t rt_proto, char* address, char* prefix);

int32_t route_flush_cache(void);

int32_t route(char* cmd);
