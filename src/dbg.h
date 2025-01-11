#ifndef DBG_H
#define DBG_H

#include "Prog.h"

#include <stdio.h>

static inline void dbg_msg(char const * msg)
{
    printf("-----\nDBG %s\n", msg);
}

static inline void dbg_Npuzzle(Npuzzle const * np, bool metrics)
{
    dbg_msg("pos:");
    printf("%.*s\n", NP_GRID_SIZE * NP_GRID_SIZE, Npuzzle_cstr(np));

    if (metrics)
    {
        printf("disorder :  %d\n", Npuzzle_measure_disorder(np));
        printf("invs :      %d\n", Npuzzle_count_invs(np));
        printf("distance : %d\n\n", Npuzzle_measure_distance(np));
    }
}

#endif