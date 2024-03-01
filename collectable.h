#pragma once
#include "./header.h"
#include <stdbool.h>


//



Collectable makeCollectable ( int x, int y, int width, int height);
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path);
void SetCounterText(counter  *counter, SDL_Renderer *renderer );
void renderCollectable (Collectable *element, SDL_Renderer *renderer, GameState *game,  int type);
void displayText (SDL_Renderer *renderer, counter *counter);
void DisplayLetterText(SDL_Renderer *renderer );
void DisplayEndPicture(SDL_Renderer *renderer, char *path);