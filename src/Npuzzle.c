#include "Npuzzle.h"

#include <string.h>

void Npuzzle_init(Npuzzle * np, char const * cstr)
{
    for (int k = 0; k < GRID_SIZE * GRID_SIZE; k ++)
    {
        np->vals[k] = cstr[k];
        if (cstr[k] == '0') np->hole_idx = k;
    }
}

void Npuzzle_move_dir(Npuzzle * np, char dir)
{
    return Npuzzle_move(np, dir_drow(dir), dir_dcol(dir));
}

bool Npuzzle_move_dir_check(Npuzzle * np, char dir)
{
    return Npuzzle_move_check(np, dir_drow(dir), dir_dcol(dir));
}