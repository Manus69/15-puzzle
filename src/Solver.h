#ifndef SOLVER_H
#define SOLVER_H

#include "Npuzzle.h"

#define SOLVER_BS (1 << 6)

typedef struct
{
    char buff[SOLVER_BS]; 
}   Solver;

static inline int Solver_solve(Solver * solver, Npuzzle const * np)
{
    int sln_len;

    * solver = (Solver) {};

    sln_len = Npuzzle_solve(np, solver->buff, SOLVER_BS - 1);

    return sln_len;
}

#endif