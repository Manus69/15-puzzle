#ifndef LIB_H
#define LIB_H

#define LIB_PATH "_slib.h"
#include LIB_PATH

#define RBUFF_SIZE (1 << 10)

typedef struct
{
    char    vals[RBUFF_SIZE];
    int     idx;
    int     len;
}   Rbuff;

static inline int Rbuff_len(Rbuff const * rbuff)
{
    return rbuff->len;
}

static inline bool Rbuff_empty(Rbuff const * rbuff)
{
    return Rbuff_len(rbuff) == 0;
}

static inline bool Rbuff_full(Rbuff const * rbuff)
{
    return rbuff->len == RBUFF_SIZE;
}

static inline char * Rbuff_get(Rbuff * rbuff, int k)
{
    return rbuff->vals + ((rbuff->idx + k) % RBUFF_SIZE);
}

static inline int Rbuff_free_capacity(Rbuff const * rbuff)
{
    return RBUFF_SIZE - rbuff->len;
}

static inline void Rbuff_pushc(Rbuff * rbuff, char x)
{
    char * dst;

    dst = Rbuff_get(rbuff, rbuff->len);
    * dst = x;
    rbuff->len ++;
}

static inline bool Rbuff_pushc_check(Rbuff * rbuff, char x)
{
    if (Rbuff_free_capacity(rbuff))
    {
        Rbuff_pushc(rbuff, x);

        return true;
    }

    return false;
}

static inline char Rbuff_pop(Rbuff * rbuff)
{
    char * ptr;

    ptr = Rbuff_get(rbuff, 0);
    rbuff->idx = (rbuff->idx + 1) % RBUFF_SIZE;
    rbuff->len --;

    return * ptr;
}

static inline void Rbuff_pop_all(Rbuff * rbuff)
{
    rbuff->len = 0;
}

static inline void Rbuff_pushc_forced(Rbuff * rbuff, char x)
{
    if (! Rbuff_free_capacity(rbuff)) Rbuff_pop(rbuff);

    Rbuff_pushc(rbuff, x);
}


#endif