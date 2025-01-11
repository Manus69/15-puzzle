#include "Npuzzle.h"

//naive v
int Npuzzle_count_invs(Npuzzle const * np)
{
    int inv;
    int lhs, rhs;

    inv = 0;
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        lhs = sym_val(Npuzzle_at(np, k));
        if (lhs == 0) continue;

        for (int w = k + 1; w < NP_GRID_SIZE * NP_GRID_SIZE; w ++)
        {
            rhs = sym_val(Npuzzle_at(np, w));
            if (rhs == 0) continue;

            if (lhs > rhs) inv ++;
        }
    }

    return inv;
}

int Npuzzle_measure_disorder(Npuzzle const * np)
{
    int idx, count;

    count = 0;
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        // if (k == np->hole_idx) continue;

        idx = sym_target_idx(Npuzzle_at(np, k));
        count += idx != k;
    }

    return count;
}

int Npuzzle_measure_distance(Npuzzle const * np)
{
    int tot;
    int ds;

    tot = 0;
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        ds = abs(k - sym_target_idx(Npuzzle_at(np, k)));
        tot += ds;
    }

    return tot;
}

static int _ds(int row_col, u64 * seed)
{
    if      (row_col == 0) return 1;
    else if (row_col == NP_GRID_SIZE - 1) return -1;
    
    return rng_xor(seed) % 2 ? -1 : 1;
}

static char _random_dir(Npuzzle const * np, u64 * seed, int k)
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
    Npuzzle copy;
    u64     seed;
    char    dir;

    copy = * np;
    seed = GetTime() + len;

    for (int k = 0; k < len; k ++)
    {
        dir = _random_dir(& copy, & seed, k);
        Npuzzle_move_dir(& copy, dir);
        buff[k] = dir;
    }
}
