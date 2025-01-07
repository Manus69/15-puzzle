#ifndef DBG_H
#define DBG_H

#include "Npuzzle.h"
#include "Solver.h"

#include <stdio.h>

static inline void _dbg_msg(char const * msg)
{
    printf("-----\nDBG %s\n", msg);
}

static inline void dbg_Npuzzle(Npuzzle const * np)
{
    _dbg_msg("pos:");
    printf("%.*s\n", NP_GRID_SIZE * NP_GRID_SIZE, Npuzzle_cstr(np));
}

static inline void dbg_Solver(Solver const * solver)
{
    _dbg_msg("solver:");
    printf("%d\n", Solver_npos(solver));
}

#endif