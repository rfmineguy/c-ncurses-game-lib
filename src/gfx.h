#ifndef GFX_H
#define GFX_H
#include <stdbool.h>
#include <stdint.h>

#define BRAILE_START 0x2800

typedef struct bitmap {
    uint32_t *colors;
    uint16_t width, height;
} bitmap;

void gfx_init();
void gfx_deinit();
void gfx_set_framerate(int fps);

void gfx_clear();
void gfx_render();

void gfx_pixel(int x, int y);
void gfx_cell(int termx, int termy);
void gfx_circle(int x, int y, int r);
void gfx_rect(int x, int y, int w, int h);
void gfx_square(int x, int y, int d);
void gfx_bitmap(int x, int y, bitmap b);

int gfx_width();
int gfx_height();
int gfx_mouse_x();
int gfx_mouse_y();

void gfx_debug_state();

bool gfx_should_close();
bool gfx_mouse_down(int);

#endif
