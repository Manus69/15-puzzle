#ifndef PROG_H
#define PROG_H

#include "Gui.h"
#include "Npuzzle.h"

typedef struct
{
    Gui     gui;
    Npuzzle np;
    bool    runs;
}   Prog;

bool Prog_init(Prog * prog);
void Prog_deinit(Prog * prog);
void Prog_input(Prog * prog);
void Prog_display(Prog const * prog);
void Prog_update(Prog * prog);

#endif