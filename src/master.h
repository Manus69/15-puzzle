#ifndef MASTER_H
#define MASTER_H

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib.h"

#define NO_IDX         (-1)
#define NP_GRID_SIZE   (4)
#define NP_SYM_CSTR    "0123456789abcdef"
#define NP_DIRS        "urdl"
#define NP_NDIRS       (4)


static inline int row_col_idx(int row, int col)
{
    return row * NP_GRID_SIZE + col;
}

static inline int idx_row(int idx)
{
    return idx / NP_GRID_SIZE;
}

static inline int idx_col(int idx)
{
    return idx % NP_GRID_SIZE;
}

static inline bool row_col_valid(int row_col)
{
    return row_col >= 0 && row_col < NP_GRID_SIZE;
}

static inline int sym_val(char x)
{
    if (x >= '0' && x <= '9') return x - '0';

    return x - 'a' + 10;
}

static inline char val_sym(int val)
{
    return NP_SYM_CSTR[val];
}

static inline int sym_target_idx(char sym)
{
    if (sym == '0') return NP_GRID_SIZE * NP_GRID_SIZE - 1;

    return sym_val(sym) - 1;
}

static inline int dir_drow(char dir)
{
    static char drow[] = 
    {
        ['u'] = -1,
        ['d'] = 1,
        ['z'] = 0,
    };

    return drow[(int) dir];
}

static inline char drow_dir(int drow)
{
    return (drow == -1) * 'u' + (drow == 1) * 'd';
}

static inline int dir_dcol(char dir)
{
    static char dcol[] =
    {
        ['l'] = -1,
        ['r'] = 1,
        ['z'] = 0,
    };

    return dcol[(int) dir];
}

static inline char dcol_dir(int dcol)
{
    return (dcol == -1) * 'l' + (dcol == 1) * 'r';
}

static inline char dir_idx_idx(int from, int to)
{
    int drow;
    int dcol;

    drow = idx_row(to) - idx_row(from);
    dcol = idx_col(to) - idx_col(from);

    return drow_dir(drow) + dcol_dir(dcol); 
}

static inline char dir_rev(char dir)
{
    static char rev[] =
    {
        ['u'] = 'd',
        ['r'] = 'l',
        ['d'] = 'u',
        ['l'] = 'r',
    };

    return rev[(int) dir];
}



#endif