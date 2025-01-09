#include "lib.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define HTBL_LOADF  (0.75)
#define HTBL_DC     (1 << 10)

struct Htbl
{
    void *  mem;
    int     len;
    int     isize;
    int     count;
};

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

static int _idxhash(Htbl const * htbl, void const * item, hashf hf)
{
    return hf(item) % (unsigned long) htbl->len;
}

static int _idxof(Htbl const * htbl, void const * item, int start_idx, eqf eq)
{
    while (true)
    {
        if (_is_empty(htbl, start_idx))         return -1;
        if (eq(item, _get(htbl, start_idx)))    return start_idx;

        start_idx = (start_idx + 1) % htbl->len;
    }
}

static int _idxfree(Htbl const * htbl, int start_idx)
{
    while (true)
    {
        if (_is_empty(htbl, start_idx)) return start_idx;

        start_idx = (start_idx + 1) % htbl->len;
    }
}

static void _set(Htbl const * htbl, void const * item, int idx)
{
    memcpy(_get(htbl, idx), item, htbl->isize);
}

static double _load(Htbl const * htbl)
{
    return ((double) htbl->count) / htbl->len;
}

static void _insert(Htbl * htbl, void const * item, hashf hf)
{
    int idx;

    idx = _idxhash(htbl, item, hf);
    idx = _idxfree(htbl, idx);

    _set(htbl, item, idx);
    htbl->count ++;
}

static bool _rehash(Htbl * htbl, hashf hf)
{
    void *  mem;
    void *  ptr;
    Htbl    new;
    int     len;

    len = htbl->len * 2;
    if (! (mem = calloc(len, htbl->isize))) return false;
    
    new = (Htbl)
    {
        .mem = mem,
        .isize = htbl->isize,
        .count = 0,
        .len = len,
    };

    for (int k = 0; k < htbl->len; k ++)
    {
        if (_is_empty(htbl, k)) continue;

        ptr = _get(htbl, k);
        _insert(& new, ptr, hf);
    }

    free(htbl->mem);
    * htbl = new;

    return true;
}

int Htbl_count(Htbl const * htbl)
{
    return htbl->count;
}

Htbl * Htbl_new(int isize, int capacity)
{
    Htbl * htbl;

    if ((htbl = calloc(1, sizeof(* htbl))))
    {
        if ((htbl->mem = calloc(capacity, isize)))
        {
            htbl->isize = isize;
            htbl->len = HTBL_DC;
        }

        return htbl;
    }

    return 0;
}

void Htbl_del(Htbl * htbl)
{
    free(htbl->mem);
    free(htbl);
}

// #include "dbg.h"
void * Htbl_get(Htbl const * htbl, void const * item, hashf hf, eqf eq)
{
    int idx;

    if (htbl->count == 0) return NULL;

    idx = _idxhash(htbl, item, hf);
    //
    // Npuzzle * lhs, * rhs;

    // lhs = _get(htbl, idx);
    // rhs = (Npuzzle *) item;
    //
    idx = _idxof(htbl, item, idx, eq);


    return idx < 0 ? 0 : _get(htbl, idx);
}

int Htbl_insert(Htbl * htbl, void const * item, hashf hf, eqf eq)
{
    if (Htbl_get(htbl, item, hf, eq)) return 0;
    if (_load(htbl) > HTBL_LOADF)
    {
        if (! _rehash(htbl, hf)) return -1;
    }

    _insert(htbl, item, hf);

    return 1;
}

void Htbl_purge(Htbl * htbl)
{
    memset(htbl->mem, 0, htbl->isize * htbl->len);
    htbl->count = 0;
}

