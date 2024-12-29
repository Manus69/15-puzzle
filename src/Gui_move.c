#include "Gui.h"

static void _compute_selection(Grid * grid, int_int cell_rc, int ncycles)
{
    int         id;
    Rectangle   cell_rect;
    Rectangle   hole_rect;

    id = Grid_get_id_rc(grid, cell_rc.a, cell_rc.b);
    cell_rect = Grid_get_rect_rc(grid, cell_rc.a, cell_rc.b);
    hole_rect = Grid_get_rect_rc(grid, grid->hole_rc.a, grid->hole_rc.b);

    grid->selection = (Selection)
    {
        .cell_id = id,
        .ds = (Vector2)
        {
            .x = (hole_rect.x - cell_rect.x) / ncycles,
            .y = (hole_rect.y - cell_rect.y) / ncycles,
        },
        .ncycles = ncycles,
    };
}

static void _send(Grid * grid, int_int cell_rc)
{
    int id;

    id = Grid_get_id_rc(grid, cell_rc.a, cell_rc.b);

    Grid_set_id_rc(grid, cell_rc.a, cell_rc.b, HOLE);
    Grid_set_id_rc(grid, grid->hole_rc.a, grid->hole_rc.b, id);

    grid->cells[HOLE].rect = Grid_get_rect_rc(grid, cell_rc.a, cell_rc.b);
    grid->hole_rc = cell_rc;
}

bool Gui_move(Gui * gui, int drow, int dcol, int ncycles)
{
    int_int rc;

    if (Grid_in_motion(& gui->grid)) return false;
    
    rc = (int_int) {gui->grid.hole_rc.a + drow, gui->grid.hole_rc.b + dcol};
    if (! row_valid(rc.a) || ! col_valid(rc.b)) return false; 

    _compute_selection(& gui->grid, rc, ncycles);
    _send(& gui->grid, rc);

    // _swap_cell(gui, rc);

    return true;
}

void Gui_move_dir(Gui * gui, char dir, int ncycles)
{

}

void Gui_grid_move(Gui * gui, int idx)
{
    Cell * src;
    Cell * dst;

    src = Grid_get_idx(& gui->grid, idx);
    dst = Grid_get_idx(& gui->grid, HOLE);

    
}
