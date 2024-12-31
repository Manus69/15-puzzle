#include "Npuzzle.h"

int Npuzzle_measure_disorder(Npuzzle const * np)
{
    int val, idx;

    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        val = sym_val(Npuzzle_at(np, k));
        idx = sym_target_idx(Npuzzle_at(np, k));

        
    }
}