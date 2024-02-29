#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <time.h>

#pragma warning(disable : 4996)

#define GRAVITY  0.6f
const int ALICE_STAR_POSITION_X = 50;
const int ALICE_STAR_POSITION_Y = 570;
const int DOOR_POSITION_X = 1230;
const int DOOR_POSIRTION_Y = 275;

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
void teleportAliceToStartPosition(object* alice);
bool isAliceOnTheFloor(object* alice);
void process(GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game);
void collisionDetect(GameState* game);
void renderCallectable(SDL_Renderer* renderer, GameState* game);

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

	surface = IMG_Load("images/3-Tile.png");
	game->tiles = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("images/dish.png");
	game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);


	game->alice.x = ALICE_STAR_POSITION_X;
	game->alice.y = ALICE_STAR_POSITION_Y;
	game->alice.dx = 0;
	game->alice.dy = 0;
	game->alice.onLedge = 0;
	game->alice.animFrame = 0;
	game->alice.facingLeft = 1;
	game->alice.slowingDown = 0;

	game->time = 0;

	//init ledges
	for (int i = 0; i < 15; i++)
	{
        game->ledges[0].x = 180;
	game->ledges[0].y = 690;
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
	game->ledges[9].y = 690;
	game->ledges[9].w = 300;
	game->ledges[9].h = 50;
	
	game->ledges[10].x = 650;
	game->ledges[10].y = 690;
	game->ledges[10].w = 300;
	game->ledges[10].h = 50;
	
	game->ledges[11].x = 850;
	game->ledges[11].y = 690;
	game->ledges[11].w = 300;
	game->ledges[11].h = 50;
	
	game->ledges[12].x = 1000;
	game->ledges[12].y = 690;
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
	
	game->counter = 0;
	game->collectable.isCollected = false;

	initCollectableAboveLedge(game);
}
}

void initCollectableAboveLedge(GameState* game)
{
	game->collectable.x = 850;
	game->collectable.y = 120;
	game->collectable.w = 32;
	game->collectable.h = 32;
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

	Collectable* collectable = &game->collectable;
	if (collide2d(
		alice->x,
		alice->y,
		collectable->x,
		collectable->y,
		alice->dx,
		alice->dy,
		collectable->w,
		collectable->h))
	{
		collectable->isCollected = true;
		collectable->x = 0;
		collectable->y = 0;
		game->counter++;
	}

	alice->dy += GRAVITY;
}

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

void collisionDetect(GameState* game)
{
	//Check for collision with any ledges (brick blocks)
	for (int i = 0; i < 15; i++)
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
	SDL_Surface* backgroundSurface = IMG_Load("images/level3.bmp");
        if (backgroundSurface == NULL) {
        printf("not: %s\n", IMG_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (backgroundTexture == NULL) {
        printf("not: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
    SDL_Surface* doorSurface = IMG_Load("images/door.bmp");
        if (doorSurface == NULL) {
        printf("not: %s\n", IMG_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* doorTexture = SDL_CreateTextureFromSurface(game->renderer, doorSurface);
    SDL_FreeSurface(doorSurface);
    if (doorTexture == NULL) {
        printf("not: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
        SDL_Rect backgroundRect = {0, 0, 1280, 720};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_Rect doorRect = {1230, 275, 50, 75};
        SDL_RenderCopy(renderer, doorTexture, NULL, &doorRect);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 15; i++)
	{
		SDL_Rect ledgeRect = {game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
		SDL_RenderCopy(renderer, game->tiles, NULL, &ledgeRect);
	}

	//draw a rectangle at man's position
	SDL_Rect rect = {game->alice.x, game->alice.y, 32, 64};
	SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
	                 NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));

	renderCallectable(renderer, game);

	renderCounterText(game->counter, renderer);
	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(doorTexture);
	SDL_RenderPresent(renderer);
}

void renderCallectable(SDL_Renderer* renderer, GameState* game)
{
	if (game->collectable.isCollected)
	{
           SDL_RenderCopy(renderer, game->collactbleImage, NULL, &(SDL_Rect){0, 0, 0, 0});
	}
	else
	{
		SDL_Rect collectableRect = {game->collectable.x, game->collectable.y, game->collectable.w, game->collectable.h};
		SDL_RenderCopy(renderer, game->collactbleImage, NULL, &collectableRect);
	}
}

// to set the text on top of the counter
void renderCounterText(int counter, SDL_Renderer* renderer)
{
	char text[20];
	char counterText[2];
	SDL_Color black = {255, 255, 255, 255};
	TTF_Font* Sans;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;

	sprintf(counterText, "%d", counter);
	sprintf(text, "%s %s", "items collected: ", counterText);
	Sans = TTF_OpenFont("./resource/fonts/font.ttf", 12);
	if (!Sans)
		printf("%s", TTF_GetError());
	surfaceMessage = TTF_RenderText_Solid(Sans, text, black);
	if (!surfaceMessage)
		printf("%s", TTF_GetError());
	Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	if (!Message)
		printf("%s", TTF_GetError());

	SDL_RenderCopy(renderer, Message, NULL, &(SDL_Rect){0, 0, 200, 50});
}
