#ifndef MASTER_H
#define MASTER_H

#include <raylib.h>
#include <stdio.h>
#include <assert.h>

#define GRID_SIZE 4

typedef struct
{
    int a;
    int b;
}   int_int;

static inline int parse_symbol(char x)
{
    if (x >= '0' && x <= '9') return x - '0';

    return 10 + (x - 'a');
}

static inline int row_col_idx(int row, int col)
{
    return row * GRID_SIZE + col;
}

static inline int idx_row(int idx)
{
    return idx / GRID_SIZE;
}

static inline int idx_col(int idx)
{
    return idx % GRID_SIZE;
}

static inline bool row_valid(int row)
{
    return row >= 0 && row < GRID_SIZE;
}

static inline bool col_valid(int col)
{
    return row_valid(col);
}

static inline int dir_drow(char dir)
{
    static const char _drow[] =
    {
        ['u'] = -1,
        ['d'] = 1,
        ['z'] = 0,
    };

    return _drow[(int) dir];
}

static inline int dir_dcol(char dir)
{
    static const char _dcol[] =
    {
        ['l'] = -1,
        ['r'] = 1,
        ['z'] = 0,
    };
    
    return _dcol[(int) dir];
}

#define $swap(lhs, rhs, T) \
{ \
    T _t = * (T *) (lhs); \
    * (T *) (lhs) = * (T *) (rhs); \
    * (T *) (rhs) = _t; \
}

#endif