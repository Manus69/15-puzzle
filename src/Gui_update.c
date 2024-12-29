#include "Gui.h"

static void _grid(Grid * grid)
{
    Cell * cell;

    if (grid->selection.ncycles)
    {
        cell = Grid_get_idx(grid, grid->selection.cell_id);
        cell->rect.x += grid->selection.ds.x;
        cell->rect.y += grid->selection.ds.y;

        grid->selection.ncycles --;
    }
}

void Gui_update(Gui * gui)
{
    _grid(& gui->grid);
}