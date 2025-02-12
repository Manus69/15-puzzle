#ifndef NPUZZLE_H
#define NPUZZLE_H

#include "master.h"

typedef struct
{
    char vals[NP_GRID_SIZE * NP_GRID_SIZE];
    char hole_idx;
}   Npuzzle;

static inline char const * Npuzzle_cstr(Npuzzle const * np)
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

static inline int Npuzzle_hole_idx(Npuzzle const * np)
{
    return np->hole_idx;
}

static inline void Npuzzle_swap_hole(Npuzzle * np, int idx)
{
    $swap(char, & np->vals[idx], & np->vals[Npuzzle_hole_idx(np)]);
    np->hole_idx = idx;
}

static inline void Npuzzle_move(Npuzzle * np, int drow, int dcol)
{
    int idx;

    idx = row_col_idx(idx_row(np->hole_idx) + drow, idx_col(np->hole_idx) + dcol);
    Npuzzle_swap_hole(np, idx);
}

static inline bool Npuzzle_move_check(Npuzzle * np, int drow, int dcol)
{
    int row, col, idx;

    row = idx_row(np->hole_idx) + drow;
    col = idx_col(np->hole_idx) + dcol;

    if (row_col_valid(row) && row_col_valid(col))
    {
        idx = row_col_idx(row, col);
        Npuzzle_swap_hole(np, idx);

        return true;
    }

    return false;
}

static inline void Npuzzle_move_dir(Npuzzle * np, char dir)
{
    return Npuzzle_move(np, dir_drow(dir), dir_dcol(dir));
}

static inline bool Npuzzle_move_dir_check(Npuzzle * np, char dir)
{
    int drow, dcol;

    drow = dir_drow(dir);
    dcol = dir_dcol(dir);

    return Npuzzle_move_check(np, drow, dcol);
}

static inline void Npuzzle_init(Npuzzle * np, char const * cstr)
{
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        np->vals[k] = cstr[k];
        if (cstr[k] == '0') np->hole_idx = k;
    }
}

static inline bool Npuzzle_adjacent(Npuzzle const * np, int idx)
{
    int hole;
    int drow;
    int dcol;

    hole = Npuzzle_hole_idx(np);
    if (hole == idx) return false;
    
    drow = abs(idx_row(hole) - idx_row(idx));
    dcol = abs(idx_col(hole) - idx_col(idx));

    return (drow <= 1 && dcol == 0) || (dcol <= 1 && drow == 0);
}

static inline bool Npuzzle_hole_in_dir(Npuzzle const * np, int idx, char dir)
{
    return Npuzzle_hole_idx(np) == idx_dir(idx, dir);
}

static inline bool Npuzzle_solved(Npuzzle const * np)
{
    if (np->hole_idx != NP_GRID_SIZE * NP_GRID_SIZE - 1) return false;

    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE - 1; k ++)
    {
        if (Npuzzle_at(np, k) != val_sym(k + 1)) return false;
    }

    return true;
}

static inline u64 Npuzzle_hash(Npuzzle const * np)
{
    return hash_djb((unsigned char const *) np->vals, NP_GRID_SIZE * NP_GRID_SIZE);
}

static inline u64 Npuzzle_hashf(void const * ptr)
{
    return Npuzzle_hash(ptr);
}

static inline bool Npuzzle_eq(Npuzzle const * lhs, Npuzzle const * rhs)
{
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        if (Npuzzle_at(lhs, k) != Npuzzle_at(rhs, k)) return false;
    }

    return true;
}

static inline bool Npuzzle_eqf(void const * lhs, void const * rhs)
{
    return Npuzzle_eq(lhs, rhs);
}

int     Npuzzle_measure_disorder(Npuzzle const * np);
int     Npuzzle_count_invs(Npuzzle const * np);
int     Npuzzle_measure_distance(Npuzzle const * np);
void    Npuzzle_scramble_seq(Npuzzle const * np, char * buff, int len);

#endif