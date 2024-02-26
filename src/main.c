#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    SDL_Texture *texture;
    SDL_Rect rect;
} Button;

Mix_Music *backgroundMusic = NULL; 
int soundOn = 1;

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

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "EndGame",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *backgroundTexture = loadTexture(renderer, "menu_b.bmp");

    if (!backgroundTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *startButtonTexture = loadTexture(renderer, "start.bmp");
    SDL_Texture *exitButtonTexture = loadTexture(renderer, "quit.bmp");
    SDL_Texture *volumeOn = loadTexture(renderer, "vol1.bmp");
    SDL_Texture *volumeOff = loadTexture(renderer, "vol2.bmp");

    if (!startButtonTexture || !exitButtonTexture || !volumeOn || !volumeOff ) {
        SDL_DestroyTexture(startButtonTexture);
        SDL_DestroyTexture(exitButtonTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(volumeOn);
        SDL_DestroyTexture(volumeOff);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    Button startButton;
    startButton.texture = startButtonTexture;
    startButton.rect.x = 320; // horizontal
    startButton.rect.y = 100; // vertical
    startButton.rect.w = 150; // width 
    startButton.rect.h = 150; // height 
    
    Button exitButton;
    exitButton.texture = exitButtonTexture;
    exitButton.rect.x = 320; // horizontal
    exitButton.rect.y = 200; // vertical
    exitButton.rect.w = 150; // width 
    exitButton.rect.h = 150; // height 
    
    Button volumeButton;
    volumeButton.texture = volumeOn;
    volumeButton.rect.x = 700;
    volumeButton.rect.y = 50;
    volumeButton.rect.w = 40;
    volumeButton.rect.h = 40;

    backgroundMusic = Mix_LoadMUS("menu.wav");
    if (!backgroundMusic) {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(backgroundMusic, -1); 
    }

    // loop
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton.rect)) {
                    printf("Start button clicked!\n");
                }

                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &exitButton.rect)) {
                    printf("Exit button clicked!\n");
                    quit = 1; // Exit 
                }

                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &volumeButton.rect)) {
                    toggleSound(); // Toggle sound on/off

                    // Change volume button 
                    if (soundOn) {
                        volumeButton.texture = volumeOn;
                    } else {
                        volumeButton.texture = volumeOff;
                    }
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_RenderCopy(renderer, startButton.texture, NULL, &startButton.rect);
        SDL_RenderCopy(renderer, exitButton.texture, NULL, &exitButton.rect);
        SDL_RenderCopy(renderer, volumeButton.texture, NULL, &volumeButton.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // 60 frames per second
    }

    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(volumeOn);
    SDL_DestroyTexture(volumeOff);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}