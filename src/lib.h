#ifndef LIB_H
#define LIB_H

#include <stdbool.h>

#define RBUFF_SIZE (1 << 5)

#define $swap(T, x, y) \
{\
    T _t = * (T *) (x);\
    * (T *) (x) = * (T *) (y);\
    * (T *) (y) = _t;\
}

typedef int (* cmpf)(void const *, void const *);
typedef bool (* eqf)(void const *, void const *);
typedef unsigned long (* hashf)(void const *);

static inline unsigned rng_xor(unsigned * seed)
{
    unsigned x;

    x = * seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return * seed = x;
}

static inline unsigned long hash(unsigned char const * bytes, int len)
{
    unsigned long hash = 5381;

    for (int k = 0; k < len; k ++)
    {
        hash = hash * 33 + bytes[k];
    }

    return hash;
}

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

static inline void Rbuff_pushc_forced(Rbuff * rbuff, char x)
{
    if (! Rbuff_free_capacity(rbuff)) Rbuff_pop(rbuff);

    Rbuff_pushc(rbuff, x);
}

typedef struct Htbl Htbl;

Htbl *  Htbl_new(int isize);
void    Htbl_del(Htbl * htbl);
void *  Htbl_get(Htbl const * htbl, void const * item, hashf hf, eqf eq);
int     Htbl_insert(Htbl * htbl, void const * item, hashf hf, eqf eq);


#endif