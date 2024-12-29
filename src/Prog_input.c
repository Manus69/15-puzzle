
#include "Prog.h"

#define NCYCLES 10

static bool _input_locked(Prog * prog)
{
    return Grid_in_motion(& prog->gui.grid);
}

void Prog_input(Prog * prog)
{
    if (WindowShouldClose()) prog->runs = false;
    if (_input_locked(prog)) return ;

    if (IsKeyPressed(KEY_UP))
    {
        Gui_move(& prog->gui, 1, 0, NCYCLES);
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        Gui_move(& prog->gui, 0, -1, NCYCLES);
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        Gui_move(& prog->gui, -1, 0, NCYCLES);
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        Gui_move(& prog->gui, 0, 1, NCYCLES);
    }
}