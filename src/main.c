#include "../inc/header.h"


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *window = SDL_CreateWindow("EndGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * temp_surf = NULL;
    SDL_Texture *backgroundTexture = loadTexture(renderer, "../resourse/menu_b.bmp");
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(300);

    SDL_Texture *startButtonTexture = loadTexture(renderer, "../resourse/buttons/start.bmp");
    SDL_Texture *exitButtonTexture = loadTexture(renderer, "../resourse/buttons/quit.bmp");
    SDL_Texture *volumeOn = loadTexture(renderer, "../resourse/buttons/vol1.bmp");
    SDL_Texture *volumeOff = loadTexture(renderer, "../resourse/buttons/vol2.bmp");

    Button startButton;
    startButton.texture = startButtonTexture;
    startButton.rect.x = 518;
    startButton.rect.y = 350;
    startButton.rect.w = 250;
    startButton.rect.h = 100;

    Button exitButton;
    exitButton.texture = exitButtonTexture;
    exitButton.rect.x = 532;
    exitButton.rect.y = 500;
    exitButton.rect.w = 220;
    exitButton.rect.h = 90;

    Button volumeButton;
    volumeButton.texture = volumeOn;
    volumeButton.rect.x = 1100;
    volumeButton.rect.y = 50;
    volumeButton.rect.w = 45;
    volumeButton.rect.h = 45;

    backgroundMusic = Mix_LoadMUS("../resourse/audio/menu.wav");
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
