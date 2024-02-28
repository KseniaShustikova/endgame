#include "game.h"


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *window = SDL_CreateWindow("EndGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *backgroundTexture = loadTexture(renderer, "./menu_b.bmp");
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(300);

    SDL_Texture *startButtonTexture = loadTexture(renderer, "./start.bmp");
    SDL_Texture *exitButtonTexture = loadTexture(renderer, "./quit.bmp");
    SDL_Texture *volumeOn = loadTexture(renderer, "./vol1.bmp");
    SDL_Texture *volumeOff = loadTexture(renderer, "./vol2.bmp");

    Button startButton;
    startButton.texture = startButtonTexture;
    startButton.rect.x = 518;
    startButton.rect.y = 100;
    startButton.rect.w = 250;
    startButton.rect.h = 250;

    Button exitButton;
    exitButton.texture = exitButtonTexture;
    exitButton.rect.x = 532;
    exitButton.rect.y = 250;
    exitButton.rect.w = 220;
    exitButton.rect.h = 220;

    Button volumeButton;
    volumeButton.texture = volumeOn;
    volumeButton.rect.x = 1090;
    volumeButton.rect.y = 50;
    volumeButton.rect.w = 60;
    volumeButton.rect.h = 60;

    backgroundMusic = Mix_LoadMUS("./menu.wav");
    if (!backgroundMusic) {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(backgroundMusic, -1);
    }

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        handleEvents(renderer, &startButton, &exitButton, &volumeButton, &quit);
        render(renderer, backgroundTexture, &startButton, &exitButton, &volumeButton);
        SDL_Delay(16);  // 60 frames 
    }

    cleanup(startButtonTexture, exitButtonTexture, backgroundTexture, volumeOn, volumeOff, renderer, window);

    return 0;
}