#include "Gui.h"

void TxLayout_init(TxLayout * layout, int xoffset, int yoffset, int size)
{
    int id;

    id = 1;
    for (int row = 0; row < GRID_SIZE; row ++)
    {
        for (int col = 0; col < GRID_SIZE; col ++)
        {
            if (id == GRID_SIZE * GRID_SIZE) return;
            
            layout->src[id] = (Rectangle)
            {
                .x = xoffset + col * size,
                .y = yoffset + row * size,
                .width = size,
                .height = size,
            };

            id ++;
        }
    }
}

