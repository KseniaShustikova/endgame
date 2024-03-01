#include"./collectable.h"

#undef main 

// global counter
counter collectableCounter;

int main() {

    bool quit = false;
    SDL_Event event;
    int x = 288;
    int y = 208;
// init SDL
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 Bounding Box Collision Detection",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    

    SDL_Texture *alice = NULL;
    SDL_Surface *surface = SDL_LoadBMP("resource/images/alice.bmp");
    alice = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect player = { x, y, 32, 64 };
    Collectable cake = makeCollectable(150, 100, 32, 32);
    SetCollectableImage(&cake, renderer, "./resource/images/cake.bmp");
    SetCounterText(&collectableCounter, renderer);

 
// handle events
 
    while (!quit)
    {
        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            // TODO keyboard input handling goes here
        }
    
        // TODO rendering & collision detection goes here 
        
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderClear(renderer);

        //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//green
        SDL_RenderCopy(renderer, alice, NULL, &player);
        
        renderCollectable(&cake, renderer, &player, type_collectable);
        displayText(renderer, &collectableCounter);

        SDL_RenderPresent(renderer);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:  player.x -=3; break;
                    case SDLK_RIGHT: player.x+=3; break;
                    case SDLK_UP:    player.y-=3; break;
                    case SDLK_DOWN:  player.y+=3; break;
                    case SDLK_ESCAPE: quit = true;
                }
                break;
        }
        SDL_RenderPresent(renderer);
      
    }
    
    // cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}