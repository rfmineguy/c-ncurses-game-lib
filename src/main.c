#include "gfx.h"
#include <curses.h>
#include <math.h>

int main() {
    bitmap player = {0};
    gfx_init();
    int x = 100;
    int y = 50;
    int size = 20;
    int dx = 1, dy = 1;
    float angle = 0;
    
    while (!gfx_should_close()) {
        gfx_clear();
        gfx_square(x, y, size);
        x += dx;
        y += dy;
        if (x + size > gfx_width()) dx *= -1;
        if (x < 0) dx *= -1;
        if (y + size > gfx_height()) dy *= -1;
        if (y < 0) dy *= -1;
        size = fabs(sin(angle)) * 20;
        angle += 0.1f;
        gfx_render();
    }

    gfx_deinit();
}
