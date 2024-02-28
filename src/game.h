#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    SDL_Texture *texture;
    SDL_Rect rect;
} Button;

Mix_Music *backgroundMusic;
int soundOn;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filename);
void toggleSound();
void handleEvents(SDL_Renderer *renderer, Button *startButton, Button *exitButton, Button *volumeButton, int *quit);
void render(SDL_Renderer *renderer, SDL_Texture *backgroundTexture, Button *startButton, Button *exitButton, Button *volumeButton);
void cleanup(SDL_Texture *startButtonTexture, SDL_Texture *exitButtonTexture, SDL_Texture *backgroundTexture, SDL_Texture *volumeOn, SDL_Texture *volumeOff, SDL_Renderer *renderer, SDL_Window *window);

#endif
