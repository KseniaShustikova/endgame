#include"./collectable.h"

#undef main 

// global counter
counter collectableCounter;

// function to make a new collectable
Collectable makeCollectable ( int x, int y, int width, int height) {
    Collectable collectable;
    collectable.rect = (SDL_Rect){x, y, width, height};
    collectable.isCollected = false;
    return collectable;
}
// to set the image of the collectable
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path){
    SDL_Texture *cake = NULL;
    SDL_Surface *surface = SDL_LoadBMP(path);
    cake = SDL_CreateTextureFromSurface(renderer, surface);

    element->image = cake;
    SDL_FreeSurface(surface);
}
// to set the text on top of the counter
void SetCounterText(counter  *counter, SDL_Renderer *renderer ){
    char text[20] ;
    char counterText[2];
    SDL_Color black = {0, 0, 0, 255};
    TTF_Font* Sans;
    SDL_Surface* surfaceMessage;
    SDL_Texture* Message;

    sprintf(counterText,"%d",counter->counter) ;
    sprintf(text, "%s %s", "items collected: ", counterText); 
    Sans = TTF_OpenFont("./resource/fonts/font.ttf", 12);
    if(!Sans)
        printf("%s", TTF_GetError());
    surfaceMessage = TTF_RenderText_Solid(Sans, text, black);
    if(!surfaceMessage)
        printf("%s", TTF_GetError()); 
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if(!Message)
        printf("%s", TTF_GetError()); 

    counter->rect = (SDL_Rect){0, 0, 200, 50};
    counter->texture = Message;
}
// to render the collectable if the player hasn't collected it yet
void renderCollectable (Collectable *element, SDL_Renderer *renderer, SDL_Rect *player ){
    SDL_bool collision = SDL_HasIntersection(player, &element->rect);
    if(collision){
        element->isCollected = true;
        element->rect = (SDL_Rect){0, 0, 0, 0};
        free(element->image);
        collectableCounter.counter++;
        SetCounterText(&collectableCounter, renderer);
    }
    if(!element->isCollected)
        SDL_RenderCopy(renderer, element->image, NULL, &element->rect);
}
// display the counter on top of the screen
void displayText (SDL_Renderer *renderer, counter *counter){
    if(!counter->texture)
        printf("error");
    SDL_RenderCopy(renderer, counter->texture, NULL, &counter->rect);
}

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
        
        renderCollectable(&cake, renderer, &player);
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