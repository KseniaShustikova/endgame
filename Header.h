#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>

#define GRAVITY 0.3f;

// cube
typedef struct
{
	float x, y;
	float dx, dy;
	char* name;
	int onLedge;
} object;

//платформа (по анології з об'єктом)
typedef struct
{
	float x, y, w, h;
} Block;

// to simplify use
typedef struct
{
	object cube;
	Block block[100];
	SDL_Renderer* renderer;
} GameState;

void processEvents(SDL_Window* wind, GameState* game);
void doRnder(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void process(GameState* game);
void loadGame(GameState* game);
