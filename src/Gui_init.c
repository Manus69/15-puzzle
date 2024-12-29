#include "Gui.h"

static void _init_rect(Grid * grid, Rectangle rect)
{
    grid->rect = rect;
}

void Grid_init_grid_cstr(Grid * grid, const char * cstr)
{
    int id;

    for (int row = 0; row < GRID_SIZE; row ++)
    {
        for (int col = 0; col < GRID_SIZE; col ++)
        {
            id = parse_symbol(* cstr ++);

            grid->cells[id] = (Cell)
            {
                .rect = Grid_get_rect_rc(grid, row, col),
                // .val = id,
            };

            grid->grid[row][col] = id;

            if (id == HOLE) grid->hole_rc = (int_int) {row, col};
        }
    }
}

static const char * _cstr[] = 
{
    [2] = "1230",
    [3] = "123456780",
    [4] = "123456789abcdef0",
};

#define TX_PATH "ass/pp.png"
#define XOFFSET 100
#define YOFFSET 100
#define SIZE    150

bool Gui_init(Gui * gui, int w, int h)
{
    assert(GRID_SIZE > 1 && GRID_SIZE <= 4);

    * gui = (Gui) {};

    gui->tx.texture = LoadTexture(TX_PATH);
    if (gui->tx.texture.id <= 0) return false;

    TxLayout_init(& gui->tx.grid_layout, XOFFSET, YOFFSET, SIZE);
    
    _init_rect(& gui->grid, (Rectangle) {0, 0, w, h});
    Grid_init_grid_cstr(& gui->grid, _cstr[GRID_SIZE]);

    return true;
}

void Gui_deinit(Gui * gui)
{
    UnloadTexture(gui->tx.texture);
}