#include "../inc/header.h"

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);

    if (!surface) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

void toggleSound() {
    if (soundOn) {
        Mix_PauseMusic();
    } else {
        Mix_ResumeMusic();
    }
    soundOn = !soundOn;
}

void handleEvents(SDL_Renderer *renderer, Button *startButton, Button *exitButton, Button *volumeButton, int *quit) {
   
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = 1;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton->rect)) {
                printf("Start button clicked!\n");
            }

            if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &exitButton->rect)) {
                printf("Exit button clicked!\n");
                *quit = 1;
            }

            if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &volumeButton->rect)) {
                toggleSound();
                if (soundOn) {
                    volumeButton->texture = loadTexture(renderer, "vol1.bmp");
                } else {
                    volumeButton->texture = loadTexture(renderer, "vol2.bmp");
                }
            }
        }
    }
}

void render(SDL_Renderer *renderer, SDL_Texture *backgroundTexture, Button *startButton, Button *exitButton, Button *volumeButton) {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, startButton->texture, NULL, &startButton->rect);
    SDL_RenderCopy(renderer, exitButton->texture, NULL, &exitButton->rect);
    SDL_RenderCopy(renderer, volumeButton->texture, NULL, &volumeButton->rect);

    SDL_RenderPresent(renderer);
}

void cleanup(SDL_Texture *startButtonTexture, SDL_Texture *exitButtonTexture, SDL_Texture *backgroundTexture, SDL_Texture *volumeOn, SDL_Texture *volumeOff, SDL_Renderer *renderer, SDL_Window *window) {
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(volumeOn);
    SDL_DestroyTexture(volumeOff);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}