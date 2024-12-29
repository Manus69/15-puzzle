#include "Prog.h"

#define WW 800
#define WH 800

bool Prog_init(Prog * prog)
{
    * prog = (Prog) {};

    InitWindow(WW, WH, 0);
    SetTargetFPS(60);

    if (! Gui_init(& prog->gui, WW, WH)) return false;

    prog->runs = true;

    return true;
}

void Prog_deinit(Prog * prog)
{
    Gui_deinit(& prog->gui);
    CloseWindow();
}