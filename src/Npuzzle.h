#ifndef NPUZZLE_H
#define NPUZZLE_H

#include "master.h"

typedef struct
{
    char    vals[GRID_SIZE * GRID_SIZE];
    int     hole_idx;
}   Npuzzle;

static inline const char * Npuzzle_cstr(Npuzzle const * np)
{
    return np->vals;
}

static inline char Npuzzle_at(Npuzzle const * np, int idx)
{
    return np->vals[idx];
}

static inline char Npuzzle_at_rc(Npuzzle const * np, int row, int col)
{
    return Npuzzle_at(np, row_col_idx(row, col));
}

static inline void Npuzzle_swap(Npuzzle * np, int k)
{
    $swap(& np->vals[np->hole_idx], & np->vals[k], char);
    np->hole_idx = k;
}

static inline void Npuzzle_move(Npuzzle * np, int drow, int dcol)
{
    int idx;

    idx = row_col_idx(idx_row(np->hole_idx) + drow, idx_col(np->hole_idx) + dcol);
    Npuzzle_swap(np, idx);
}

static inline bool Npuzzle_move_check(Npuzzle * np, int drow, int dcol)
{
    int row, col;

    row = idx_row(np->hole_idx) + drow;
    col = idx_col(np->hole_idx) + dcol;

    if (row_valid(row) && col_valid(col))
    {
        Npuzzle_move(np, drow, dcol);
        return true;
    }

    return false;
}

void Npuzzle_init(Npuzzle * np, char const * cstr);
void Npuzzle_move_dir(Npuzzle * np, char dir);
bool Npuzzle_move_dir_check(Npuzzle * np, char dir);


#endif