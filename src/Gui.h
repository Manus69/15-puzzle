#ifndef GUI_H
#define GUI_H

#include "master.h"

#define HOLE 0

typedef struct
{
    Rectangle   rect;
    // int         val;
}   Cell;

typedef struct
{
    int         cell_id;
    int         ncycles;
    Vector2     ds;
}   Selection;

typedef struct
{
    Rectangle   rect;
    Cell        cells[GRID_SIZE * GRID_SIZE];
    int         grid[GRID_SIZE][GRID_SIZE];
    int_int     hole_rc;
    Selection   selection;
}   Grid;

static inline float Grid_cellw(Grid const * grid)
{
    return grid->rect.width / GRID_SIZE;
}

static inline float Grid_cellh(Grid const * grid)
{
    return grid->rect.height / GRID_SIZE;
}

static inline Rectangle Grid_get_rect_rc(Grid * grid, int row, int col)
{
    return (Rectangle)
    {
        .x = grid->rect.x + col * Grid_cellw(grid),
        .y = grid->rect.y + row * Grid_cellh(grid),
        .width = Grid_cellw(grid),
        .height = Grid_cellh(grid),
    };
}

static inline Cell * Grid_get_idx(Grid const * grid, int idx)
{
    return (Cell *) & grid->cells[idx];
}

static inline int Grid_get_id_rc(Grid const * grid, int row, int col)
{
    return grid->grid[row][col];
}

// rem
static inline Cell * Grid_get_cell_rc(Grid const * grid, int row, int col)
{
    return (Cell *) & grid->cells[grid->grid[row][col]];
}

//
static inline Cell * Grid_hole(Grid const * grid)
{
    return Grid_get_idx(grid, HOLE);
}

static inline void Grid_set_id_rc(Grid * grid, int row, int col, int val)
{
    grid->grid[row][col] = val;
}



static inline bool Grid_in_motion(Grid const * grid)
{
    return grid->selection.ncycles;
}

typedef struct
{
    Rectangle src[GRID_SIZE * GRID_SIZE];
}   TxLayout;

static inline Rectangle TxLayout_get(TxLayout const * layout, int id)
{
    return layout->src[id];
}

typedef struct
{
    Texture2D   texture;
    TxLayout    grid_layout;
}   Tx;

typedef struct
{
    Grid        grid;
    Tx          tx;
}   Gui;

void TxLayout_init(TxLayout * layout, int xoffset, int yoffset, int size);
bool Gui_init(Gui * gui, int w, int h);
void Gui_deinit(Gui * gui);
void Gui_update(Gui * gui);
void Gui_draw(Gui const * gui);
bool Gui_move(Gui * gui, int drow, int dcol, int ncycles);

#endif