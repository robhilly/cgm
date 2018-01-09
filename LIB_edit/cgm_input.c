
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "cgm_input.h"

keyboard* _key = NULL;
mouse* _ms = NULL;

int input_init()
{
    int i;

    _key = (keyboard*)malloc(sizeof(keyboard));
    if(_key == NULL){
        printf("\nError allocating keyboard");
        return -1;
    }
    _ms = (mouse*)malloc(sizeof(mouse));
    if(_ms == NULL){
        printf("\nError allocating mouse");
        return -1;
    }

    _key->keys = NULL;
    _key->prev_keys = NULL;

    _key->key_state = SDL_GetKeyboardState(&_key->num_keys);

    _key->keys = (Uint8*)malloc(sizeof(Uint8) * _key->num_keys);
    _key->prev_keys = (Uint8*)malloc(sizeof(Uint8) * _key->num_keys);

    if(_key->keys == NULL || _key->prev_keys == NULL){
        return -1;
    }

    for(i = 0; i < _key->num_keys; i++){
        _key->keys[i] = _key->key_state[i];
        _key->prev_keys[i] = 0;
    }

    return 1;
}

void input_close()
{
    if(_key->prev_keys)
        free(_key->prev_keys);
    if(_key->keys)
        free(_key->keys);

    free(_ms);
    free(_key);
}

void input_update()
{
    int i;

    _key->key_state = SDL_GetKeyboardState(&_key->num_keys);

    for(i = 0; i < _key->num_keys; i++){
        _key->prev_keys[i] = _key->keys[i];
        _key->keys[i] = _key->key_state[i];
    }

    /*update mouse position*/
    SDL_GetMouseState(&_ms->x, &_ms->y);
}

int mouse_down()
{
    Uint32 state;

    state = SDL_GetMouseState(&_ms->x, &_ms->y);
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)){
        _ms->button = SDL_BUTTON_LEFT;
        return 1;
    }
    else if(state & SDL_BUTTON(SDL_BUTTON_MIDDLE)){
        _ms->button = SDL_BUTTON_MIDDLE;
        return 1;
    }
    else if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        _ms->button = SDL_BUTTON_RIGHT;
        return 1;
    }
    else{
        return 0; /*no mouse button pressed*/
    }
}

int mouse_getX()
{
    return _ms->x;
}

int mouse_getY()
{
    return _ms->y;
}

int mouse_getBtn()
{
    return _ms->button;
}



int key_down(int key)
{
    return _key->keys[key];
}

int key_up(int key)
{
    return (_key->prev_keys[key] && !_key->keys[key]);
}
