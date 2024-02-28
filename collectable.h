#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "funcCollect.c"
#include "./resource/SDL2/inc/SDL.h"
#include "./resource/SDL_TTF/include/SDL2/SDL_ttf.h"

//counter collectableCounter;

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

Collectable makeCollectable ( int x, int y, int width, int height);
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path);
void SetCounterText(counter  *counter, SDL_Renderer *renderer );
void renderCollectable (Collectable *element, SDL_Renderer *renderer, SDL_Rect *player );
void displayText (SDL_Renderer *renderer, counter *counter);