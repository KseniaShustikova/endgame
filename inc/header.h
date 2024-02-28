#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_W 1280
#define WINDOW_H 720

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define GRAVITY 0.35f;

typedef struct {
	SDL_Texture* texture;
	SDL_Rect rect;
} Button;

Mix_Music* backgroundMusic;
int soundOn;

typedef struct
{
	float x, y;
	float dx, dy;
	int onLedge;

	int animFrame, facingLeft, slowingDown;
} object;

typedef struct
{
	int x, y, w, h;
} Ledge;

typedef struct
{
	object alice;
	Button startButton, exitButton, volumeButton;
	Ledge ledges[100];

	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;

	SDL_Texture* backgroundTexture;
    Mix_Music* backgroundMusic;

	int time;

	SDL_Renderer* renderer;
} GameState;

int processEvents(SDL_Window* wind, GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void process(GameState* game);
void loadGame(GameState* game, const char *gameBackground, const char *gameMusic);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
void toggleSound();
void handleEvents(SDL_Renderer* renderer, Button* startButton, Button* exitButton, Button* volumeButton, int* quit, int *start);
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Button* startButton, Button* exitButton, Button* volumeButton);
void cleanup(SDL_Texture* startButtonTexture, SDL_Texture* exitButtonTexture, SDL_Texture* backgroundTexture, SDL_Texture* volumeOn, SDL_Texture* volumeOff, SDL_Renderer* renderer, SDL_Window* window);


#endif