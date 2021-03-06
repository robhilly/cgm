#include <stdio.h>
#include <stdlib.h>

#include "SDL_image.h"

#include "cgm_engine.h"
#include "cgm_tilemap.h"

int tm_init(tileMap* tm, int tw, int th, int mw, int mh)
{
    int i;

    tm->t_w = tw;
    tm->t_h = th;
    tm->m_w = mw;
    tm->m_h = mh;

    tm->tiles = NULL;
    tm->tiles = (tile*)malloc(sizeof(tile) * (mw * mh));
    if(tm->tiles == NULL){
        printf("\nError allocating tiles");
        return -1;
    }

    /*reset tiles*/
    for(i = 0; i < (mw*mh); i++){
        tm->tiles[i].index = 0;
        tm->tiles[i].type = TILE_TYPE_NONE;
    }

    return 1;
}

tile tm_getTile(tileMap* tm, int tx, int ty)
{
    return tm->tiles[(ty * tm->m_w) + tx];
}

void tm_close(tileMap* tm)
{
    if(tm->tiles != NULL){
        free(tm->tiles);
    }
}
