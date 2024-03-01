#pragma once

#ifndef HEADER_H
#define HEADER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_W 1280
#define WINDOW_H 720

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define GRAVITY 0.35f;

const int DOOR_HEIGHT = 68;
const int DOOR_WIDTH = 52;

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

typedef struct Door
{
	int x, y, w, h;
	bool isOpen;
} Door;
typedef struct
{
	int x, y, w, h;
} Ledge;

typedef struct
{
	object alice;
	Button startButton, exitButton, volumeButton;

	Ledge ledges[100];
	Door hole;
	int levelcount;
	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;

	int time;

	SDL_Renderer* renderer;
} GameState;

int processEvents(SDL_Window* wind, GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void process(GameState* game);
void loadGame(GameState* game);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
void toggleSound();
void handleEvents(SDL_Renderer* renderer, Button* startButton, Button* exitButton, Button* volumeButton, int* quit);
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Button* startButton, Button* exitButton, Button* volumeButton);
void cleanup(SDL_Texture* startButtonTexture, SDL_Texture* exitButtonTexture, SDL_Texture* backgroundTexture, SDL_Texture* volumeOn, SDL_Texture* volumeOff, SDL_Renderer* renderer, SDL_Window* window);


#endif
