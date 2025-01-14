#include "Solver.h"
#include "dbg.h"

#include <assert.h>
#include <string.h>

typedef struct
{
    int id;
    int metric;
}   Elem;

$swapf_gen(Elem)

static inline int Elem_cmpf(void const * lhs, void const * rhs)
{
    Elem const * _lhs, * _rhs;

    _lhs = lhs;
    _rhs = rhs;

    return _lhs->metric < _rhs->metric ? -1 : _lhs->metric > _rhs->metric;
}

typedef struct
{
    int         id;
    int         parent_id;
    int         metric;
}   Pos;

typedef int (* metricf)(Npuzzle const *);

static inline int _cum_metric(Npuzzle const * np)
{
    // return Npuzzle_measure_disorder(np);
    return Npuzzle_measure_distance(np);
}

static Pos _Solver_pos(Solver const * solver, int id)
{
    return (Pos)
    {
        .id = id,
        .metric = $drf(int) Vec_get(& solver->metric_vec, id),
        .parent_id = $drf(int) Vec_get(& solver->parent_vec, id),
    };
}

static Npuzzle * _Solver_np(Solver const * solver, int id)
{
    return Vec_get(& solver->np_vec, id);
}

static void _Solver_add(Solver * solver, Npuzzle const * np, int parent_id)
{
    Elem    elem;
    int     id;
    int     met;

    id = Vec_len(& solver->np_vec);
    met = _cum_metric(np);

    elem = (Elem)
    {
        .id = id,
        .metric = met,
    };

    assert(Heap_insert_check(& solver->queue, & elem, Elem_cmpf, Elem_swapf));
    assert(Htbl_insert_check(& solver->np_tbl, np, Npuzzle_hashf));
    assert(Vec_push_check(& solver->metric_vec, & met));
    assert(Vec_push_check(& solver->np_vec, np));
    assert(Vec_push_check(& solver->parent_vec, & parent_id));
}

static bool _Solver_check_visited(Solver const * solver, Npuzzle const * np)
{
    return Htbl_get(& solver->np_tbl, np, Npuzzle_hashf, Npuzzle_eqf);
}

static Pos _Solver_peek_top(Solver * solver)
{
    Elem    elm;
    Pos     pos;

    elm = $drf(Elem) Heap_top(& solver->queue);
    pos = _Solver_pos(solver, elm.id);

    return pos;
}

static void _Solver_pop_top(Solver * solver)
{
    Heap_pop_top(& solver->queue, Elem_cmpf, Elem_swapf);
}

static int _Solver_backtrack(Solver * solver)
{
    int     len;
    char    dir;
    Pos     pos;
    Pos     parent;

    len = 0;
    pos = _Solver_peek_top(solver);

    while (true)
    {
        if (len >= SOLVER_BS)           return NO_IDX;
        if (pos.parent_id == NO_IDX)    return len;

        parent = _Solver_pos(solver, pos.parent_id);
        dir = dir_idx_idx(  Npuzzle_hole_idx(_Solver_np(solver, parent.id)), 
                            Npuzzle_hole_idx(_Solver_np(solver, pos.id)));
        
        solver->buff[len] = dir;
        len ++;
        pos = parent;
    }
}

static void _Solver_next(Solver * solver, Pos const * pos, char dir)
{
    Npuzzle np;

    np = $drf(Npuzzle) _Solver_np(solver, pos->id);

    if (! Npuzzle_move_dir_check(& np, dir)) return ;
    if (_Solver_check_visited(solver, & np)) return ;
    
    _Solver_add(solver, & np, pos->id);
}

static bool _Solver_visit(Solver * solver)
{
    Pos         pos;
    Npuzzle *   np;

    pos = _Solver_peek_top(solver);
    np = _Solver_np(solver, pos.id);

    if (Npuzzle_solved(np)) return true;

    _Solver_pop_top(solver);

    for (int k = 0; k < NP_NDIRS; k ++)
    {
        _Solver_next(solver, & pos, NP_DIRS[k]);
    }

    return false;
}

static void _Solver_pop_all(Solver * solver)
{
    Vec_pop_all(& solver->metric_vec);
    Vec_pop_all(& solver->np_vec);
    Vec_pop_all(& solver->parent_vec);
    Heap_pop_all(& solver->queue);
    Htbl_purge(& solver->np_tbl);
}

int Solver_solve(Solver * solver, Npuzzle const * np)
{
    memset(solver->buff, 0, SOLVER_BS);
    _Solver_pop_all(solver);
    _Solver_add(solver, np, NO_IDX);

    while (true)
    {
        if (Heap_empty(& solver->queue)) return -1;
        if (_Solver_visit(solver)) return _Solver_backtrack(solver);
    }
}

#define _DC (1 << 10)
bool Solver_init(Solver * solver)
{
    return ((Vec_new_capacity(& solver->metric_vec, sizeof(int), _DC)) && 
            (Vec_new_capacity(& solver->np_vec, sizeof(Npuzzle), _DC)) &&
            (Vec_new_capacity(& solver->parent_vec, sizeof(int), _DC)) &&
            (Htbl_new_capacity(& solver->np_tbl, sizeof(Npuzzle), _DC)) &&
            (Heap_new_capacity(& solver->queue, sizeof(Elem), _DC)));
}

void Solver_deinit(Solver * solver)
{
    Vec_del(& solver->metric_vec);
    Vec_del(& solver->np_vec);
    Vec_del(& solver->parent_vec);
    Htbl_del(& solver->np_tbl);
    Heap_del(& solver->queue);
}



