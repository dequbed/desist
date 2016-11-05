#include "hash.h"

uint16_t read16bit(uint8_t* data)
{
    return ((data[1] << 8) + data[0]);
}

uint32_t hash(uint8_t* data, uint32_t len)
{
    if (len <= 0 || data == NULL)
        return 0;

    uint32_t tmp, rem;
    uint32_t hash = len;

    rem = len & 3; // bAND 3
    len /= 4;

    for (; len > 0; len--)
    {
        hash += read16bit(data);
        tmp   = (read16bit(data + 2) << 11) ^ hash;
        hash  = (hash << 16) ^ tmp;
        hash += hash >> 11;
        // Advance pointer by the 4 read bytes
        data += 4;
    }

    switch (rem)
    {
        case 1:
            hash += *data;
            hash ^= hash << 10;
            hash += hash >> 1;
            break;

        case 2:
            hash += read16bit(data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;

        case 3:
            hash += read16bit(data);
            hash ^= hash << 16;
            hash ^= data[2] << 18;
            hash += hash >> 11;
            break;
    }

    /* Avalanching of final bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
