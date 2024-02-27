#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./resource/SDL2/inc/SDL.h"
#include "./resource/SDL_TTF/include/SDL2/SDL_ttf.h"

typedef struct counter{
    int counter;
    SDL_Texture *texture;
    SDL_Rect rect;
    
} counter;

typedef struct collectable {
    SDL_Rect rect;
    SDL_Texture *image;
    bool isCollected;
}Collectable;