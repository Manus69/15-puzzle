#include "lib.h"

#include <stdlib.h>
#include <string.h>

struct Vec
{
    void *  mem;
    int     len;
    int     isize;
    int     idx;
};

Vec * Vec_new(int isize, int capacity)
{
    Vec * vec;

    if ((vec = calloc(1, sizeof(Vec))))
    {
        if ((vec->mem = calloc(capacity, isize)))
        {
            vec->len = capacity;
            vec->isize = isize;
            vec->idx = 0;

            return vec;
        }
    }

    return 0;
}

void Vec_del(Vec * vec)
{
    free(vec->mem);
    free(vec);
}

int Vec_len(Vec const * vec)
{
    return vec->idx;
}

int Vec_empty(Vec const * vec)
{
    return Vec_len(vec) == 0;
}

void * Vec_get(Vec const * vec, int idx)
{
    return vec->mem + idx * vec->isize;
}

static void * _end(Vec const * vec)
{
    return Vec_get(vec, vec->idx);
}

void Vec_set(Vec * vec, int idx, void const * item)
{
    memcpy(Vec_get(vec, idx), item, vec->isize);
}

static void _push(Vec * vec, void const * item)
{
    memcpy(_end(vec), item, vec->isize);
    vec->idx ++;
}

static int _free_capacity(Vec const * vec)
{
    return vec->len - vec->idx;
}

bool Vec_push(Vec * vec, void const * item)
{
    if (! _free_capacity(vec))
    {
        vec->mem = realloc(vec->mem, (vec->len * 2 * vec->isize));
        if (! vec->mem) return false;

        vec->len = vec->len * 2;
    }

    _push(vec, item);

    return true;
}

void * Vec_pop(Vec * vec)
{
    vec->idx --;

    return _end(vec);
}