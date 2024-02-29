#pragma oncce

#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <time.h>

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

typedef struct Collectable
{
	int x, y, w, h;
	bool isCollected;
} Collectable;

typedef struct
{
	//Players
	object alice;

	//Ledges
	Ledge ledges[15];

	Collectable collectable;
	int counter;

	//Images
	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;
	SDL_Texture* collactbleImage;
	int time;

	//Renderer
	SDL_Renderer* renderer;
} GameState;

void initCollectableAboveLedge(GameState* game);
void loadGame(GameState* game);
void renderCounterText(int counter, SDL_Renderer* renderer);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);
int processEvents(SDL_Window* window, GameState* game);
void process(GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void renderCallectable(SDL_Renderer* renderer, GameState* game);

#endif
