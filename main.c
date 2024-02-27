#include "header.h"

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

	while (1)
	{
		processEvents(wind, &gameState);
		doRnder(renderer, &gameState);
		process(&gameState);
		collisionDetect(&gameState);
		SDL_Delay(15);
	}
	SDL_DestroyWindow(wind);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
	return 0;
}
