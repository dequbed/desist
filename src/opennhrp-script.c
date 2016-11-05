#include "opennhrp-script.h"
#include "util.h"

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 2)
    {
        usage();
        exit(1);
    }

    switch (lookup(argv[1]))
    {
        case INTERFACE_UP:
            break;

        case PEER_REGISTER:
            break;

        case PEER_UP:
            break;

        case PEER_DOWN:
            break;

        case NHS_UP:
            break;

        case NHS_DOWN:
            break;

        case ROUTE_UP:
            break;

        case ROUTE_DOWN:
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
