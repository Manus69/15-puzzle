#include "Solver.h"

#include <assert.h>
#include <string.h>

typedef struct
{
    Npuzzle np;
    int     disorder;
}   Pos;

$swapf_gen(Pos)

static int _Pos_cmpf(void const * _lhs, void const * _rhs)
{
    Pos * lhs;
    Pos * rhs;

    lhs = (Pos *) _lhs;
    rhs = (Pos *) _rhs;

    return lhs->disorder < rhs->disorder ? -1 : lhs->disorder > rhs->disorder;
}

static bool _Pos_from_np(Pos * pos, Npuzzle const * np, char dir, int (* metric)(Npuzzle const *))
{
    pos->np = * np;
    if (Npuzzle_move_dir_check(& pos->np, dir))
    {
        pos->disorder = metric(& pos->np);

        return true;
    }

    return false;
}

static void _add_to_queue(Solver * solver, Pos const * pos)
{
    bool res;

    res =  Heap_insert_check(& solver->heap, pos, _Pos_cmpf, Pos_swapf);
    assert(res);
}

static Pos _pop_queue(Solver * solver)
{
    assert(Heap_count(& solver->heap) > 0);

    return * (Pos *) Heap_pop_top(& solver->heap, _Pos_cmpf, Pos_swapf);
}

static bool _add_to_tbl(Solver * solver, Pos const * pos)
{
    bool res;

    if (Htbl_get(& solver->htbl, & pos->np, Npuzzle_hashf, Npuzzle_eqf)) return false;

    res = Htbl_insert_check(& solver->htbl, & pos->np, Npuzzle_hashf);
    assert(res);

    return true;
}

static bool _visit(Solver * solver, Pos const * pos, int (* metric)(Npuzzle const *))
{
    Pos next;

    if (Npuzzle_solved(& pos->np)) return true;
    for (int k = 0; k < NP_NDIRS; k ++)
    {
        if (_Pos_from_np(& next, & pos->np, NP_DIRS[k], metric))
        {
            if (_add_to_tbl(solver, & next))
            {
                _add_to_queue(solver, & next);
            }
        }
    }

    return false;
}

static int _solve(Solver * solver, int (* metric)(Npuzzle const *))
{
    Pos current;

    while (true)
    {
        current = _pop_queue(solver);
        if (_visit(solver, & current, metric)) return Htbl_count(& solver->htbl);
    }
}

int Solver_solve(Solver * solver, Npuzzle const * np)
{
    Pos pos;
    int (* metric)(Npuzzle const *) = Npuzzle_measure_distance;

    memset(solver->buff, 0, SOLVER_BS);
    Htbl_purge(& solver->htbl);
    Heap_pop_all(& solver->heap);

    pos = (Pos)
    {
        .np = * np,
        .disorder = metric(np),
    };

    _add_to_queue(solver, & pos);
    _add_to_tbl(solver, & pos);

    return _solve(solver, metric);
}

#define _DC (1 << 10)
bool Solver_init(Solver * solver)
{
    return ((Htbl_new_capacity(& solver->htbl, sizeof(Npuzzle), _DC)) &&
            (Heap_new_capacity(& solver->heap, sizeof(Pos), _DC)));
}

void Solver_deinit(Solver * solver)
{
    Htbl_del(& solver->htbl);
    Heap_del(& solver->heap);
}



