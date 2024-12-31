#include "Gui.h"
#include "init.h"

static float _Grid_cell_size(Grid const * grid)
{
    return grid->rect.width / NP_GRID_SIZE;
}

static Rectangle _Grid_layout_rect_rc(Grid const * grid, int row, int col)
{
    float size;

    size = _Grid_cell_size(grid);

    return (Rectangle)
    {
        .x = grid->rect.x + size * col,
        .y = grid->rect.y + size * row,
        .width = size,
        .height = size,
    };
}

static Rectangle _Grid_layout_rect(Grid const * grid, int idx)
{
    return _Grid_layout_rect_rc(grid, idx_row(idx), idx_col(idx));
}

static Rectangle * _Grid_get(Grid const * grid, int idx)
{
    return (Rectangle *) & grid->cells[idx];
}

static int _Window_h(void)
{
    return WH;
}

static int _Window_w(void)
{
    return WW;
}

static bool _Tx_init(Gui * gui)
{
    gui->tx.texture = LoadTexture(TX_PATH);
    if (gui->tx.texture.id == 0) return false;

    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        gui->tx.src[k] = (Rectangle)
        {
            .x = k * TX_CELL_SIZE,
            .y = 0,
            .width = TX_CELL_SIZE,
            .height = TX_CELL_SIZE,
        };
    }

    return true;
}

static void _Grid_init(Gui * gui)
{
    gui->grid.rect = gui->rect;
}

bool Gui_init(Gui * gui)
{
    * gui = (Gui) {};

    gui->rect.width = _Window_w();
    gui->rect.height = _Window_h();

    InitWindow(gui->rect.width, gui->rect.height, 0);
    SetTargetFPS(TFPS);

    if (! _Tx_init(gui)) return false;
    _Grid_init(gui);
    
    return true;
}

void Gui_deinit(Gui * gui)
{
    UnloadTexture(gui->tx.texture);
    CloseWindow();
}

void Gui_update(Gui * gui)
{
    if (Gui_grid_in_animation(gui))
    {
        gui->grid.selection.rect->x += gui->grid.selection.ds.x;
        gui->grid.selection.rect->y += gui->grid.selection.ds.y;
        gui->grid.selection.ncycles --;
    }
}

void _Grid_draw(Gui * gui)
{
    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        DrawTexturePro(gui->tx.texture, gui->tx.src[k], gui->grid.cells[k], (Vector2){}, 0, RAYWHITE);
    }
}

void Gui_draw(Gui * gui)
{
    BeginDrawing();
    ClearBackground(GRAY);

    _Grid_draw(gui);

    EndDrawing();
}

bool Gui_grid_in_animation(Gui const * gui)
{
    return gui->grid.selection.ncycles;   
}

void Gui_grid_set(Gui * gui, char const * cstr)
{
    int idx;

    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        idx = sym_val(cstr[k]);
        gui->grid.cells[idx] = _Grid_layout_rect(& gui->grid, k);
    }
}

void Gui_grid_move(Gui * gui, int idx, int ncycles)
{
    Rectangle * src;
    Rectangle * dst;

    src = _Grid_get(& gui->grid, idx);
    dst = _Grid_get(& gui->grid, 0);

    gui->grid.selection = (Selection)
    {
        .ds = (Vector2)
        {
            .x = (dst->x - src->x) / ncycles,
            .y = (dst->y - src->y) / ncycles,  
        },
        .rect = src,
        .ncycles = ncycles,
    };

    * dst = * src;
}


