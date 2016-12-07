#include "common.h"
#include "libvici.h"
#include "hash.h"

void add_list(vici_req_t* req, char* type, char* proposals[], uint32_t len);
int setup(char* remote);
int initiate(char* remote);
