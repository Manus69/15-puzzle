#ifndef PROG_H
#define PROG_H

#include "Gui.h"
#include "Npuzzle.h"
#include "Solver.h"

typedef struct
{
    Gui     gui;
    Npuzzle np;
    Rbuff   action_buff;
    Solver  solver;
    int     speed;
    bool    runs;
}   Prog;

bool    Prog_start(Prog * prog);
void    Prog_stop(Prog * prog);
void    Prog_input(Prog * prog);
void    Prog_update(Prog * prog);
void    Prog_display(Prog * prog);
int     Prog_error(Prog * prog, char const * msg);


#endif