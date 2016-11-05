/* Random small utilites */
#include "util.h"

uint64_t lookup(char* key)
{
    for (uint64_t i = 0; i < NKEYS; i++)
    {
        kval_t* e = lookuptable + i * sizeof(kval_t);

        if (strcmp(e->key, key) == 0)
            return e->val;
    }

    return BADKEY;
}
