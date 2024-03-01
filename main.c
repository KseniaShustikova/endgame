#include <stdbool.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <time.h>

#pragma warning(disable : 4996)


#define GRAVITY  0.4f
const int ALICE_STAR_POSITION_X = 100 - 40;
const int ALICE_STAR_POSITION_Y = 240 - 40;
const int FLOR_POSITION_Y = 700;
const int LEDGE_HEIGHT = 64;
const int LEDGE_WIDTH = 256;
const int COLLACTABLE_SIZE = 48;
const int DOOR_HEIGHT = 64;
const int DOOR_WIDTH = 48;

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

typedef struct Door
{
	int x, y, w, h;
} Door;

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
	Ledge ledges[100];

	Door door;

	Collectable collectable;
	int collactableCounter;
	int levelCounter;

	//Images
	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;
	SDL_Texture* doorImage;
	SDL_Texture* doorImageLevel3;
	SDL_Texture* collactbleImage;
	SDL_Texture* backgroundLeve1Texture;
	SDL_Texture* backgroundLevel2Texture;
	SDL_Texture* backgroundLevel3Texture;
	int time;
	SDL_Texture* texture; //end
	//Renderer
	SDL_Renderer* renderer;
	bool isEnd;

} GameState;

void initCollectableAboveLedge(GameState* game);
void loadGame(GameState* game);
void renderCounterText(int counter, SDL_Renderer* renderer);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);
int processEvents(SDL_Window* window, GameState* game);
void teleportAliceToStartPosition(object* alice);
bool isAliceOnTheFloor(object* alice);
void process(GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void renderCallectable(SDL_Renderer* renderer, GameState* game);
bool isAliceRichedTheDoor(object* alice, Door* door);
void clearLedges(GameState* game);
void DisplayEndPicture(SDL_Renderer* renderer, char* path, GameState* game);

int main(int argc, char* argv[])
{
	GameState gameState;
	SDL_Window* window = NULL; // Declare a window
	SDL_Renderer* renderer = NULL; // Declare a renderer

	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Alice's Adventures: Tea Odyssey", // window title
		SDL_WINDOWPOS_UNDEFINED, // initial x position
		SDL_WINDOWPOS_UNDEFINED, // initial y position
		1280, // width, in pixels
		720, // height, in pixels
		0 // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;

	loadGame(&gameState);

	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;


	//Event loop
	while (!done)
	{
		//Check for events
		done = processEvents(window, &gameState);

		process(&gameState);
		collisionDetect(&gameState);

		//Render display
		doRender(renderer, &gameState);

		//don't burn up the CPU
		SDL_Delay(16);
	}


	//Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.aliceFrames[0]);
	SDL_DestroyTexture(gameState.aliceFrames[1]);
	SDL_DestroyTexture(gameState.aliceFrames[2]);
	SDL_DestroyTexture(gameState.aliceFrames[3]);
	SDL_DestroyTexture(gameState.aliceFrames[4]);
	SDL_DestroyTexture(gameState.aliceFrames[5]);
	SDL_DestroyTexture(gameState.tiles);
	SDL_DestroyTexture(gameState.doorImage);
	SDL_DestroyTexture(gameState.backgroundLeve1Texture);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	SDL_Quit();
	return 0;
}

void loadGame(GameState* game)
{
	SDL_Surface* surface = NULL;

	//Load images and create rendering textures from them

	surface = IMG_Load("images/Alice_stand.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_stand.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/Alice_1.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_1.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/Alice_2.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_2.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/Alice_3.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_3.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/Alice_4.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_4.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/Alice_J.png");
	if (surface == NULL)
	{
		printf("Cannot find Alice_J.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	game->aliceFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/tiles1.png");
	game->tiles = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/door.bmp");
	game->doorImage = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/door1.bmp");
	game->doorImageLevel3 = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/cake.png");
	game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/level1.bmp");
	game->backgroundLeve1Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/level2.bmp");
	game->backgroundLevel2Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/level3.bmp");
	game->backgroundLevel3Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	if (surface == NULL)
	{
		printf("Cannot find background image: %s\n\n");
		SDL_Quit();
		exit(1);
	}


	game->alice.x = ALICE_STAR_POSITION_X;
	game->alice.y = ALICE_STAR_POSITION_Y;
	game->alice.dx = 0;
	game->alice.dy = 0;
	game->alice.onLedge = 0;
	game->alice.animFrame = 0;
	game->alice.facingLeft = 1;
	game->alice.slowingDown = 0;

	game->time = 0;
	game->isEnd = false;

	//init ledges
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = LEDGE_WIDTH;
		game->ledges[i].h = LEDGE_HEIGHT;
		game->ledges[i].x = i * 256;
		game->ledges[i].y = FLOR_POSITION_Y;
	}

	game->ledges[99].x = 0;
	game->ledges[99].y = 550;

	game->ledges[98].x = 550;
	game->ledges[98].y = 550;

	game->ledges[97].x = 1050;
	game->ledges[97].y = 250;

	game->ledges[96].x = 300;
	game->ledges[96].y = 400;

	game->ledges[95].x = 0;
	game->ledges[95].y = 250;

	game->ledges[94].x = 800;
	game->ledges[94].y = 400;

	game->door.x = 1150;
	game->door.y = 100;
	game->door.w = 70;
	game->door.h = 150;

	game->collactableCounter = 0;
	game->levelCounter = 1;
	game->collectable.isCollected = false;

	initCollectableAboveLedge(game);
}



void initCollectableAboveLedge(GameState* game)
{
	game->collectable.x = game->ledges[99].x + LEDGE_WIDTH / 2 - COLLACTABLE_SIZE / 2;
	game->collectable.y = game->ledges[99].y - LEDGE_HEIGHT - 20;
	game->collectable.w = COLLACTABLE_SIZE;
	game->collectable.h = COLLACTABLE_SIZE;
}

int processEvents(SDL_Window* window, GameState* game)
{
	SDL_Event event;
	int done = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			case SDLK_UP:
				if (game->alice.onLedge)
				{
					game->alice.dy = -8;
					game->alice.onLedge = 0;
				}
				break;
			}
		}
		break;
		case SDL_QUIT:
			//quit out of the game
			done = 1;
			break;
		}
	}

	//More jumping
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP])
	{
		game->alice.dy -= 0.2f;
	}

	//Walking
	if (state[SDL_SCANCODE_LEFT])
	{
		game->alice.dx -= 0.5;
		if (game->alice.dx < -6)
		{
			game->alice.dx = -6;
		}
		game->alice.facingLeft = 1;
		game->alice.slowingDown = 0;
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		game->alice.dx += 0.5;
		if (game->alice.dx > 6)
		{
			game->alice.dx = 6;
		}
		game->alice.facingLeft = 0;
		game->alice.slowingDown = 0;
	}
	else
	{
		game->alice.animFrame = 0;
		game->alice.dx *= 0.8f;
		game->alice.slowingDown = 1;
		if (fabsf(game->alice.dx) < 0.1f)
		{
			game->alice.dx = 0;
		}
	}

	return done;
}

int isAliceRichedCollectable(object* alice, Collectable* collectable)
{
	return collide2d(
		alice->x,
		alice->y,
		collectable->x,
		collectable->y,
		alice->dx,
		alice->dy,
		collectable->w,
		collectable->h);
}

void initLevel2Ledges(GameState* game)
{
	game->ledges[0].x = 180;
	game->ledges[0].y = FLOR_POSITION_Y;
	game->ledges[0].w = 300;
	game->ledges[0].h = 50;

	game->ledges[1].x = 0;
	game->ledges[1].y = 600;
	game->ledges[1].w = 210;
	game->ledges[1].h = 150;

	game->ledges[2].x = 250;
	game->ledges[2].y = 470;
	game->ledges[2].w = 150;
	game->ledges[2].h = 35;

	game->ledges[3].x = 400;
	game->ledges[3].y = 350;
	game->ledges[3].w = 150;
	game->ledges[3].h = 35;

	game->ledges[4].x = 550;
	game->ledges[4].y = 250;
	game->ledges[4].w = 150;
	game->ledges[4].h = 35;

	game->ledges[5].x = 1000;
	game->ledges[5].y = 450;
	game->ledges[5].w = 50;
	game->ledges[5].h = 35;

	game->ledges[6].x = 0;
	game->ledges[6].y = 150;
	game->ledges[6].w = 150;
	game->ledges[6].h = 35;

	game->ledges[7].x = 170;
	game->ledges[7].y = 250;
	game->ledges[7].w = 150;
	game->ledges[7].h = 35;

	game->ledges[8].x = 1120;
	game->ledges[8].y = 350;
	game->ledges[8].w = 160;
	game->ledges[8].h = 35;

	game->ledges[9].x = 450;
	game->ledges[9].y = FLOR_POSITION_Y;
	game->ledges[9].w = 300;
	game->ledges[9].h = 50;

	game->ledges[10].x = 650;
	game->ledges[10].y = FLOR_POSITION_Y;
	game->ledges[10].w = 300;
	game->ledges[10].h = 50;

	game->ledges[11].x = 850;
	game->ledges[11].y = FLOR_POSITION_Y;
	game->ledges[11].w = 300;
	game->ledges[11].h = 50;

	game->ledges[12].x = 1000;
	game->ledges[12].y = FLOR_POSITION_Y;
	game->ledges[12].w = 300;
	game->ledges[12].h = 50;

	game->ledges[13].x = 850;
	game->ledges[13].y = 150;
	game->ledges[13].w = 50;
	game->ledges[13].h = 35;

	game->ledges[14].x = 800;
	game->ledges[14].y = 570;
	game->ledges[14].w = 150;
	game->ledges[14].h = 35;
}


void initLevel2Collectable(GameState* game)
{
	SDL_Surface* surface = IMG_Load("images/cookie1.bmp");
	game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
	game->collectable.x = 850;
	game->collectable.y = 97;
	game->collectable.w = 32;
	game->collectable.h = 32;
}

void initLevel2Door(GameState* game)
{
	game->door.x = 1150;
	game->door.y = 200;
}


void initLevel3Ledges(GameState* game);

void process(GameState* game)
{
	//add time
	game->time++;

	//man movement
	object* alice = &game->alice;
	alice->x += alice->dx;
	alice->y += alice->dy;

	if (alice->dx != 0 && alice->onLedge && !alice->slowingDown)
	{
		if (game->time % 8 == 0)
		{
			if (alice->animFrame != 4 && alice->animFrame != 5)
			{
				alice->animFrame++;
			}
			else
			{
				alice->animFrame = 1;
			}
		}
	}
	else if (!alice->onLedge)
	{
		alice->animFrame = 5;
	}
	else
	{
		alice->animFrame = 0;
	}

	if (isAliceOnTheFloor(alice))
	{
		teleportAliceToStartPosition(alice);
	}

	Collectable* collectable = &game->collectable;
	if (isAliceRichedCollectable(alice, collectable))
	{
		if (collectable->isCollected == false)
		{
			game->collactableCounter++;
		}

		collectable->isCollected = true;
		collectable->x = 0;
		collectable->y = 0;

	}

	if (isAliceRichedTheDoor(&game->alice, &game->door))
	{
		game->collectable.isCollected = false;
		game->levelCounter++;
		teleportAliceToStartPosition(alice);
	}

	if (game->levelCounter == 2)
	{
		clearLedges(game);
		initLevel2Ledges(game);
		initLevel2Collectable(game);
		initLevel2Door(game);
	}
	else if (game->levelCounter == 3)
	{
		clearLedges(game);
		initLevel3Ledges(game);
		game->collectable.x = game->ledges[96].x + game->ledges[96].w / 2 - COLLACTABLE_SIZE / 2;
		game->collectable.y = game->ledges[96].y - game->ledges[96].h - 20;
		game->collectable.w = 32;
		game->collectable.h = 32;
		game->door.x = game->ledges[89].x + game->ledges[89].w / 3 - DOOR_WIDTH / 2;
		game->door.y = game->ledges[89].y - game->ledges[89].h - 1;
		game->door.w = DOOR_WIDTH;
		game->door.h = DOOR_HEIGHT;
		SDL_Surface* surface = IMG_Load("images/dish.bmp");
		game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
		SDL_FreeSurface(surface);

	}
	else if (game->levelCounter == 4)
	{
		game->isEnd = true;
	}
	alice->dy += GRAVITY;

}

void initLevel3Ledges(GameState* game)
{
	//init ledges
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = LEDGE_WIDTH;
		game->ledges[i].h = LEDGE_HEIGHT;
		game->ledges[i].x = i * LEDGE_WIDTH;
		game->ledges[i].y = FLOR_POSITION_Y;


		int baseX = 1;
		int baseY = 660;
		int xDiff = 256;
		int yDiff = 108;

		game->ledges[99].x = baseX;
		game->ledges[99].y = baseY;
		game->ledges[99].h = 60;

		game->ledges[98].x = baseX + xDiff;
		game->ledges[98].y = baseY - yDiff;
		game->ledges[98].w = 208;

		game->ledges[97].x = baseX + 2 * xDiff;
		game->ledges[97].y = baseY - 2 * yDiff;
		game->ledges[97].w = 208;

		//cookie
		game->ledges[86].x = baseX + 3.5 * xDiff;
		game->ledges[86].y = baseY - 2.1 * yDiff;
		game->ledges[86].w = 148;

		game->ledges[87].x = baseX + 3.5 * xDiff;
		game->ledges[87].y = baseY - 2.3 * yDiff;
		game->ledges[87].w = 148;

		game->ledges[96].x = baseX + 3.5 * xDiff;
		game->ledges[96].y = baseY - 2.5 * yDiff;
		game->ledges[96].w = 148;

		//1 way
		game->ledges[95].x = baseX + 4.5 * xDiff;
		game->ledges[95].y = baseY - yDiff;
		game->ledges[95].w = 208;

		game->ledges[94].x = baseX + 6 * xDiff;
		game->ledges[94].y = baseY - 2 * yDiff;
		game->ledges[94].w = 148;

		game->ledges[93].x = baseX + 5 * xDiff;
		game->ledges[93].y = baseY - 3.5 * yDiff;
		game->ledges[93].w = 148;

		//top 
		game->ledges[92].x = baseX + 3.4 * xDiff;
		game->ledges[92].y = baseY - 4.5 * yDiff;
		game->ledges[92].w = 180;

		game->ledges[91].x = baseX + 3.1 * xDiff;
		game->ledges[91].y = game->ledges[92].y - game->ledges[91].h;
		game->ledges[91].w = 128;

		//2 way
		game->ledges[90].x = baseX + 1.3 * xDiff;
		game->ledges[90].y = baseY - 3.2 * yDiff;
		game->ledges[90].w = 128;
		game->ledges[90].h = 56;

		//top 
		game->ledges[89].x = baseX + 32;
		game->ledges[89].y = baseY - 4.5 * yDiff;
		game->ledges[89].w = 180;


		game->ledges[88].x = baseX - 64;
		game->ledges[88].y = game->ledges[89].y - game->ledges[88].h - 16;
		game->ledges[88].w = 128;
	}
}


bool isAliceOnTheFloor(object* alice)
{
	return alice->y >= FLOR_POSITION_Y - LEDGE_HEIGHT;
}

bool isAliceRichedTheDoor(object* alice, Door* door)
{
	return collide2d(
		alice->x,
		alice->y,
		door->x,
		door->y,
		alice->dx,
		alice->dy,
		door->w,
		door->h);
}

void teleportAliceToStartPosition(object* alice)
{
	alice->x = ALICE_STAR_POSITION_X;
	alice->y = ALICE_STAR_POSITION_Y;
}

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

void clearLedges(GameState* game)
{
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = 0;
		game->ledges[i].h = 0;
		game->ledges[i].x = 0;
		game->ledges[i].y = 0;
	}
}
void collisionDetect(GameState* game)
{
	//Check for collision with any ledges (brick blocks)
	for (int i = 0; i < 100; i++)
	{
		float mw = 32, mh = 64;
		float mx = game->alice.x, my = game->alice.y;
		float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

		if (game->alice.x < 0) {
			game->alice.x = 0;
		}
		if (game->alice.x + 32 > 1280) {
			game->alice.x = 1280 - 32;
		}
		if (game->alice.y < 0) {
			game->alice.y = 0;
		}
		if (game->alice.y + 64 > 720) {
			game->alice.y = 720 - 64;
			game->alice.dy = 0;
			game->alice.onLedge = 1;
		}

		if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
		{
			//are we bumping our head?
			if (my < by + bh && my > by && game->alice.dy < 0)
			{
				//correct y
				game->alice.y = by + bh;
				my = by + bh;

				//bumped our head, stop any jump velocity
				game->alice.dy = 0;
				game->alice.onLedge = 1;
			}
		}
		if (mx + mw > bx && mx < bx + bw)
		{
			//are we landing on the ledge
			if (my + mh > by && my < by && game->alice.dy > 0)
			{
				//correct y
				game->alice.y = by - mh;
				my = by - mh;

				//landed on this ledge, stop any jump velocity
				game->alice.dy = 0;
				game->alice.onLedge = 1;
			}
		}


		if (my + mh > by && my < by + bh)
		{
			//rubbing against right edge
			if (mx < bx + bw && mx + mw > bx + bw && game->alice.dx < 0)
			{
				//correct x
				game->alice.x = bx + bw;
				mx = bx + bw;

				game->alice.dx = 0;
			}
			//rubbing against left edge
			else if (mx + mw > bx && mx < bx && game->alice.dx > 0)
			{
				//correct x
				game->alice.x = bx - mw;
				mx = bx - mw;

				game->alice.dx = 0;
			}
		}
	}
}

void doRender(SDL_Renderer* renderer, GameState* game)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white

	if (game->levelCounter == 2)
	{
		SDL_RenderCopy(renderer, game->backgroundLevel2Texture, NULL, NULL);
	}
	else if (game->levelCounter >= 3)
	{
		SDL_RenderCopy(renderer, game->backgroundLevel3Texture, NULL, NULL);
	}
	else
	{
		SDL_RenderCopy(renderer, game->backgroundLeve1Texture, NULL, NULL);
	}

	for (int i = 0; i < 100; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
		SDL_RenderCopy(renderer, game->tiles, NULL, &ledgeRect);
	}

	SDL_Rect DoorRect = { game->door.x, game->door.y, game->door.w, game->door.h };
	if (game->levelCounter == 3)
	{
		SDL_RenderCopy(renderer, game->doorImageLevel3, NULL, &DoorRect);
	}
	else
	{
		SDL_RenderCopy(renderer, game->doorImage, NULL, &DoorRect);
	}

	//draw a rectangle at man's position
	SDL_Rect rect = { game->alice.x, game->alice.y, 32, 64 };
	SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
		NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));

	renderCallectable(renderer, game);
	if (game->collactableCounter > 0)
	{
		renderCounterText(game->collactableCounter, renderer);
	}

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);

	if (game->isEnd)
	{
		if (game->collactableCounter == 3) {
			DisplayEndPicture(renderer, "images/good_end.bmp", &game);
		}
		else {
			DisplayEndPicture(renderer, "images/bad_end.bmp", &game);
		}
	}
}

void DisplayEndPicture(SDL_Renderer* renderer, char* path, GameState* game) {

	SDL_Surface* surface;
	surface = IMG_Load(path);
	if (!surface) {
		printf("Error loading image: %s\n", IMG_GetError());
		return;
	}
	game->texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (SDL_GetError() && SDL_GetError()[0] != '\0') {
		printf("SDL Render Error: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(surface);

	SDL_Rect endimg = { 1, 1, 1280, 720 };
	SDL_RenderCopy(renderer, game->texture, NULL, &endimg);
}


void renderCallectable(SDL_Renderer* renderer, GameState* game)
{
	if (game->collectable.isCollected)
	{
		SDL_RenderCopy(renderer, game->collactbleImage, NULL, &(SDL_Rect){0, 0, 0, 0});
	}
	else
	{
		SDL_Rect collectableRect = { game->collectable.x, game->collectable.y, game->collectable.w, game->collectable.h };
		SDL_RenderCopy(renderer, game->collactbleImage, NULL, &collectableRect);
	}
}

// to set the text on top of the collactableCounter
void renderCounterText(int counter, SDL_Renderer* renderer)
{
	char text[20];
	char counterText[2];
	SDL_Color white = { 255, 255, 255,255 };
	TTF_Font* Sans;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;

	sprintf(counterText, "%d", counter);
	sprintf(text, "%s %s", "items collected: ", counterText);
	Sans = TTF_OpenFont("./resourse/fonts/font.ttf", 12);
	if (!Sans)
		printf("%s", TTF_GetError());
	surfaceMessage = TTF_RenderText_Solid(Sans, text, white);
	if (!surfaceMessage)
		printf("%s", TTF_GetError());
	Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	if (!Message)
		printf("%s", TTF_GetError());

	SDL_RenderCopy(renderer, Message, NULL, &(SDL_Rect){0, 0, 200, 50});
}

