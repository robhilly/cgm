#ifndef CGM_SPRITE_H
#define CGM_SPRITE_H

#include "SDL.h"
#include "SDL_image.h"
#include "cgm_engine.h"


/*sprite states*/
#define SPR_STATE_DEAD      0
#define SPR_STATE_ALIVE     1
#define SPR_STATE_DYING     2
#define SPR_STATE_ANIM_DONE 1

#define SPR_MAX_FRAMES      64
#define SPR_MAX_ANIMATIONS  32

#define SPR_MAX_VARS        64

/*attributes for animation*/
#define SPR_ATTR_SINGLE         1
#define SPR_ATTR_MULTI_FRAME    2
#define SPR_ATTR_MULTI_ANIM     4
#define SPR_ATTR_ANIM_ONE_SHOT  8   /*animation will only perform once*/

typedef struct spriteObj{
    int state;
    int attr;
    int w, h;
    int x, y;
    int xv, yv;

    int remove;


    /*sprite vars*/
    int iVars[SPR_MAX_VARS];
    int fVars[SPR_MAX_VARS];

    int num_frames;
    int cur_frame;
    int cur_anim;
    int anim_index;

    /*TIMING*/
    int anim_count;
    int anim_count_max;

    int* animations[SPR_MAX_ANIMATIONS];


    SDL_Surface* frames[SPR_MAX_FRAMES];


}sprite;

void spr_init(sprite* spr, int w, int h, int attr)
{
    int i;

    spr->state = SPR_STATE_DEAD;

    spr->x = 0;
    spr->y = 0;
    spr->w = w;
    spr->h = h;
    spr->xv = 0;
    spr->yv = 0;
    spr->attr = attr;

    for(i = 0; i < SPR_MAX_ANIMATIONS; i++){
        spr->animations[i] = NULL;
    }

    for(i = 0; i < SPR_MAX_FRAMES; i++){
        spr->frames[i] = NULL;
    }

    spr->cur_frame = 0;
    spr->cur_anim = 0;
    spr->anim_index = 0;

    spr->anim_count = 0;
    spr->anim_count_max = 5;

    spr->remove = 0;
}

void spr_close(sprite* spr)
{
    int i;

    for(i = 0; i < SPR_MAX_ANIMATIONS; i++){
        if(spr->animations[i]){
            free(spr->animations[i]);
            spr->animations[i] = NULL;
        }
    }

    for(i = 0; i < SPR_MAX_FRAMES; i++){
        if(spr->frames[i]){
            SDL_FreeSurface(spr->frames[i]);
            spr->frames[i] = NULL;
        }
    }
}

void spr_setAnimSpeed(sprite* spr, int as)
{
    spr->anim_count_max = as;
}

void spr_setAnimIndex(sprite* spr, int ai)
{
    spr->cur_anim = ai;
}

int spr_loadFrames(sprite* spr, char* img, int num_frames, int bor)
{
    int i;
    SDL_Surface* surf;
    SDL_Rect copy;

    spr->num_frames = num_frames;

    copy.x = bor;
    copy.y = bor;
    copy.w = spr->w;
    copy.h = spr->h;

    surf = IMG_Load(img);
    if(surf == NULL){
        printf("\nError loading frame: %s", IMG_GetError());
        return -1;
    }

    for(i = 0; i < spr->num_frames; i++){
        spr->frames[i] = eng_createSurface(spr->w, spr->h);
        if(spr->frames[i] == NULL){
            printf("\nError loading frame: %i, %s", i, SDL_GetError());
            SDL_FreeSurface(surf);
            return -1;
        }
        SDL_BlitSurface(surf, &copy, spr->frames[i], NULL);
        copy.x += (spr->w + bor);
        if(copy.x >= surf->w){
            copy.x = bor;
            copy.y += (spr->h + bor);
        }
    }

    SDL_FreeSurface(surf);

    return 1;

}

int spr_loadAnim(sprite* spr, int a_i, int length, int* anim_data)
{
    int i;

    if(spr->animations[a_i]){
        printf("\nAnimation already exists in index: %d", a_i);
        return -1;
    }

	/*shouldn't assume clean alloc*******/
    spr->animations[a_i] = (int*)malloc(sizeof(int) * (length + 1));
    for(i = 0; i < length; i++){
        spr->animations[a_i][i] = anim_data[i];
    }

    spr->animations[a_i][i] = -1;

    return 1;
}

void spr_update(sprite* spr)
{

    if(spr->attr & SPR_ATTR_SINGLE){
        spr->cur_frame = 0;
    }
    else if(spr->attr & SPR_ATTR_MULTI_FRAME){
        if(++spr->anim_count > spr->anim_count_max){
            spr->cur_frame += 1;
            if(spr->cur_frame >= spr->num_frames){
                spr->cur_frame = 0;
            }
            spr->anim_count = 0;
        }
    }
    else if(spr->attr & SPR_ATTR_MULTI_ANIM){
        if(++spr->anim_count > spr->anim_count_max){
            spr->anim_index += 1;
            if(spr->animations[spr->cur_anim][spr->anim_index] == -1){
                spr->anim_index = 0;
            }
            spr->cur_frame = spr->animations[spr->cur_anim][spr->anim_index];
            spr->anim_count = 0;
        }
    }

    /*update sprite position*/
    spr->x += spr->xv;
    spr->y += spr->yv;
}

void spr_show(sprite* spr)
{
    SDL_Rect draw;

    draw.x = spr->x;
    draw.y = spr->y;
    draw.w = spr->w;
    draw.h = spr->h;

    SDL_BlitSurface(spr->frames[spr->cur_frame], NULL, eng_getScreen(), &draw);
}

#endif // CGM_SPRITE_H
