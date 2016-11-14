#include "route.h"

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/msg.h>
#include <netlink/addr.h>
#include <netlink/route/route.h>
#include <netlink/route/nexthop.h>
#include <netlink/route/addr.h>

int32_t route_flush(int32_t rt_proto, char* interface)
{
    uint32_t len = 26 + 10 + strlen(interface);
    char* cmd = malloc(len);
    snprintf(cmd, len, "ip route flush proto %d dev %s", rt_proto, interface);
    return system(cmd);
}

int32_t route_add(int32_t rt_proto, char* source, char* destination, char* mtu)
{
    struct nl_sock* sk;
    struct rtnl_route* route;
    struct nl_addr* dst, *gw;
    struct rtnl_nexthop* nh;
    int mtui;

    /* Allocate a new socket */
    sk = nl_socket_alloc();
    nl_connect(sk, NETLINK_ROUTE);

    route = rtnl_route_alloc();
    nh = rtnl_route_nh_alloc();

    dst = nl_addr_alloc(4);
    gw = nl_addr_alloc(4);

    nl_addr_parse(source, AF_INET, &gw);
    nl_addr_parse(destination, AF_INET, &dst);

    rtnl_route_nh_set_gateway(nh, gw);

    rtnl_route_add_nexthop(route, nh);
    rtnl_route_set_dst(route, dst);

    mtui = atoi(mtu);
    rtnl_route_set_metric(route, 2 /*mtu*/, mtui);

    return rtnl_route_add(sk, route, NLM_F_EXCL);
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
