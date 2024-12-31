#ifndef GUI_H
#define GUI_H

#include "master.h"

typedef struct
{
    Rectangle * rect;
    Vector2     ds;
    int         ncycles;
}   Selection;

typedef struct
{
    Rectangle   rect;
    Rectangle   cells[NP_GRID_SIZE * NP_GRID_SIZE];
    Selection   selection;
}   Grid;

typedef struct
{
    Texture2D   texture;
    Rectangle   src[NP_GRID_SIZE * NP_GRID_SIZE];
}   Tx;

typedef struct
{
    Rectangle   rect;
    Grid        grid;
    Tx          tx;
}   Gui;

bool Gui_init(Gui * gui);
void Gui_deinit(Gui * gui);
void Gui_update(Gui * gui);
void Gui_draw(Gui * gui);
bool Gui_grid_in_animation(Gui const * gui);
void Gui_grid_set(Gui * gui, char const * cstr);
void Gui_grid_move(Gui * gui, int idx, int ncycles);


#endif