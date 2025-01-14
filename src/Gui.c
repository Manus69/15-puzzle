#include "Gui.h"

#include <assert.h>

#define SOUND_PATH  "ass/slap.wav"
#define TX_PATH     "ass/tx.png"
#define ICON_PATH   "ass/icon.png"
#define TX_CELL_SIZE 100
#define WW 800
#define WH 800
#define FRAME_THICKNESS 70
#define TFPS 60
#define PAD 5
#define SND_CYCLE_OFS 3
#define SND_VOL 0.05
#define SND_CYCLES 20

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

static int _Grid_row_click(Grid const * grid, Vector2 xy)
{
    return (xy.y - grid->rect.y) / _Grid_cell_size(grid);
}

static int _Grid_col_click(Grid const * grid, Vector2 xy)
{
    return (xy.x - grid->rect.x) / _Grid_cell_size(grid);
}

static int _Window_h(void)
{
    return WH;
}

static int _Window_w(void)
{
    return WW;
}

static void _Tx_init_tiles(Gui * gui)
{
    int idx;

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
}

static void _Tx_init_frame(Gui * gui)
{
    gui->tx.frame = (Rectangle)
    {
        .x = 400,
        .y = 0,
        .height = 500,
        .width = 490,
    };
}

static void _Tx_init_bg(Gui * gui)
{
    gui->tx.bg = (Rectangle)
    {
        .x = 910,
        .y = 0,
        .width = 410,
        .height = 410,
    };
}

static bool _Tx_init(Gui * gui)
{
    gui->tx.texture = LoadTexture(TX_PATH);
    if (gui->tx.texture.id == 0) return false;

    _Tx_init_tiles(gui);    
    _Tx_init_frame(gui);
    _Tx_init_bg(gui);

    return true;
}

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

static bool _Gui_init_icon(Gui * gui)
{
    gui->icon = LoadImage(ICON_PATH);

    return gui->icon.data;
}

static bool _Snd_init(Snd * snd)
{
    InitAudioDevice();
    if (! IsAudioDeviceReady()) return false;

    snd->sound = LoadSound(SOUND_PATH);
    SetSoundVolume(snd->sound, SND_VOL);
    
    return IsSoundReady(snd->sound);
}

bool Gui_init(Gui * gui)
{
    * gui = (Gui) {};

    gui->rect.width = _Window_w();
    gui->rect.height = _Window_h();

    InitWindow(gui->rect.width, gui->rect.height, 0);
    SetTargetFPS(TFPS);

    if (! _Gui_init_icon(gui)) return false;
    SetWindowIcon(gui->icon);

    if (! _Tx_init(gui)) return false;
    _Gui_elem_init(gui);

    if (! _Snd_init(& gui->snd)) return false;
    
    return true;
}

void Gui_deinit(Gui * gui)
{
    UnloadTexture(gui->tx.texture);
    UnloadImage(gui->icon);
    UnloadSound(gui->snd.sound);
    CloseAudioDevice();
    CloseWindow();
}

void Gui_mute_toggle(Gui * gui)
{
    SetSoundVolume(gui->snd.sound, gui->snd.muted ? SND_VOL : 0);
    
    gui->snd.muted = ! gui->snd.muted;
    gui->snd.cycles = TFPS;
}

void Gui_update(Gui * gui)
{
    if (Gui_grid_in_animation(gui))
    {
        gui->grid.selection.rect->x += gui->grid.selection.ds.x;
        gui->grid.selection.rect->y += gui->grid.selection.ds.y;
        gui->grid.selection.ncycles --;

        if (gui->grid.selection.ncycles == 1)
        {
            gui->snd.should_play = true;
        }
    }
}

void _draw_msg(Gui * gui)
{
    static const char * _msg[] =
    {
        "Sound ON",
        "Sound OFF",
    };

    if (gui->snd.cycles)
    {
        DrawText(_msg[gui->snd.muted], PAD, PAD, 30, BLACK);
        gui->snd.cycles --;
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
    ClearBackground(BLACK);

    DrawTexturePro(gui->tx.texture, gui->tx.bg, gui->bg, (Vector2){}, 0, GRAY);
    _Grid_draw(gui);
    DrawTexturePro(gui->tx.texture, gui->tx.frame, gui->frame, (Vector2){}, 0, RAYWHITE);
    _draw_msg(gui);

    EndDrawing();
}

void Gui_play_sound(Gui * gui)
{
    if (gui->snd.should_play)
    {
        PlaySound(gui->snd.sound);
        gui->snd.should_play = false;
    }
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

static Vector2 _Rect_center(Rectangle rect)
{
    return (Vector2)
    {
        .x = rect.x + rect.width / 2,
        .y = rect.y + rect.height / 2,
    };
}

static Vector2 _Rect_corner(Rectangle rect, int dx, int dy)
{
    Vector2 center;

    center = _Rect_center(rect);

    return (Vector2)
    {
        .x = center.x + dx * (rect.width / 2),
        .y = center.y + dy * (rect.height / 2),
    };
}
        
typedef struct
{
    Vector2 vertex[3];
}   Tri;

static Tri _Tri_rect(Rectangle rect, char dir)
{
    Tri tri;

    static const char _dx[][2] =
    {
        ['r'] = {1, 1},
        ['l'] = {-1, -1},
        ['u'] = {-1, 1},
        ['d'] = {-1, 1},
        ['z'] = {},
    };

    static const char _dy[][2] =
    {
        ['u'] = {-1, -1},
        ['d'] = {1, 1},
        ['l'] = {-1, 1},
        ['r'] = {-1, 1},
        ['z'] = {},
    };

    tri = (Tri)
    {
        .vertex[0] = _Rect_center(rect),
        .vertex[1] = _Rect_corner(rect, _dx[(int) dir][0], _dy[(int) dir][0]),
        .vertex[2] = _Rect_corner(rect, _dx[(int) dir][1], _dy[(int) dir][1]),
    };

    return tri;
}

static char _click_pdir(Rectangle rect, Vector2 xy)
{
    Tri tri;

    for (int k = 0; k < NP_NDIRS; k ++)
    {
        tri = _Tri_rect(rect, NP_DIRS[k]);
        if (CheckCollisionPointTriangle(xy, tri.vertex[0], tri.vertex[1], tri.vertex[2])) return NP_DIRS[k];
    }

    return NO_IDX;
}

static int _click_idx(Gui const * gui, Vector2 xy)
{
    int row, col;

    if (! CheckCollisionPointRec(xy, gui->grid.rect)) return NO_IDX;

    row = _Grid_row_click(& gui->grid, xy);
    col = _Grid_col_click(& gui->grid, xy);

    return row_col_idx(row, col);
}

GuiClck Gui_click(Gui const * gui, Vector2 xy)
{
    int     idx;
    char    dir;

    idx = _click_idx(gui, xy);
    if (idx == NO_IDX) return (GuiClck) {.idx = idx};

    dir = _click_pdir(_Grid_layout_rect(& gui->grid, idx), xy);

    if (dir == NO_IDX) return (GuiClck) {.idx = idx};
    
    return (GuiClck)
    {
        .idx = idx,
        .pdir = dir,
    };
}
