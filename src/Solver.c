#include "Solver.h"

#include <string.h>

typedef struct
{
    Npuzzle np;
    int     disorder;
}   Pos;

static bool _Pos_impossible(Pos const * pos)
{
    return pos->disorder == __INT_MAX__;
}

static int _Pos_cmpf(void const * _lhs, void const * _rhs)
{
    Pos * lhs;
    Pos * rhs;

    lhs = (Pos *) _lhs;
    rhs = (Pos *) _rhs;

    return lhs->disorder < rhs->disorder ? -1 : lhs->disorder > rhs->disorder;
}

static Pos _pos_from_np(Npuzzle const * np, char dir)
{
    Pos pos;

    pos.np = * np;
    if (Npuzzle_move_dir_check(& pos.np, dir))
    {
        pos.disorder = Npuzzle_measure_disorder(& pos.np);
    }
    else
    {
        pos.disorder = __INT_MAX__;
    }

    return pos;
}

static int _solve(Solver * solver, Pos const * pos, int idx)
{
    Pos next[NP_NDIRS];
    int pos_count;
    int sln;

    if (Npuzzle_solved(& pos->np)) return 0;
    // if (Htbl_get(solver->htbl, & pos->np, Npuzzle_hashf, Npuzzle_eqf)) return -1;
    if (Htbl_insert(solver->htbl, & pos->np, Npuzzle_hashf, Npuzzle_eqf) <= 0) return -1;

    pos_count = NP_NDIRS;
    for (int k = 0; k < NP_NDIRS; k ++)
    {
        next[k] = _pos_from_np(& pos->np, NP_DIRS[k]);
        if (_Pos_impossible(next + k)) pos_count --;
    }

    qsort(next, NP_NDIRS, sizeof(Pos), _Pos_cmpf);

    for (int k = 0; k < pos_count; k ++)
    {
        sln = _solve(solver, next + k, idx + 1);
        if (sln >= 0)
        {
            solver->buff[idx] = dir_idx_idx(Npuzzle_hole_idx(& pos->np), Npuzzle_hole_idx(& next[k].np));
            return 1 + sln;
        }
    }

    return -1;
}

int Solver_solve(Solver * solver, Npuzzle const * np)
{
    Pos pos;

    memset(solver->buff, 0, SOLVER_BS);

    pos = (Pos)
    {
        .np = * np,
        .disorder = Npuzzle_measure_disorder(np),
    };

    Htbl_insert(solver->htbl, np, Npuzzle_hashf, Npuzzle_eqf);

    return _solve(solver, & pos, 0);
}

bool Solver_init(Solver * solver)
{
    return (solver->htbl = Htbl_new(sizeof(Npuzzle)));
}

void Solver_deinit(Solver * solver)
{
    Htbl_del(solver->htbl);
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


