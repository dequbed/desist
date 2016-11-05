/* Random small utilites */
#include "util.h"

int32_t lookup(char* key)
{
    for (uint32_t i = 0; i < NKEYS; i++)
    {
        kval_t* e = &lookuptable[i];

        if (strncmp(e->key, key, 10) == 0)
            return e->val;
    }

    return BADKEY;
}
