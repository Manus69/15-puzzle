#include "Npuzzle.h"

int Npuzzle_measure_disorder(Npuzzle const * np)
{
    int idx, count;

    count = 0;
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        idx = sym_target_idx(Npuzzle_at(np, k));
        count += idx < k;
    }

    return count;
}

static int _ds(int row_col, unsigned * seed)
{
    if      (row_col == 0) return 1;
    else if (row_col == NP_GRID_SIZE - 1) return -1;
    
    return rng_xor(seed) % 2 ? -1 : 1;
}

static char _random_dir(Npuzzle const * np, unsigned * seed, int k)
{
    int idx, row, col;
    int drow, dcol;

    idx = Npuzzle_hole_idx(np);
    row = idx_row(idx);
    col = idx_col(idx);

    if (k % 2)
    {
        drow = 0;
        dcol = _ds(col, seed);
    }
    else
    {
        drow = _ds(row, seed);
        dcol = 0;
    }

    return dcol_dir(dcol) + drow_dir(drow);
}

void Npuzzle_scramble_seq(Npuzzle const * np, char * buff, int len)
{
    Npuzzle     copy;
    unsigned    seed;
    char        dir;

    copy = * np;
    seed = GetTime() + len;

    for (int k = 0; k < len; k ++)
    {
        dir = _random_dir(& copy, & seed, k);
        Npuzzle_move_dir(& copy, dir);
        buff[k] = dir;
    }
}

typedef struct
{
    Npuzzle np;
    int     disorder;
    int     parent_idx;
}   Pos;

static bool _Pos_impossible(Pos const * pos)
{
    return pos->disorder == __INT_MAX__;
}

static int _Pos_cmp(void const * _lhs, void const * _rhs)
{
    Pos * lhs;
    Pos * rhs;

    lhs = (Pos *) _lhs;
    rhs = (Pos *) _rhs;

    return lhs->disorder < rhs->disorder ? -1 : lhs->disorder > rhs->disorder;
}

static int _solve(Pos const * pos, char * buff, int len)
{
    Pos next[NP_NDIRS];
    int npos;

    if (_Pos_impossible(pos))       return NO_IDX;
    if (len == 0)                   return NO_IDX;
    if (Npuzzle_solved(& pos->np))  return 0;

    npos = NP_NDIRS;

    for (int k = 0; k < NP_NDIRS; k ++)
    {
        next[k].np = pos->np;
        next[k].parent_idx = Npuzzle_hole_idx(& pos->np);

        if (Npuzzle_move_dir_check(& next[k].np, NP_DIRS[k]))
        {
            if (Npuzzle_hole_idx(& next[k].np) == pos->parent_idx) next[k].disorder = __INT_MAX__ - 1;
            else next[k].disorder = Npuzzle_measure_disorder(& next[k].np);
        }
        else
        {
            next[k].disorder = __INT_MAX__;
            npos --;
        }
    }

    qsort(next, NP_NDIRS, sizeof(Pos), _Pos_cmp);

    int     sln_len;
    char    dir;

    for (int k = 0; k < npos; k ++)
    {
        sln_len = _solve(next + k, buff + 1, len - 1);
        if (sln_len >= 0)
        {
            dir = dir_idx_idx(Npuzzle_hole_idx(& pos->np), Npuzzle_hole_idx(& next[k].np));
            * buff = dir;

            return sln_len + 1;
        }
    }

    return NO_IDX;
}

int Npuzzle_solve(Npuzzle const * np, char * buff, int len)
{
    Pos pos;

    pos = (Pos)
    {
        .np = * np,
        .parent_idx = NO_IDX,
    };

    return _solve(& pos, buff, len);
}