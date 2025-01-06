#ifndef SOLVER_H
#define SOLVER_H

#include "Npuzzle.h"

#define SOLVER_BS (1 << 6)

typedef struct
{
    char    buff[SOLVER_BS];
    Htbl *  htbl;
}   Solver;

int     Solver_solve(Solver * solver, Npuzzle const * np);
bool    Solver_init(Solver * solver);
void    Solver_deinit(Solver * solver);


#endif