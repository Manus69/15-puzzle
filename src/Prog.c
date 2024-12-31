#include "Prog.h"

#define CSTR_DFLT   "123456789abcdef0"
#define NCYCLES     10

bool Prog_start(Prog * prog)
{
    * prog = (Prog) {};

    if (! Gui_init(& prog->gui)) return false;

    Npuzzle_init(& prog->np, CSTR_DFLT);
    Gui_grid_set(& prog->gui, Npuzzle_cstr(& prog->np));
    prog->runs = true;

    return true;
}

void Prog_stop(Prog * prog)
{
    Gui_deinit(& prog->gui);
}

static void _try_dir(Prog * prog, char dir)
{
    int from, to;

    from = Npuzzle_hole_idx(& prog->np);
    if (Npuzzle_move_dir_check(& prog->np, dir))
    {
        to = sym_val(Npuzzle_at(& prog->np, from));
        Gui_grid_move(& prog->gui, to, NCYCLES);
    }

    if (Npuzzle_solved(& prog->np))
    {
        printf("solved\n");
    }
}

void Prog_input(Prog * prog)
{
    if (WindowShouldClose())
    {
        prog->runs = false;
        return ;
    }

    if (Gui_grid_in_animation(& prog->gui)) return ;

    if (IsKeyPressed(KEY_UP))       return _try_dir(prog, 'd');
    if (IsKeyPressed(KEY_RIGHT))    return _try_dir(prog, 'l');
    if (IsKeyPressed(KEY_DOWN))     return _try_dir(prog, 'u');
    if (IsKeyPressed(KEY_LEFT))     return _try_dir(prog, 'r');

}

void Prog_update(Prog * prog)
{
    Gui_update(& prog->gui);
}

void Prog_display(Prog * prog)
{
    Gui_draw(& prog->gui);
}

int Prog_error(Prog * prog, char const * msg)
{
    (void) prog;

    printf("%s\n", msg);

    return 1;
}