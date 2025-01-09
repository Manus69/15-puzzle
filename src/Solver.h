#ifndef SOLVER_H
#define SOLVER_H

#include "Npuzzle.h"

#define SOLVER_BS (1 << 8)

typedef struct
{
    char    buff[SOLVER_BS];
    Htbl *  htbl;
    Heap * heap;
}   Solver;

int     Solver_solve(Solver * solver, Npuzzle const * np);
bool    Solver_init(Solver * solver);
void    Solver_deinit(Solver * solver);
//
int     Solver_npos(Solver const * solver);
bool    Solver_test_not_visited(Solver * solver, Npuzzle const * np);
void    Solver_test(Solver * solver);

#endif