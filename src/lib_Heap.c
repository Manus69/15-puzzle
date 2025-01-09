#include "lib.h"

#include <string.h>
#include <stdlib.h>

#define HEAP_DC (1 << 10)

struct Heap
{
    Vec * vec;
};  

static int _parent_idx(int idx)
{
    return (idx - 1) / 2;
}

static int _left_idx(int idx)
{
    return idx * 2 + 1; 
}

static int _right_idx(int idx)
{
    return idx * 2 + 2;
}

static void * _get(Heap const * heap, int idx)
{
    return Vec_get(heap->vec, idx);
}

static void * _ins_spot(Heap const * heap)
{
    return Vec_get(heap->vec, Vec_len(heap->vec));
}

Heap * Heap_new(int isize, int capacity)
{
    Heap * heap;

    if ((heap = calloc(1, sizeof(* heap))))
    {
        if ((heap->vec = Vec_new(isize, capacity)))
        {
            return heap;
        }

        free(heap);
    }

    return 0;
}

void Heap_del(Heap * heap)
{
    Vec_del(heap->vec);
    free(heap);
}

int Heap_count(Heap const * heap)
{
    return Vec_len(heap->vec);
}

static void _insert(Heap * heap, cmpf cmp, swapf swap, int idx)
{
    int     parent_idx;
    void *  parent;

    while (true)
    {
        if (idx == 0) return ;

        parent_idx = _parent_idx(idx);
        parent = _get(heap, parent_idx);

        if (cmp(parent, _get(heap, idx)) > 0)
        {
            swap(parent, _get(heap, idx));
        }
        else return ;

        idx = parent_idx;
    }
}

bool Heap_insert(Heap * heap, void const * item, cmpf cmp, swapf swap)
{
    if (! Vec_push(heap->vec, item)) return false;

    _insert(heap, cmp, swap, Vec_len(heap->vec) - 1);

    return true;
}

static int _min_idx(Heap const * heap, cmpf cmp, int lhs, int rhs)
{
    int res;

    res = cmp(_get(heap, lhs), _get(heap, rhs));

    return res < 0 ? lhs : rhs;
}

static void _propagate_down(Heap * heap, cmpf cmp, swapf swap, int idx, int nitems)
{
    int left_idx, right_idx, min_idx;

    left_idx = _left_idx(idx);
    right_idx = _right_idx(idx);

    if (left_idx >= nitems) return ;
    if (right_idx >= nitems)
    {
        min_idx = _min_idx(heap, cmp, idx, left_idx);
        if (min_idx == left_idx)
        {
            swap(_get(heap, left_idx), _get(heap, idx));
        }
    }
    else
    {
        min_idx = _min_idx(heap, cmp, left_idx, right_idx);
        min_idx = _min_idx(heap, cmp, idx, min_idx);
        if (min_idx != idx)
        {
            swap(_get(heap, min_idx), _get(heap, idx));

            return _propagate_down(heap, cmp, swap, min_idx, nitems);
        }
    }
}

void * Heap_pop(Heap * heap, cmpf cmp, swapf swap)
{
    void * top, * last;

    top = _get(heap, 0);
    last = _ins_spot(heap);

    swap(top, last);
    _propagate_down(heap, cmp, swap, 0, Vec_len(heap->vec) - 1);

    return Vec_pop(heap->vec);
}

void Heap_purge(Heap * heap)
{
    Vec_pop_all(heap->vec);
}