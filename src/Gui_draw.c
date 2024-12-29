#include "Gui.h"

static char const * _get_text(int x)
{
    static const char * _txt[] =
    {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16",
    };

    return _txt[x];
}

static void _grid_txt(const Grid * grid)
{
    Cell * cell;

    for (int k = 1; k < GRID_SIZE * GRID_SIZE; k ++)
    {
        cell = Grid_get_idx(grid, k);
        DrawRectangleRec(cell->rect, ORANGE);
        DrawRectangleLinesEx(cell->rect, 1, BLACK);
        // DrawText(_get_text(cell->val), cell->rect.x + 1, cell->rect.y + 1, 30, RAYWHITE);
    }
}

static void _grid_texture(Gui const * gui)
{
    Cell * cell;

    for (int id = 1; id < GRID_SIZE * GRID_SIZE; id ++)
    {
        cell = Grid_get_idx(& gui->grid, id);
        DrawTexturePro(gui->tx.texture, TxLayout_get(& gui->tx.grid_layout, id), cell->rect, (Vector2){}, 0, RAYWHITE);
    }
}

void Gui_draw(const Gui * gui)
{
    (void) _grid_txt;

    BeginDrawing();
    ClearBackground(GRAY);

    // _grid_txt(& gui->grid);
    _grid_texture(gui);

    EndDrawing();
}