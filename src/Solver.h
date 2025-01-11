#ifndef SOLVER_H
#define SOLVER_H

#include "Npuzzle.h"
#include LIB_PATH

#define SOLVER_BS (1 << 10)

typedef struct
{
    char    buff[SOLVER_BS];
    Vec     np_vec;
    Vec     metric_vec;
    Vec     parent_vec;
    Htbl    np_tbl;
    Heap    queue;
}   Solver;

int     Solver_solve(Solver * solver, Npuzzle const * np);
bool    Solver_init(Solver * solver);
void    Solver_deinit(Solver * solver);

#endif