#include "../inc/header.h"


int main() {

	GameState game;

	SDL_Init(SDL_INIT_VIDEO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_CreateWindow("EndGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	game.renderer = renderer;
	SDL_Surface* temp_surf = NULL;
	SDL_Texture* backgroundTexture = loadTexture(renderer, "../resourse/menu_b.bmp");
	const char* gameBackground = "../resourse/level0/level0.bmp";
    const char* gameMusic = "../resourse/audio/level0.wav";
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(300);

	SDL_Texture* startButtonTexture = loadTexture(renderer, "../resourse/buttons/start.bmp");
	SDL_Texture* exitButtonTexture = loadTexture(renderer, "../resourse/buttons/quit.bmp");
	SDL_Texture* volumeOn = loadTexture(renderer, "../resourse/buttons/vol1.bmp");
	SDL_Texture* volumeOff = loadTexture(renderer, "../resourse/buttons/vol2.bmp");

	
	game.startButton.texture = startButtonTexture;
	game.startButton.rect.x = 518;
	game.startButton.rect.y = 350;
	game.startButton.rect.w = 250;
	game.startButton.rect.h = 100;

	game.exitButton.texture = exitButtonTexture;
	game.exitButton.rect.x = 532;
	game.exitButton.rect.y = 500;
	game.exitButton.rect.w = 220;
	game.exitButton.rect.h = 90;

	game.volumeButton.texture = volumeOn;
	game.volumeButton.rect.x = 1185;
	game.volumeButton.rect.y = 50;
	game.volumeButton.rect.w = 45;
	game.volumeButton.rect.h = 45;

	backgroundMusic = Mix_LoadMUS("../resourse/audio/menu.wav");
	if (!backgroundMusic) {
		printf("Failed to load background music: %s\n", Mix_GetError());
	}
	else {
		Mix_PlayMusic(backgroundMusic, -1);
	}

	int quit = 0;
	int start = 0;
	SDL_Event e;

	while (!quit && !start) {
		handleEvents(renderer, &game.startButton, &game.exitButton, &game.volumeButton, &quit, &start);
		render(renderer, backgroundTexture, &game.startButton, &game.exitButton, &game.volumeButton);
		SDL_Delay(16);  // 60 frames 
	}
	if (start == 1)
	{
		loadGame(&game, gameBackground, gameMusic);

		int done = 0;

		//Event loop
		while (!done)
		{
			//Check for events
			done = processEvents(window, &game);

			process(&game);
			collisionDetect(&game);

			//Render display
			doRender(renderer, &game);

			//don't burn up the CPU
			SDL_Delay(16);
		}
	}

	cleanup(startButtonTexture, exitButtonTexture, backgroundTexture, volumeOn, volumeOff, renderer, window);

	return 0;
}
