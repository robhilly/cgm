#include "cgm_engine.h"
#include "cgm_input.h"
#include "cgm_world.h"

#include <stdio.h>
#include <stdlib.h>

void fill_screen(Uint32 color)
{
    SDL_FillRect(_eng->screen, NULL, color);
}

int main(int argc, char* argv[])
{
    SDL_Event evt;
    sprite sp[4];
    int loop = 1;
    int i;
    int cx, cy;
    int px, py;

    if(eng_init(1024, 768, 0) < 0){
        printf("\nError initializing engine");
        return 1;
    }

    eng_setFPS(60);

    input_init();
    wld_init(32, 32, 100, 100);

    cx = 0;
    cy = 0;
    px = 0;
    py = 0;

    for(i = 0; i < 4; i++){
        spr_init(&sp[i], 34, 22, SPR_ATTR_MULTI_FRAME );
        spr_loadFramesPos(&sp[i], "sheets/spr_1.png", 2, 0, cx, cy);
        cy += 1;

        sp[i].x = px;
        sp[i].y = py;
        px += sp[i].w;

        wld_addSprite(&sp[i]);
    }

    while(loop){
        while(SDL_PollEvent(&evt)){
            switch(evt.type){
            case SDL_QUIT:
                printf("\nExiting loop");
                loop = 0;
                break;
            default: break;
            }
        }
        input_update();

        if(key_down(SDL_SCANCODE_ESCAPE)){
            loop = 0;
        }

        if(key_down(SDL_SCANCODE_1)){
            spl_setRemove(_wld->spl, 0);
        }

        if(key_down(SDL_SCANCODE_2)){
            spl_setRemove(_wld->spl, 1);
        }

        if(key_down(SDL_SCANCODE_3)){
            spl_setRemove(_wld->spl, 2);
        }

        if(key_down(SDL_SCANCODE_4)){
            spl_setRemove(_wld->spl, 3);
        }

        eng_fillScreen(0x00ffffff);

        for(i = 100; i < 200; i++){
            eng_drawLine(0, i, 1023, i, 0x0000ff00);
        }

        wld_update();
        wld_render();

        eng_wait();
        eng_flip();
    }

    wld_close();
    input_close();
    eng_close();



    return 0;
}
