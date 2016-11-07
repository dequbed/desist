#include "opennhrp-script.h"
#include "util.h"
#include "hash.h"
#include "neigh.h"
#include "route.h"

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 2)
    {
        usage();
        exit(1);
    }

    char* type             = getenv("NRHP_TYPE");
    char* gre_key          = getenv("NRHP_GRE_KEY");
    char* interface        = getenv("NHRP_INTERFACE");
    char* destmtu          = getenv("NHRP_DESTMTU");
    char* destnbma         = getenv("NRHP_DESTNBMA");
    char* srcnbma          = getenv("NRHP_SRCNBMA");
    char* destaddr         = getenv("NRHP_DESTADDR");
    char* srcaddr          = getenv("NRHP_SRCADDR");
    char* peer_down_reason = getenv("NRHP_PEER_DOWN_REASON");
    char* nexthop          = getenv("NRHP_NEXTHOP");
    char* destprefix       = getenv("NRHP_DESTPREFIX");

    switch (lookup(argv[1]))
    {
        case INTERFACE_UP:
            route_flush(RT_PROTO, interface);
            neigh_flush(interface);
            break;

        case PEER_REGISTER:
            break;

        case PEER_UP:
            if (destmtu != NULL)
                route_add(RT_PROTO, srcnbma, destnbma, destmtu);

            break;

        case PEER_DOWN:
            route_del(RT_PROTO, srcnbma, destnbma);
            break;

        case NHS_UP:
            break;

        case NHS_DOWN:
            break;

        case ROUTE_UP:
            system("ip route flush cache");
            break;

        case ROUTE_DOWN:
            system("ip route flush cache");
            break;

        /* This should never trigger in normal usage */
        case BADKEY:
            usage();
            exit(1);
            break;
    }

}

void usage(void)
{}
