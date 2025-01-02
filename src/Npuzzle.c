#include "Npuzzle.h"

int Npuzzle_measure_disorder(Npuzzle const * np)
{
    int idx, count;

    count = 0;
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        idx = sym_target_idx(Npuzzle_at(np, k));
        count += idx > k;
    }

    return count;
}

static char _random_dir(Npuzzle const * np, unsigned * seed, int k)
{
    int idx, row, col;
    int drow, dcol;

    idx = Npuzzle_hole_idx(np);
    row = idx_row(idx);
    col = idx_col(idx);

    drow = 0;
    dcol = 0;

    if (k % 2)
    {
        if      (col == 0) dcol = 1;
        else if (col == NP_GRID_SIZE - 1) dcol = -1;
        else dcol = rng_xor(seed) % 2 ? -1 : 1;
    }
    else
    {
        if      (row == 0) drow = 1;
        else if (row == NP_GRID_SIZE - 1) drow = -1;
        else drow = rng_xor(seed) % 2 ? -1 : 1;
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