#ifndef CGM_ENGINE_H
#define CGM_ENGINE_H

#include "SDL.h"

/*C GaMe Main engine module*/

typedef struct engineObj{
    SDL_Window* window;
    SDL_Surface* screen;

    /*timing*/
    int fps;
    Uint32 tick_count;
    Uint32 cur_tick;
    Uint32 next_tick;

    /*resolution and fullscreen toggle*/
    int scr_w, scr_h;
    int fullscreen;
}engine;

int eng_init(int w, int h, int full);

void eng_close();

/*draw contents of screen*/
void eng_flip();

/*timing*/
void eng_setFPS(int fps);
int eng_wait();

/*screen surface access*/
SDL_Surface* eng_getScreen();

SDL_Surface* eng_createSurface(int w, int h);

void eng_drawPoint(int x, int y, Uint32 color);

void eng_drawLine(int x0, int y0, int x1, int y1, Uint32 color);

/*global*/
extern engine _eng;

#endif // CGM_ENGINE_H
