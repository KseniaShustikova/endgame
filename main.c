#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <time.h>

#define GRAVITY  0.35f

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
	//Players
	object alice;

	//Ledges
	Ledge ledges[100];

	//Images
	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;

	int time;

	//Renderer
	SDL_Renderer* renderer;
} GameState;

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

	

	game->alice.x = 320 - 40;
	game->alice.y = 240 - 40;
	game->alice.dx = 0;
	game->alice.dy = 0;
	game->alice.onLedge = 0;
	game->alice.animFrame = 0;
	game->alice.facingLeft = 1;
	game->alice.slowingDown = 0;

	game->time = 0;

	//init ledges
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = 256;
		game->ledges[i].h = 64;
		game->ledges[i].x = i * 256;
		game->ledges[i].y = 400;
	}
	game->ledges[99].x = 350;
	game->ledges[99].y = 200;

	game->ledges[98].x = 350;
	game->ledges[98].y = 350;
}

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
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

	

	alice->dy += GRAVITY;
}

void collisionDetect(GameState* game)
{
	//Check for collision with any ledges (brick blocks)
	for (int i = 0; i < 100; i++)
	{
		float mw = 32, mh = 64;
		float mx = game->alice.x, my = game->alice.y;
		float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;
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

void doRender(SDL_Renderer* renderer, GameState* game)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 100; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
		SDL_RenderCopy(renderer, game->tiles, NULL, &ledgeRect);
	}

	//draw a rectangle at man's position
	SDL_Rect rect = { game->alice.x, game->alice.y, 32, 64 };
	SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
		NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
	GameState gameState;
	SDL_Window* window = NULL;              // Declare a window
	SDL_Renderer* renderer = NULL;         // Declare a renderer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Alice's Adventures: Tea Odyssey",   // window title
		SDL_WINDOWPOS_UNDEFINED,                                   // initial x position
		SDL_WINDOWPOS_UNDEFINED,                                   // initial y position
		1280,                                                      // width, in pixels
		720,                                                       // height, in pixels
		0                                                          // flags
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
