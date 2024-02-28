#include "./collectable.h"

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

    counter->rect = (SDL_Rect){0, 0, 200, 30};
    counter->texture = Message;
}

void DisplayLetterText(SDL_Renderer *renderer){
    SDL_Color black = {0, 0, 0, 255};
    TTF_Font* Sans;
    SDL_Surface* surfaceMessage;
    SDL_Texture* texture; 
    int texW = 0;
    int texH = 0;
    int marginX = 200;
    int marginY = 100;
    SDL_Rect background;

    msg.text = "Привіт, Алісо! Ласкаво просимо до Країни Чудес! Ми вже чекаємо на тебе на нашому святковому чаюванні, але, здається, ми загубили кілька предметів. Знайди їх, будь ласка, щоб злий Оракл не вийшов на наш слід! Він може зловити тебе, будь обережною!";
    Sans = TTF_OpenFont("./resource/fonts/font.ttf", 20);
    if(!Sans)
        printf("%s", TTF_GetError());
    //replace 640 with window width
    surfaceMessage =  TTF_RenderUTF8_Blended_Wrapped(Sans, msg.text, black, 1280-marginX);
    if(!surfaceMessage)
        printf("%s", TTF_GetError()); 
    texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if(!texture)
        printf("%s", TTF_GetError());
    msg.texture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    //replace 400 with window height
    msg.rect = (SDL_Rect){marginX, 720-marginY, texW - marginX, texH};
    background = (SDL_Rect){marginX, 720-marginY, texW - marginX, texH};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer,255,255 ,255,100);
    SDL_RenderFillRect(renderer,&background);

    SDL_RenderCopy(renderer, msg.texture, NULL, &msg.rect);
}

// to render the collectable if the player hasn't collected it yet
void renderCollectable (Collectable *element, SDL_Renderer *renderer, SDL_Rect *player, int type ){
    SDL_bool collision = SDL_HasIntersection(player, &element->rect);
    if(collision){
        element->isCollected = true;
        element->rect = (SDL_Rect){0, 0, 0, 0};
        free(element->image);
        if(type == 1)
            collectableCounter.counter++;
        if(type == 2)
            msg.isOn = true;
        SetCounterText(&collectableCounter, renderer);
    }
    if(!element->isCollected)
        SDL_RenderCopy(renderer, element->image, NULL, &element->rect);
}
// display the counter on top of the screen
void displayText (SDL_Renderer *renderer, counter *counter){
    if(!counter->texture)
        printf("error");
    if (collectableCounter.counter > 0)
        SDL_RenderCopy(renderer, counter->texture, NULL, &counter->rect);
}