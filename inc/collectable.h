#pragma once
#include "header.h"

#define type_collectable 1
#define type_letter 2

//
typedef struct counter{
    int counter;
    SDL_Texture *texture;
    SDL_Rect rect;
    
} counter;

typedef struct message{
    char *text;
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isOn;
} message;

typedef struct collectable {
    SDL_Rect rect;
    SDL_Texture *image;
    bool isCollected;
}Collectable;

counter collectableCounter;
message msg;

Collectable makeCollectable ( int x, int y, int width, int height);
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path);
void SetCounterText(counter  *counter, SDL_Renderer *renderer );
void renderCollectable (Collectable *element, SDL_Renderer *renderer, GameState* game, int type);
void displayText (SDL_Renderer *renderer, counter *counter);
void DisplayLetterText(SDL_Renderer *renderer );
void DisplayEndPicture(SDL_Renderer *renderer, char *path);
