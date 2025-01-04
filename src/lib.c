#include "lib.h"

#include <stdbool.h>

#define HTBL_LOADF (75.0)

typedef struct 
{
    void *  mem;
    int     len;
    int     isize;
    int     count;
}   Htbl;

static void * _get(Htbl const * htbl, int idx)
{
    return htbl->mem + idx * htbl->isize;
}

static bool _is_empty(Htbl const * htbl, int idx)
{
    char * ptr;

    ptr = _get(htbl, idx);

    return * ptr == 0;
}

static int _idxof(Htbl const * htbl, void const * item, int start_idx, cmpf cmp)
{
    while (true)
    {
        if (cmp(item, _get(htbl, start_idx)) == 0) return start_idx;
        if (! _is_empty(htbl, start_idx)) return -1;

        start_idx = (start_idx + 1) % htbl->len;
    }
}

static void _set(Htbl const * htbl, void const * item, int idx)
{
    
}

void * Htbl_get(Htbl const * htbl, void const * item, hashf hf, cmpf cmp)
{
    int             idx;
    unsigned long   hash;

    hash = hf(item);
    idx = (hash % (unsigned long) htbl->len);
    idx = _idxof(htbl, item, idx, cmp);

    return idx < 0 ? 0 : _get(htbl, idx);
}