#pragma once

#include "common.h"

/*
 * Lookup definitions used in main()
 */

#define BADKEY -1
#define INTERFACE_UP 1
#define PEER_REGISTER 2
#define PEER_UP 3
#define PEER_DOWN 4
#define NHS_UP 5
#define NHS_DOWN 6
#define ROUTE_UP 7
#define ROUTE_DOWN 8

typedef struct
{
    char* key;
    int32_t val;
} kval_t;

static kval_t lookuptable[] =
{
    { "interface-up", INTERFACE_UP },
    { "peer-register", PEER_REGISTER },
    { "peer-up", PEER_UP },
    { "peer-down", PEER_DOWN },
    { "nhs-up", NHS_UP },
    { "nhs-down", NHS_DOWN },
    { "route-up", ROUTE_UP },
    { "route-down", ROUTE_DOWN }
};

#define NKEYS (sizeof(lookuptable)/sizeof(kval_t))
