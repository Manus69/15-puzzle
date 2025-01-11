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
    int idx;

    gui->tx.texture = LoadTexture(TX_PATH);
    if (gui->tx.texture.id == 0) return false;

    for (int k = 1; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        int row, col;

        idx = k - 1;
        row = idx_row(idx);
        col = idx_col(idx);

        gui->tx.src[k] = (Rectangle)
        {
            .x = col * TX_CELL_SIZE,
            .y = row * TX_CELL_SIZE,
            .width = TX_CELL_SIZE,
            .height = TX_CELL_SIZE,
        };
    }

    gui->tx.frame = (Rectangle)
    {
        .x = 400,
        .y = 0,
        .height = 500,
        .width = 490,
    };

    gui->tx.bg = (Rectangle)
    {
        .x = 910,
        .y = 0,
        .width = 410,
        .height = 410,
    };

    return true;
}

#define PAD 5
static void _Gui_elem_init(Gui * gui)
{
    gui->frame = gui->rect;
    gui->grid.rect = (Rectangle)
    {
        .x = gui->frame.x + FRAME_THICKNESS + 2 * PAD,
        .y = gui->frame.y + FRAME_THICKNESS,
        .width = gui->frame.width - 2 * FRAME_THICKNESS,
        .height = gui->frame.height - 2 * FRAME_THICKNESS - 2 * PAD,
    };

    gui->bg = gui->rect;
}

bool Gui_init(Gui * gui)
{
    * gui = (Gui) {};

    gui->rect.width = _Window_w();
    gui->rect.height = _Window_h();

    InitWindow(gui->rect.width, gui->rect.height, 0);
    SetTargetFPS(TFPS);

    if (! _Tx_init(gui)) return false;
    _Gui_elem_init(gui);
    
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
    DrawTexturePro(gui->tx.texture, gui->tx.bg, gui->bg, (Vector2){}, 0, GRAY);

    for (int k = 0; k < NP_GRID_SIZE * NP_GRID_SIZE; k ++)
    {
        DrawTexturePro(gui->tx.texture, gui->tx.src[k], gui->grid.cells[k], (Vector2){}, 0, RAYWHITE);
    }

    DrawTexturePro(gui->tx.texture, gui->tx.frame, gui->frame, (Vector2){}, 0, RAYWHITE);
}

void Gui_draw(Gui * gui)
{
    BeginDrawing();
    ClearBackground(BLACK);

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


