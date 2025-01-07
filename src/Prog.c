#include "Prog.h"
#include "sln.h"
#include "dbg.h"

#define CSTR_DFLT   "123456789abcdef0"
#define NCYCLES     (10)
#define BUFF_SIZE   (RBUFF_SIZE)

bool Prog_start(Prog * prog)
{
    * prog = (Prog) {};

    if (! Gui_init(& prog->gui))        return false;
    if (! Solver_init(& prog->solver))  return false;

    Npuzzle_init(& prog->np, CSTR_DFLT);
    Gui_grid_set(& prog->gui, Npuzzle_cstr(& prog->np));
    prog->runs = true;

    return true;
}

void Prog_stop(Prog * prog)
{
    Gui_deinit(& prog->gui);
    Solver_deinit(& prog->solver);
}

static void _try_dir(Prog * prog, char dir)
{
    int from, to;

    from = Npuzzle_hole_idx(& prog->np);
    if (Npuzzle_move_dir_check(& prog->np, dir))
    {
        to = sym_val(Npuzzle_at(& prog->np, from));
        Gui_grid_move(& prog->gui, to, prog->speed);
    }

    //
    // if (Npuzzle_solved(& prog->np))
    // {
    //     printf("solved\n");
    // }
    //
    // printf("%d\n", Npuzzle_measure_disorder(& prog->np));
}

static bool _Prog_queue_action(Prog * prog, char x, int speed)
{
    if (Rbuff_full(& prog->action_buff)) return false;

    prog->speed = speed;
    Rbuff_pushc(& prog->action_buff, x);

    return true;
}

static bool _Prog_perform_action(Prog * prog)
{
    if (Gui_grid_in_animation(& prog->gui)) return false;
    if (Rbuff_empty(& prog->action_buff))   return false;

    _try_dir(prog, Rbuff_pop(& prog->action_buff));


    return true;
}

static void _scramble(Prog * prog, int nmoves)
{
    char buff[BUFF_SIZE] = {};

    if (nmoves >= BUFF_SIZE) return ;
    Npuzzle_scramble_seq(& prog->np, buff, nmoves);

    //
    // printf("%s\n", buff);

    for (int k = 0; k < nmoves; k ++)
    {
        _Prog_queue_action(prog, buff[k], NCYCLES / 2);
    }
}

static void _solve(Prog * prog)
{
    int len;

    len = Solver_solve(& prog->solver, & prog->np);

    if (len >= 0) 
    {
        printf("Solution: %s\n", prog->solver.buff);
    }
    else
    {
        printf("Failed to solve\n");
    }
}

static void _test(Prog * prog)
{
//
    if (Solver_test_not_visited(& prog->solver, & prog->np))
    {
        dbg_Npuzzle(& prog->np);
        dbg_Solver(& prog->solver);
        // Solver_test(& prog->solver);
    }
}

void Prog_input(Prog * prog)
{
    if (WindowShouldClose()) { prog->runs = false; return ; }
    if (Gui_grid_in_animation(& prog->gui)) return ;

    if (GetKeyPressed())
    {
        // _test(prog);
    }

    if      (IsKeyPressed(KEY_UP))      _Prog_queue_action(prog, 'd', NCYCLES);
    else if (IsKeyPressed(KEY_RIGHT))   _Prog_queue_action(prog, 'l', NCYCLES);
    else if (IsKeyPressed(KEY_DOWN))    _Prog_queue_action(prog, 'u', NCYCLES);
    else if (IsKeyPressed(KEY_LEFT))    _Prog_queue_action(prog, 'r', NCYCLES);
    else if (IsKeyPressed(KEY_SPACE))   _scramble(prog, BUFF_SIZE - 1);
    else if (IsKeyPressed(KEY_S))       _solve(prog);
}

void Prog_update(Prog * prog)
{
    _Prog_perform_action(prog);
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