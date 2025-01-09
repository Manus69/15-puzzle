#include "Solver.h"

#include <string.h>

typedef struct
{
    Npuzzle np;
    int     disorder;
}   Pos;

static int _Pos_cmpf(void const * _lhs, void const * _rhs)
{
    Pos * lhs;
    Pos * rhs;

    lhs = (Pos *) _lhs;
    rhs = (Pos *) _rhs;

    return lhs->disorder < rhs->disorder ? -1 : lhs->disorder > rhs->disorder;
}

static void _Pos_swapf(void * lhs, void * rhs)
{
    $swap(Pos, lhs, rhs);
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

static int _visit(Solver * solver, Pos const * pos, int (* metric)(Npuzzle const *))
{
    Pos next;
    int count;

    count = 0;
    for (int k = 0; k < NP_NDIRS; k ++)
    {
        if (_Pos_from_np(& next, & pos->np, NP_DIRS[k], metric))
        {
            if (Htbl_insert(solver->htbl, & next.np, Npuzzle_hashf, Npuzzle_eqf) > 0)
            {
                if (! Heap_insert(solver->heap, & next, _Pos_cmpf, _Pos_swapf))
                {
                    return -1;
                }

                count ++;
            }
        }
    }

    return count;
}

static int _solve(Solver * solver, int (* metric)(Npuzzle const *))
{
    Pos * pos;

    while (true)
    {
        if (! Heap_count(solver->heap)) return -1;
        pos = Heap_pop(solver->heap, _Pos_cmpf, _Pos_swapf);

        if (Npuzzle_solved(& pos->np))  return 0;

        _visit(solver, pos, metric);
    }
}

int Solver_solve(Solver * solver, Npuzzle const * np)
{
    Pos pos;
    int (* metric)(Npuzzle const *) = Npuzzle_measure_distance;

    memset(solver->buff, 0, SOLVER_BS);
    Htbl_purge(solver->htbl);
    Heap_purge(solver->heap);

    pos = (Pos)
    {
        .np = * np,
        .disorder = metric(np),
    };

    _visit(solver, & pos, metric);

    return _solve(solver, metric);
}

#define _DC (1 << 10)
bool Solver_init(Solver * solver)
{
    return ((solver->htbl = Htbl_new(sizeof(Npuzzle), _DC)) &&
            (solver->heap = Heap_new(sizeof(Pos), _DC)));
}

void Solver_deinit(Solver * solver)
{
    Htbl_del(solver->htbl);
    Heap_del(solver->heap);
}

int Solver_npos(Solver const * solver)
{
    return Htbl_count(solver->htbl);
}

//123456709ab8defc

void Solver_test(Solver * solver)
{
    Npuzzle np;

    Npuzzle_init(& np, "123456709ab8defc");
    Htbl_insert(solver->htbl, & np, Npuzzle_hashf, Npuzzle_eqf);
    Htbl_insert(solver->htbl, & np, Npuzzle_hashf, Npuzzle_eqf);


}

bool Solver_test_not_visited(Solver * solver, Npuzzle const * np)
{
    // void * ptr;

    // ptr = Htbl_get(solver->htbl, np, Npuzzle_hashf, Npuzzle_eqf);

    // return ptr;

    return Htbl_insert(solver->htbl, np, Npuzzle_hashf, Npuzzle_eqf) > 0;
}


