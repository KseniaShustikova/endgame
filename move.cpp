#include <SDL.h>
#include <stdio.h>

typedef struct сoordinates
{
	int x, y;
}coord;

void processEvents(SDL_Window* wind, coord* c, bool& quit)
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
				quit = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = 1;
				break;
			case SDLK_LEFT:
				c->x -= 10;
				break;
			case SDLK_RIGHT:
				c->x += 10;
				break;
			case SDLK_UP:
				c->y -= 10;
				break;
			case SDLK_DOWN:
				c->y += 10;
				break;
			}
		}
		break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}
}
		
void doeRnderer(SDL_Renderer* render, coord *c) {
	
	SDL_SetRenderDrawColor(render, 164, 151, 227, 255);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(render, 254, 188, 99, 255);
	SDL_Rect rect = {c->x, c->y,80, 80};
	SDL_RenderFillRect(render, &rect);

	SDL_RenderPresent(render);
}

int main(int argc, char* args[]) {
	SDL_Window* wind;
	SDL_Renderer* render;

	SDL_Init(SDL_INIT_VIDEO);

	wind = SDL_CreateWindow("Endgame",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);
	coord c;
	c.x = 220;
	c.y = 140;

	render = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	bool quit = false;
	while (!quit)
	{
		doeRnderer(render, &c);
		processEvents(wind, &c, quit);
		SDL_Delay(20);
	}
	SDL_DestroyWindow(wind);
	SDL_DestroyRenderer(render);

	SDL_Quit();
	return 0;
}