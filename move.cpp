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

void loadGame(GameState* game) {
	//cube
	game->cube.x = 600;
	game->cube.y = 400;
	game->cube.dy = 0;
	// init blocks
	for (int i = 0; i < 100; i++) {  
		game->block[i].w = 256;
		game->block[i].h = 64;
		game->block[i].x = i * 256;
		game->block[i].y = 600;
 }
	game->block[99].x = 350;
	game->block[99].y = 400;
}
// close program and keys 
void processEvents(SDL_Window* wind, GameState *game)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (wind)
			{
				SDL_DestroyWindow(wind);
				wind = NULL;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				SDL_DestroyWindow(wind);
				wind = NULL;
				SDL_Quit();
				exit(0); 
				break;
			}
		}
		break;
		case SDL_QUIT:            
			SDL_DestroyWindow(wind);
			wind = NULL;
			SDL_Quit();
			exit(0);  
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		game->cube.x -= 10;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		game->cube.x += 10;
	}
	if (state[SDL_SCANCODE_UP])
	{
		if (!game->cube.dy)
		{
			game->cube.dy = -15;
		}
	}
}

void jump(GameState* game)
{
	game->cube.y += game->cube.dy;
	game->cube.dy += GRAVITY;
}

// дотик до блоків 
void collisionDetect(GameState* game)
{
	for (int i = 0; i < 100; i++)
	{
		float cubew = 50, cubeh = 50;
		float cubex = game->cube.x, cubey = game->cube.y;
		float bx = game->block[i].x, by = game->block[i].y,
			bw = game->block[i].w, bh = game->block[i].h;

		if (cubey + cubeh > by && cubey < by + bh)
		{
			// дотик з правої сторони 
			if (cubex < bx + bw && cubex + cubew > bx + bw)
			{
				game->cube.x = bx + bw;
			}
			// дотик з лiвої сторони 
			else if (cubex + cubew > bx && cubex < bx)
			{
				game->cube.x = bx - cubew;
			}
		}
		if (cubex + cubew > bx && cubex < bx + bw)
		{ 
			// дотик знизу(головою) 
			if (cubey < by + bh && cubey + cubeh > by + bh)
			{
				game->cube.y = by + bh;
				game->cube.dy = 0;
			}
			// дотик зверху(ноги) 
		    else if (cubey + cubeh > by && cubey < by)
		    {
			game->cube.y = by - cubeh;
			game->cube.dy = 0;
		    }
		}
	}
}

void doRnder(SDL_Renderer* renderer,GameState* game) {
	
	SDL_SetRenderDrawColor(renderer, 164, 151, 227, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 254, 188, 99, 255);
	SDL_Rect rect = { game->cube.x, game->cube.y, 50, 50};
	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 255, 250, 250, 255);
	for (int i = 0; i < 100; i++)
	{
		SDL_Rect blockRect = { game->block[i].x,game->block[i].y, game->block[i].w,game->block[i].h };
		SDL_RenderFillRect(renderer, &blockRect);
	}
	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
	GameState gameState;
	SDL_Window* wind;
	SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_VIDEO);

	wind = SDL_CreateWindow("Endgame",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				1280,
				720,
				0
	            );
	
	renderer = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;
	
	loadGame(&gameState);
	
	while(1)
	{
	    processEvents(wind, &gameState);
		doRnder(renderer, &gameState);
		jump(&gameState);
		collisionDetect(&gameState);

		SDL_Delay(10);
	}
	SDL_DestroyWindow(wind);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
	return 0;
}
