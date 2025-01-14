#include "Prog.h"
#include "dbg.h"

#include <assert.h>

#define CSTR_DFLT   "123456789abcdef0"
#define NCYCLES     (10)
#define BUFF_SIZE   (RBUFF_SIZE)
#define SCRAMBLELN  (1 << 5)

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
    Solver_deinit(& prog->solver);
    Gui_deinit(& prog->gui);
}

static void _try_dir(Prog * prog, char dir)
{
    int from, to;

    from = Npuzzle_hole_idx(& prog->np);
    if (Npuzzle_move_dir_check(& prog->np, dir))
    {
        to = sym_val(Npuzzle_at(& prog->np, from));
        Gui_grid_move(& prog->gui, to, prog->anm.speed);
    }
}

static bool _Prog_queue_action(Prog * prog, char x, int speed)
{
    if (Rbuff_full(& prog->action_buff)) return false;

    prog->anm.speed = speed;
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

    for (int k = 0; k < nmoves; k ++)
    {
        _Prog_queue_action(prog, buff[k], NCYCLES / 2);
    }
}

static void _solve_dbg(Prog * prog)
{
    int len;

    len = Solver_solve(& prog->solver, & prog->np);

    assert(len >= 0);

    printf("%d\n\n", len);

    Npuzzle_init(& prog->np, CSTR_DFLT);
    Gui_grid_set(& prog->gui, CSTR_DFLT);
}

static void _solve(Prog * prog)
{
    (void) _solve_dbg;

    int len;

    len = Solver_solve(& prog->solver, & prog->np);
        
    if (len < 0) return dbg_msg("Failed to solve");

    for (int k = 0; k < len; k ++)
    {
        _Prog_queue_action(prog, prog->solver.buff[len - k - 1], NCYCLES / 3);
    }
}

static void _pause_toggle(Prog * prog)
{
    prog->anm.pause = ! prog->anm.pause;
}

static void _input_reg(Prog * prog, int key)
{
    if (key == KEY_SPACE) return _pause_toggle(prog);
    if (! Rbuff_empty(& prog->action_buff)) return ;

    if      (key == KEY_UP)      _Prog_queue_action(prog, 'd', NCYCLES);
    else if (key == KEY_RIGHT)   _Prog_queue_action(prog, 'l', NCYCLES);
    else if (key == KEY_DOWN)    _Prog_queue_action(prog, 'u', NCYCLES);
    else if (key == KEY_LEFT)    _Prog_queue_action(prog, 'r', NCYCLES);
    else if (key == KEY_X)       _scramble(prog, $min(SCRAMBLELN, BUFF_SIZE));
    else if (key == KEY_S)       _solve(prog);
}

static void _input_paused(Prog * prog, int key)
{
    if      (key == KEY_SPACE)  return _pause_toggle(prog);
    else if (key == KEY_UP || key == KEY_RIGHT || key == KEY_DOWN || key == KEY_LEFT || 
             key == KEY_X || key == KEY_S)
    {
        _pause_toggle(prog);
        Rbuff_pop_all(& prog->action_buff);

        return _input_reg(prog, key);
    }
}

static bool _click_check(Prog const * prog, GuiClck const * click)
{
    return  Npuzzle_hole_idx(& prog->np) != click->idx && 
            Npuzzle_hole_in_dir(& prog->np, click->idx, click->pdir);
}

static void _board_click(Prog * prog, GuiClck const * click)
{
    if (Rbuff_empty(& prog->action_buff))
    {
        if (_click_check(prog, click)) _Prog_queue_action(prog, dir_rev(click->pdir), NCYCLES);

        return ;
    }

    if (prog->anm.pause)
    {
        if (_click_check(prog, click))
        {
            Rbuff_pop_all(& prog->action_buff);
            _Prog_queue_action(prog, dir_rev(click->pdir), NCYCLES);
            _pause_toggle(prog);
        }
    }
    else _pause_toggle(prog);
}

static void _input_click(Prog * prog)
{
    Vector2 xy;
    GuiClck click;

    xy = GetMousePosition();
    click = Gui_click(& prog->gui, xy);

    if (click.idx == NO_IDX) return ;
    
    _board_click(prog, & click);
}

void Prog_input(Prog * prog)
{
    int key;

    if (WindowShouldClose()) { prog->runs = false; return ; }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return _input_click(prog);
    if (IsKeyPressed(KEY_M)) return Gui_mute_toggle(& prog->gui);
    
    key = GetKeyPressed();

    if (prog->anm.pause) return _input_paused(prog, key);
    else return _input_reg(prog, key);
}

void Prog_update(Prog * prog)
{
    if (! prog->anm.pause) _Prog_perform_action(prog);

    Gui_update(& prog->gui);
}

void Prog_display(Prog * prog)
{
    Gui_draw(& prog->gui);
    Gui_play_sound(& prog->gui);
}

int Prog_error(Prog * prog, char const * msg)
{
    (void) prog;

    printf("%s\n", msg);

    return 1;
}