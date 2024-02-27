#include "header.h"

void loadGame(GameState* game) {
	//cube
	game->cube.x = 600;
	game->cube.y = 400;
	game->cube.dy = 0;
	game->cube.dx = 0;
	game->cube.onLedge = 0;
	// init blocks
	for (int i = 0; i < 100; i++) 
	{  
		game->block[i].w = 148;
		game->block[i].h = 64;
		game->block[i].x = i * 148;
		game->block[i].y = 650;
    }
	game->block[85].x = 600;
	game->block[85].y = 500;

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
		game->cube.dx -= 0.5;
		if(game->cube.dx < -6);
		game->cube.dx = -6;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		game->cube.dx += 0.5;
		if (game->cube.dx > 6);
		game->cube.dx = 6;
	}
	else
	{
		game->cube.dx *= 0.8f;
		if(fabsf(game->cube.dx) < 0.1f)
			game->cube.dx = 0;
	}
	if (state[SDL_SCANCODE_UP])
	{
		if (!game->cube.dy)
		{
			game->cube.dy = -10;
		}
	}
}
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}
void process(GameState* game)
{
	//game->time++;
	game->cube.x += game->cube.dx;
	game->cube.y += game->cube.dy;
	game->cube.dy += GRAVITY;
}

// дотик до блоків 
void collisionDetect(GameState* game)
{
	for (int i = 0; i < 100; i++)
	{
		float mw = 48, mh = 48;
		float mx = game->cube.x, my = game->cube.y;
		float bx = game->block[i].x, by = game->block[i].y, 
			bw = game->block[i].w, bh = game->block[i].h;
		if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
		{
			//bumping our head
			if ((my < by + bh && my > by && game->cube.dy < 0) || my == 719)
			{
				game->cube.y = by + bh;
				
				game->cube.dy = 0;
				game->cube.onLedge = 1;
			}
		}
		if (mx + mw > bx && mx < bx + bw)
		{
			//landing 
			if ((my + mh > by && my < by && game->cube.dy > 0) || my == 719)
			{
				game->cube.y = by - mh;
				
				game->cube.dy = 0;
				game->cube.onLedge = 1;
			}
		}
		if (my + mh > by && my < by + bh)
		{
			//right edge
			if ((mx < bx + bw && mx + mw > bx + bw && game->cube.dx > 0) || mx == 1279)
			{
				//correct x
				game->cube.x = bx + bw;
				

				game->cube.dx = 0;
			}
			// left edge
			else if ((mx + mw > bx && mx < bx && game->cube.dx < 0 )|| mx == -1279)
			{
				game->cube.x = bx - mw;
				game->cube.dx = 0;
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

