#include "gfx.h"
#include <locale.h>
#include <unistd.h>
#define _XOPEN_SOURCE_EXTENDED
#include <curses.h>
#include <stdlib.h>
#include <string.h>

typedef struct region {
    int x, y, w, h;
} region;

typedef struct mouse {
    int x, y;
    bool buttons[3];
} mouse_;

char* old_locale = NULL;
int term_width, term_height;
int mouseX, mouseY;
bool keyboard[255] = {0};
bool running = true;
uint8_t *screen;
int ticks = 0;
FILE* logfile;
mouse_ mouse = {0};

region update_stack[1000] = {0};
int    update_stack_top = -1;

void gfx_init(){
    old_locale = setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    getmaxyx(stdscr, term_height, term_width);
    screen = calloc(term_width * term_height, sizeof(uint8_t));
    memset(screen, 0x00, term_width * term_height);
    logfile = fopen("log", "w");
}
void gfx_deinit(){
    free(screen);
    endwin();
    setlocale(LC_ALL, old_locale);
    fclose(logfile);
}
void gfx_set_framerate(int fps){
}
void gfx_render(){
    while (update_stack_top != -1) {
        region r = update_stack[update_stack_top];
        for (int x = r.x; x < r.x + r.w; x++) {
            for (int y = r.y; y < r.y + r.h; y++) {
                mvprintw(y, x, "%lc", 0x2800 | screen[y * term_width + x]);
            }
        }
        update_stack_top--;
    }

    refresh();
}
void gfx_clear() {
    memset(screen, 0x00, term_width * term_height);
    clear();
}
void gfx_pixel(int x, int y) {
    int termx = x / 2;
    int termy = y / 4;
    if (termx < 0 || termx >= term_width) return;
    if (termy < 0 || termy >= term_height) return;
    int gx = x % 2;
    int gy = y % 4;

    uint8_t character = screen[termy * term_width + termx];
    mvprintw(20, 10, "%d, %d", gx, gy);
    if (gx == 0 && gy == 0) character |= 0b00000001;
    if (gx == 1 && gy == 0) character |= 0b00001000;
    if (gx == 0 && gy == 1) character |= 0b00000010;
    if (gx == 1 && gy == 1) character |= 0b00010000;
    if (gx == 0 && gy == 2) character |= 0b00000100;
    if (gx == 1 && gy == 2) character |= 0b00100000;
    if (gx == 0 && gy == 3) character |= 0b01000000;
    if (gx == 1 && gy == 3) character |= 0b10000000;
    int index = termy * term_width + termx;
    screen[index] = character;
    update_stack[++update_stack_top] = (region) {.x = termx, .y = termy, 1, 1};
}
void gfx_cell(int termx, int termy) {
    screen[termy * term_width + termx] = 0xff;
    update_stack[++update_stack_top] = (region) {.x = termx, .y = termy, 1, 1};
}
void gfx_circle(int x, int y, int r) {
}
void gfx_rect(int x, int y, int w, int h) {
}
void gfx_square(int x, int y, int d) {
    for (int i = x; i < x + d; i++) {
        for (int j = y; j < y + d; j++) {
            gfx_pixel(i, j);
        }
    } 
}
void gfx_bitmap(int x, int y, bitmap b) {
}
int gfx_width(){
    return term_width * 2;
}
int gfx_height(){
    return term_height * 4;
}
int gfx_mouse_x(){
    return mouse.x;
}
int gfx_mouse_y(){
    return mouse.y;
}
void gfx_debug_state() {
}
bool gfx_should_close(){
    MEVENT event;
    if (nodelay(stdscr, TRUE) != ERR) {
        int c = getch();
        switch (c) {
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_PRESSED) mouse.buttons[0] = 1;
                    if (event.bstate & BUTTON1_RELEASED) mouse.buttons[0] = 0;
                    mouse.x = event.x;
                    mouse.y = event.y;
                }
        }
        running = c == 'q';
    }
    ticks++;
    usleep(1000000/60.f);       //sleep 1/60 second
    return running;
}
bool gfx_mouse_down(int button) {
    return mouse.buttons[button];
}
