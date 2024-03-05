#include "../inc/header.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);

    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);   

    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

void renderCollectable (Collectable *element, SDL_Renderer *renderer, GameState* game, int type ){
    SDL_bool collision = SDL_HasIntersection(&(SDL_Rect){game->alice.x, game->alice.y, 42, 74}, &element->rect);

    if(collision){
        element->isCollected = true;
        element->rect = (SDL_Rect){0, 0, 0, 0};
        // free(element->image);
        if(type == 1)
            collectableCounter.counter++;
        if(type == 2)
            msg.isOn = true;
        SetCounterText(game);
    }
    if(!element->isCollected)
        SDL_RenderCopy(renderer, element->image, NULL, &element->rect);
}
void displayText (GameState *game){
    if(!game->collactableCounter.texture)
        printf("error");
    if (game->collactableCounter.counter > 0)
        SDL_RenderCopy(game->renderer, game->collactableCounter.texture, NULL, &game->collactableCounter.rect);
}

Collectable makeCollectable ( int x, int y, int width, int height) {
    Collectable collectable;
    collectable.rect = (SDL_Rect){x, y, width, height};
    collectable.isCollected = false;
    return collectable;
}

void SetCounterText(GameState *game) {
    char text[100] ;
    char counterText[2];
    SDL_Color black = {255, 255, 255, 255};
    TTF_Font* Sans;
    SDL_Surface* surfaceMessage;
    SDL_Texture* texture;
    int texW = 0;
    int texH = 0;
    sprintf(counterText,"%d", game->collactableCounter.counter);
    sprintf(text, "%s %s", "кількість зібраних предметів: ", counterText); 
    Sans = TTF_OpenFont("./resource/fonts/font.ttf", 24);
    if(!Sans)
        printf("%s", TTF_GetError());
    surfaceMessage =  TTF_RenderUTF8_Blended_Wrapped(Sans, text, black, 400);
    if(!surfaceMessage)
        printf("%s", TTF_GetError()); 
    texture = SDL_CreateTextureFromSurface(game->renderer, surfaceMessage);
    if(!texture)
        printf("%s", TTF_GetError()); 
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    game->collactableCounter.rect = (SDL_Rect){0, 0, texW, texH};
    game->collactableCounter.texture = texture;
}
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path){
    SDL_Texture *cake = NULL;
    SDL_Surface *surface = SDL_LoadBMP(path);
    cake = SDL_CreateTextureFromSurface(renderer, surface);
    element->image = cake;
    SDL_FreeSurface(surface);
}

void DisplayLetterText(SDL_Renderer *renderer){
    SDL_Color black = {0, 0, 0, 255};
    TTF_Font* Sans;
    SDL_Surface* surfaceMessage;
    SDL_Texture* texture; 
    int texW = 0;
    int texH = 0;
    int marginX = 150;
    int marginY = 100;
    SDL_Rect background;
    msg.text = "Привіт, Алісо! Ласкаво просимо до Країни Чудес! Ми вже чекаємо на тебе на нашому святковому чаюванні, але, здається, ми загубили кілька предметів. Знайди їх, будь ласка, щоб злий Оракл не вийшов на наш слід! Він може зловити тебе, будь обережною!\0";
    Sans = TTF_OpenFont("./resource/fonts/font.ttf", 24);
    if(!Sans)
        printf("%s", TTF_GetError());

    surfaceMessage =  TTF_RenderUTF8_Blended_Wrapped(Sans, msg.text, black, 1280-marginX);
    if(!surfaceMessage)
        printf("%s", TTF_GetError()); 
    texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if(!texture)
        printf("%s", TTF_GetError());
    msg.texture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
   
    msg.rect = (SDL_Rect){marginX, 720-marginY, texW - marginX, texH};
    background = (SDL_Rect){marginX, 720-marginY, texW - marginX, texH};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer,255,255 ,255,100);
    SDL_RenderFillRect(renderer,&background);
    SDL_RenderCopy(renderer, msg.texture, NULL, &msg.rect);
}

void load_lvl_0(GameState* game, const char* backgroundFile, const char* musicFile)
{
    SDL_Surface* surface = NULL;



    surface = IMG_Load("./resource/Alice/Alice_stand.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_stand.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_1.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_1.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_2.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_2.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_3.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_3.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_4.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_4.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_J.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_J.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/3-Tile.png");
    game->tiles = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(backgroundFile);
    game->backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    if (surface == NULL)
    {
        printf("Cannot find background image: %s\n\n", backgroundFile);
        SDL_Quit();
        exit(1);
    }

    game->backgroundMusic = Mix_LoadMUS(musicFile);
    if (!game->backgroundMusic)
    {
        printf("Failed to load background music: %s\n", Mix_GetError());
    }
    else
    {
        Mix_PlayMusic(game->backgroundMusic, -1);
    }

    game->alice.x = 320 - 40;
    game->alice.y = 240 - 40;
    game->alice.dx = 0;
    game->alice.dy = 0;
    game->alice.onLedge = 0;
    game->alice.animFrame = 0;
    game->alice.facingLeft = 1;
    game->alice.slowingDown = 0;

    game->time = 0;

    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 250;
        game->ledges[i].h = 78;
        game->ledges[i].x = i * 200;
        game->ledges[i].y = 650;
    }

}

void load_lvl_1(GameState* game, const char* musicFile)
{
    SDL_Surface* surface = NULL;


    surface = IMG_Load("./resource/Alice/Alice_stand.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_stand.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_1.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_1.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_2.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_2.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_3.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_3.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_4.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_4.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/Alice/Alice_J.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_J.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/textures/3-Tile.png");
    game->tiles = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/textures/door.bmp");
    game->doorImage = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/textures/door1.bmp");
    game->doorImageLevel3 = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/items/cake.bmp");
    game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/level1/level1.bmp");
    game->backgroundLeve1Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/level2/level2.bmp");
    game->backgroundLevel2Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/level3/level3.bmp");
    game->backgroundLevel3Texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    if (surface == NULL)
    {
        printf("Cannot find background image:");
        SDL_Quit();
        exit(1);
    }

    game->backgroundMusic = Mix_LoadMUS(musicFile);
    if (!game->backgroundMusic)
    {
        printf("Failed to load background music: %s\n", Mix_GetError());
    }
    else
    {
        Mix_PlayMusic(game->backgroundMusic, -1);
    }

    game->alice.x = 60;
    game->alice.y = 200;
    game->alice.dx = 0;
    game->alice.dy = 0;
    game->alice.onLedge = 0;
    game->alice.animFrame = 0;
    game->alice.facingLeft = 1;
    game->alice.slowingDown = 0;

    game->time = 0;
    game->isEnd = false;

    
    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 256;
        game->ledges[i].h = 64;
        game->ledges[i].x = i * 256;
        game->ledges[i].y = 700;
    }

    game->ledges[99].x = 0;
    game->ledges[99].y = 550;

    game->ledges[98].x = 550;
    game->ledges[98].y = 550;

    game->ledges[97].x = 1050;
    game->ledges[97].y = 250;

    game->ledges[96].x = 300;
    game->ledges[96].y = 400;

    game->ledges[95].x = 0;
    game->ledges[95].y = 250;

    game->ledges[94].x = 800;
    game->ledges[94].y = 400;

    game->door.x = 1150;
    game->door.y = 150;
    game->door.w = 70;
    game->door.h = 100;

    game->collactableCounter.counter = 0;
    game->levelCounter = 1;
    game->collectable.isCollected = false;

    initCollectableAboveLedge(game);
}

void initCollectableAboveLedge(GameState* game)
{
    game->collectable.x = game->ledges[99].x + 256 / 2 - 48 / 2;
    game->collectable.y = game->ledges[99].y - 64 - 20;
    game->collectable.w = 48;
    game->collectable.h = 48;
}

void toggleSound() {
    if (soundOn) {
        Mix_PauseMusic();
    }
    else {
        Mix_ResumeMusic();
    }
    soundOn = !soundOn;
}

void handleEvents(SDL_Renderer* renderer, Button* startButton, Button* exitButton, Button* volumeButton, int* quit, int* start) {

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = 1;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & startButton->rect)) {
                printf("Start button clicked!\n");
                *start = 1;
            }

            if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & exitButton->rect)) {
                printf("Exit button clicked!\n");
                *quit = 1;
            }

            if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & volumeButton->rect)) {
                toggleSound();
                if (soundOn) {
                    volumeButton->texture = loadTexture(renderer, "./resource/buttons/vol1.bmp");
                }
                else {
                    volumeButton->texture = loadTexture(renderer, "./resource/buttons/vol2.bmp");
                }
            }
        }
    }
}

void doRender(SDL_Renderer* renderer, GameState* game, Collectable *letter)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, game->backgroundTexture, NULL, NULL);

    for (int i = 0; i < 100; i++)
    {
        SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
        SDL_RenderCopy(renderer, game->tiles, NULL, &ledgeRect);
    }

    SDL_Rect rect = { game->alice.x, game->alice.y, 42, 74 };
    SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
        NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));

    renderCollectable(letter, renderer, game, type_letter);
    if(msg.isOn)
        DisplayLetterText(renderer);
   
    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window* window, GameState* game)
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;

        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;
            case SDLK_UP:
                if (game->alice.onLedge)
                {
                    game->alice.dy = -8;
                    game->alice.onLedge = 0;
                }
                break;
            }
        }
        break;
        case SDL_QUIT:
            done = 1;
            break;
        }
    }

    //jumping
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        game->alice.dy -= 0.2f;
    }

    //Walking
    if (state[SDL_SCANCODE_LEFT])
    {
        game->alice.dx -= 0.5;
        if (game->alice.x < 0) {
            game->alice.x = 0;
        }
        if (game->alice.dx < -6)
        {
            game->alice.dx = -6;
        }
        game->alice.facingLeft = 1;
        game->alice.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        game->alice.dx += 0.5;
        if (game->alice.x + 32 > WINDOW_W) {
            game->alice.x = WINDOW_W - 32;
        }
        if (game->alice.dx > 6)
        {
            game->alice.dx = 6;
        }
        game->alice.facingLeft = 0;
        game->alice.slowingDown = 0;
    }
    else
    {
        game->alice.animFrame = 0;
        game->alice.dx *= 0.8f;
        game->alice.slowingDown = 1;
        if (fabsf(game->alice.dx) < 0.1f)
        {
            game->alice.dx = 0;
        }
    }

    if (game->alice.x >= 1200)
    {
        done = 1;
    }

    return done;
}

void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Button* startButton, Button* exitButton, Button* volumeButton) {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, startButton->texture, NULL, &startButton->rect);
    SDL_RenderCopy(renderer, exitButton->texture, NULL, &exitButton->rect);
    SDL_RenderCopy(renderer, volumeButton->texture, NULL, &volumeButton->rect);

    SDL_RenderPresent(renderer);
}

void collisionDetect(GameState* game)
{

    for (int i = 0; i < 100; i++)
    {
        float mw = 42, mh = 74;
        float mx = game->alice.x, my = game->alice.y;
        float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

        if (game->alice.x < 0) {
            game->alice.x = 0;
        }
        if (game->alice.x + 32 > WINDOW_W) {
            game->alice.x = WINDOW_W - 32;
        }
        if (game->alice.y < 0) {
            game->alice.y = 0;
        }
        if (game->alice.y + 64 > WINDOW_H) {
            game->alice.y = WINDOW_H - 64;
            game->alice.dy = 0;
            game->alice.onLedge = 1;
        }

        if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
        {
            //are we bumping our head?
            if (my < by + bh && my > by && game->alice.dy < 0)
            {
                game->alice.y = by + bh;
                my = by + bh;

                //bumped our head, stop any jump velocity
                game->alice.dy = 0;
                game->alice.onLedge = 1;
            }
        }
        if (mx + mw > bx && mx < bx + bw)
        {
            //are we landing on the ledge
            if (my + mh > by && my < by && game->alice.dy > 0)
            {
                game->alice.y = by - mh;
                my = by - mh;

                //landed on this ledge, stop any jump velocity
                game->alice.dy = 0;
                game->alice.onLedge = 1;
            }
        }


        if (my + mh > by && my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw && mx + mw > bx + bw && game->alice.dx < 0)
            {
                //correct x
                game->alice.x = bx + bw;
                mx = bx + bw;

                game->alice.dx = 0;
            }
            //rubbing against left edge
            else if (mx + mw > bx && mx < bx && game->alice.dx > 0)
            {
                //correct x
                game->alice.x = bx - mw;
                mx = bx - mw;

                game->alice.dx = 0;
            }
        }
    }
}

void process(GameState* game)
{
   
    game->time++;

    object* alice = &game->alice;
    alice->x += alice->dx;
    alice->y += alice->dy;

    if (alice->dx != 0 && alice->onLedge && !alice->slowingDown)
    {
        if (game->time % 8 == 0)
        {
            if (alice->animFrame != 4 && alice->animFrame != 5)
            {
                alice->animFrame++;
            }
            else
            {
                alice->animFrame = 1;
            }
        }
    }
    else if (!alice->onLedge)
    {
        alice->animFrame = 5;
    }
    else
    {
        alice->animFrame = 0;
    }



    alice->dy += GRAVITY;
}

void diana_process(GameState* game)
{

    game->time++;

    object* alice = &game->alice;
    alice->x += alice->dx;
    alice->y += alice->dy;

    if (alice->dx != 0 && alice->onLedge && !alice->slowingDown)
    {
        if (game->time % 8 == 0)
        {
            if (alice->animFrame != 4 && alice->animFrame != 5)
            {
                alice->animFrame++;
            }
            else
            {
                alice->animFrame = 1;
            }
        }
    }
    else if (!alice->onLedge)
    {
        alice->animFrame = 5;
    }
    else
    {
        alice->animFrame = 0;
    }

    if (isAliceOnTheFloor(alice))
    {
        teleportAliceToStartPosition(alice);
    }

    Collectable* collectable = &game->collectable;
    if (isAliceRichedCollectable(alice, collectable))
    {
        if (collectable->isCollected == false)
        {
            game->collactableCounter.counter++;
            SetCounterText(game);

        }

        collectable->isCollected = true;
        collectable->x = 0;
        collectable->y = 0;

    }

    if (isAliceRichedTheDoor(&game->alice, &game->door))
    {
        game->collectable.isCollected = false;
        game->levelCounter++;
        teleportAliceToStartPosition(alice);
    }

    if (game->levelCounter == 2)
    {
        clearLedges(game);
        initLevel2Ledges(game);
        initLevel2Collectable(game);
        initLevel2Door(game);
    }
    else if (game->levelCounter == 3)
    {
        clearLedges(game);
        initLevel3Ledges(game);
        game->collectable.x = game->ledges[96].x + game->ledges[96].w / 2 - 48 / 2;
        game->collectable.y = game->ledges[96].y - game->ledges[96].h - 20;
        game->collectable.w = 32;
        game->collectable.h = 32;
        game->door.x = game->ledges[89].x + game->ledges[89].w / 3 - 48 / 2;
        game->door.y = game->ledges[89].y - game->ledges[89].h - 30;
        game->door.w = 50;
        game->door.h = 100;
        SDL_Surface* surface = IMG_Load("./resource/items/tea.bmp");
        game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);

    }
    else if (game->levelCounter == 4)
    {
        game->isEnd = true;
    }
    alice->dy += GRAVITY;

}

void DisplayEndPicture(SDL_Renderer* renderer, char* path, GameState* game) {

    SDL_Surface* surface;
    surface = IMG_Load(path);
    if (!surface) {
        printf("Error loading image: %s\n", IMG_GetError());
        return;
    }
    game->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (SDL_GetError() && SDL_GetError()[0] != '\0') {
        printf("SDL Render Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);

    //SDL_Rect endimg = { 0, 0, 1280, 720 };
    SDL_RenderCopy(renderer, game->texture, NULL, NULL);
}
bool isAliceRichedThefinalDoor(object* alice, Door* door)
{
    return collide2d(
        alice->x,
        alice->y,
        door->x,
        door->y,
        alice->dx,
        alice->dy,
        door->w,
        door->h);
}

bool isAliceRichedTheDoor1(object* alice, Door* door1)
{
    return collide2d(
        alice->x,
        alice->y,
        door1->x,
        door1->y,
        alice->dx,
        alice->dy,
        door1->w,
        door1->h);
}

void diana_doRender(SDL_Renderer* renderer, GameState* game)
{
    
    SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);

    SDL_RenderClear(renderer);

    if (game->isEnd == false){
        if (game->levelCounter == 2)
            SDL_RenderCopy(renderer, game->backgroundLevel2Texture, NULL, NULL);
        else if (game->levelCounter == 3)
            SDL_RenderCopy(renderer, game->backgroundLevel3Texture, NULL, NULL);
        else if (game->levelCounter == 1)
            SDL_RenderCopy(renderer, game->backgroundLeve1Texture, NULL, NULL);
        else if (game->levelCounter > 4)
            game->isEnd = true; 

        for (int i = 0; i < 100; i++)
        {
            SDL_Rect ledgeRect = { game->scrollX + game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
            SDL_RenderCopy(renderer, game->tiles, NULL, &ledgeRect);
        }

        SDL_Rect DoorRect = { game->door.x, game->door.y, game->door.w, game->door.h };
        //SDL_Rect Door1Rect = { game->door1.x, game->door1.y, game->door1.w, game->door1.h };
        if (game->collactableCounter.counter < 3 && game->levelCounter == 3)
            SDL_RenderCopy(renderer, game->doorImageLevel3, NULL, &DoorRect);
        else
            SDL_RenderCopy(renderer, game->doorImage, NULL, &DoorRect);

        SDL_Rect rect = { game->scrollX + game->alice.x, game->alice.y, 32, 64 };
        SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
            NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));

        renderCallectable(renderer, game);
        if (game->collactableCounter.counter > 0)
            displayText(game);
    }

    if (game->isEnd)
    {
        if (game->collactableCounter.counter == 3) {
            DisplayEndPicture(renderer, "./resource/end/good_end.bmp", game);
        }
        else {
            DisplayEndPicture(renderer, "./resource/end/bad_end.bmp", game);
        }
    }
    SDL_RenderPresent(renderer);
}

bool isAliceOnTheFloor(object* alice)
{
    return alice->y >= 700 - 64;
}

void teleportAliceToStartPosition(object* alice)
{
    alice->x = 60;
    alice->y = 200;
}

int isAliceRichedCollectable(object* alice, Collectable* collectable)
{
    return collide2d(
        alice->x,
        alice->y,
        collectable->x,
        collectable->y,
        alice->dx,
        alice->dy,
        collectable->w,
        collectable->h);
}



bool isAliceRichedTheDoor(object* alice, Door* door)
{
    return collide2d(
        alice->x,
        alice->y,
        door->x,
        door->y,
        alice->dx,
        alice->dy,
        door->w,
        door->h);
}

void clearLedges(GameState* game)
{
    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 0;
        game->ledges[i].h = 0;
        game->ledges[i].x = 0;
        game->ledges[i].y = 0;
    }
}

void initLevel2Door(GameState* game)
{
    game->door.x = 1150;
    game->door.y = 250;
}

void initLevel2Ledges(GameState* game)
{
    game->ledges[0].x = 180;
    game->ledges[0].y = 700;
    game->ledges[0].w = 300;
    game->ledges[0].h = 50;

    game->ledges[1].x = 0;
    game->ledges[1].y = 600;
    game->ledges[1].w = 210;
    game->ledges[1].h = 150;

    game->ledges[2].x = 250;
    game->ledges[2].y = 470;
    game->ledges[2].w = 150;
    game->ledges[2].h = 35;

    game->ledges[3].x = 400;
    game->ledges[3].y = 350;
    game->ledges[3].w = 150;
    game->ledges[3].h = 35;

    game->ledges[4].x = 550;
    game->ledges[4].y = 250;
    game->ledges[4].w = 150;
    game->ledges[4].h = 35;

    game->ledges[5].x = 1000;
    game->ledges[5].y = 450;
    game->ledges[5].w = 50;
    game->ledges[5].h = 35;

    game->ledges[6].x = 0;
    game->ledges[6].y = 150;
    game->ledges[6].w = 150;
    game->ledges[6].h = 35;

    game->ledges[7].x = 170;
    game->ledges[7].y = 250;
    game->ledges[7].w = 150;
    game->ledges[7].h = 35;

    game->ledges[8].x = 1120;
    game->ledges[8].y = 350;
    game->ledges[8].w = 160;
    game->ledges[8].h = 35;

    game->ledges[9].x = 450;
    game->ledges[9].y = 700;
    game->ledges[9].w = 300;
    game->ledges[9].h = 50;

    game->ledges[10].x = 650;
    game->ledges[10].y = 700;
    game->ledges[10].w = 300;
    game->ledges[10].h = 50;

    game->ledges[11].x = 850;
    game->ledges[11].y = 700;
    game->ledges[11].w = 300;
    game->ledges[11].h = 50;

    game->ledges[12].x = 1000;
    game->ledges[12].y = 700;
    game->ledges[12].w = 300;
    game->ledges[12].h = 50;

    game->ledges[13].x = 850;
    game->ledges[13].y = 150;
    game->ledges[13].w = 50;
    game->ledges[13].h = 35;

    game->ledges[14].x = 800;
    game->ledges[14].y = 570;
    game->ledges[14].w = 150;
    game->ledges[14].h = 35;
}

void initLevel3Ledges(GameState* game)
{
    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 256;
        game->ledges[i].h = 70;
        game->ledges[i].x = i * 256;
        game->ledges[i].y = 700;


        int baseX = 1;
        int baseY = 660;
        int xDiff = 256;
        int yDiff = 108;

        game->ledges[99].x = baseX;
        game->ledges[99].y = baseY;
        game->ledges[99].h = 60;

        game->ledges[98].x = baseX + xDiff;
        game->ledges[98].y = baseY - yDiff;
        game->ledges[98].w = 208;

        game->ledges[97].x = baseX + 2 * xDiff;
        game->ledges[97].y = baseY - 2 * yDiff;
        game->ledges[97].w = 208;

        //cookie
        game->ledges[86].x = baseX + 3.5 * xDiff;
        game->ledges[86].y = baseY - 2.1 * yDiff;
        game->ledges[86].w = 148;

        game->ledges[87].x = baseX + 3.5 * xDiff;
        game->ledges[87].y = baseY - 2.3 * yDiff;
        game->ledges[87].w = 148;

        game->ledges[96].x = baseX + 3.5 * xDiff;
        game->ledges[96].y = baseY - 2.5 * yDiff;
        game->ledges[96].w = 148;

        //1 way
        game->ledges[95].x = baseX + 4.5 * xDiff;
        game->ledges[95].y = baseY - yDiff;
        game->ledges[95].w = 208;

        game->ledges[94].x = baseX + 6 * xDiff;
        game->ledges[94].y = baseY - 2 * yDiff;
        game->ledges[94].w = 148;

        game->ledges[93].x = baseX + 5 * xDiff;
        game->ledges[93].y = baseY - 3.5 * yDiff;
        game->ledges[93].w = 148;

        //top 
        game->ledges[92].x = baseX + 3.4 * xDiff;
        game->ledges[92].y = baseY - 4.5 * yDiff;
        game->ledges[92].w = 180;

        game->ledges[91].x = baseX + 3.1 * xDiff;
        game->ledges[91].y = game->ledges[92].y - game->ledges[91].h;
        game->ledges[91].w = 128;

        //2 way
        game->ledges[90].x = baseX + 1.3 * xDiff;
        game->ledges[90].y = baseY - 3.2 * yDiff;
        game->ledges[90].w = 128;
        game->ledges[90].h = 56;

        //top 
        game->ledges[89].x = baseX + 32;
        game->ledges[89].y = baseY - 4.5 * yDiff;
        game->ledges[89].w = 180;


        game->ledges[88].x = baseX - 64;
        game->ledges[88].y = game->ledges[89].y - game->ledges[88].h - 16;
        game->ledges[88].w = 128;
    }
}

void initLevel2Collectable(GameState* game)
{
        game->collectable.x = 850;
        game->collectable.y = 75;
        game->collectable.w = 32;
        game->collectable.h = 32;
        SDL_Surface* surface = SDL_LoadBMP("./resource/items/cookie1.bmp");
        game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
}

void renderCallectable(SDL_Renderer* renderer, GameState* game)
{
    if (game->collectable.isCollected)
    {
        SDL_RenderCopy(renderer, game->collactbleImage, NULL, &(SDL_Rect){0, 0, 0, 0});
    }
    else
    {
        SDL_Rect collectableRect = { game->collectable.x, game->collectable.y, game->collectable.w, game->collectable.h };
        SDL_RenderCopy(renderer, game->collactbleImage, NULL, &collectableRect);
    }
}

void renderCounterText(int counter, SDL_Renderer* renderer)
{
    char text[20];
    char counterText[2];
    SDL_Color white = { 255, 255, 255,255 };
    TTF_Font* Sans;
    SDL_Surface* surfaceMessage;
    SDL_Texture* Message;

    sprintf(counterText, "%d", counter);
    sprintf(text, "%s %s", "items collected: ", counterText);
    Sans = TTF_OpenFont("./resource/fonts/font.ttf", 12);
    if (!Sans)
        printf("%s", TTF_GetError());
    surfaceMessage = TTF_RenderText_Solid(Sans, text, white);
    if (!surfaceMessage)
        printf("%s", TTF_GetError());
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (!Message)
        printf("%s", TTF_GetError());

    SDL_RenderCopy(renderer, Message, NULL, &(SDL_Rect){0, 0, 200, 50});
}


int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
    return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

void diana_collisionDetect(GameState* game)
{
    for (int i = 0; i < 100; i++)
    {
        float mw = 42, mh = 74;
        float mx = game->alice.x, my = game->alice.y;
        float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

        if (game->alice.x < 0) {
            game->alice.x = 0;
        }
        if (game->alice.x + 32 > 1280) {
            game->alice.x = 1280 - 32;
        }
        if (game->alice.y < 0) {
            game->alice.y = 0;
        }
        if (game->alice.y + 64 > 720) {
            game->alice.y = 720 - 64;
            game->alice.dy = 0;
            game->alice.onLedge = 1;
        }

        if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
        {
            //are we bumping our head?
            if (my < by + bh && my > by && game->alice.dy < 0)
            {
                //correct y
                game->alice.y = by + bh;
                my = by + bh;

                //bumped our head, stop any jump velocity
                game->alice.dy = 0;
                game->alice.onLedge = 1;
            }
        }
        if (mx + mw > bx && mx < bx + bw)
        {
            //are we landing on the ledge
            if (my + mh > by && my < by && game->alice.dy > 0)
            {
                //correct y
                game->alice.y = by - mh;
                my = by - mh;

                //landed on this ledge, stop any jump velocity
                game->alice.dy = 0;
                game->alice.onLedge = 1;
            }
        }


        if (my + mh > by && my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw && mx + mw > bx + bw && game->alice.dx < 0)
            {
                //correct x
                game->alice.x = bx + bw;
                mx = bx + bw;

                game->alice.dx = 0;
            }
            //rubbing against left edge
            else if (mx + mw > bx && mx < bx && game->alice.dx > 0)
            {
                //correct x
                game->alice.x = bx - mw;
                mx = bx - mw;

                game->alice.dx = 0;
            }
        }
    }
}

int diana_processEvents(SDL_Window* window, GameState* game)
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;
            case SDLK_UP:
                if (game->alice.onLedge)
                {
                    game->alice.dy = -8;
                    game->alice.onLedge = 0;
                }
                break;
            }
        }
        break;
        case SDL_QUIT:
            done = 1;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        game->alice.dy -= 0.2f;
    }

    if (state[SDL_SCANCODE_LEFT])
    {
        game->alice.dx -= 0.5;
        if (game->alice.dx < -6)
        {
            game->alice.dx = -6;
        }
        game->alice.facingLeft = 1;
        game->alice.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        game->alice.dx += 0.5;
        if (game->alice.dx > 6)
        {
            game->alice.dx = 6;
        }
        game->alice.facingLeft = 0;
        game->alice.slowingDown = 0;
    }
    else
    {
        game->alice.animFrame = 0;
        game->alice.dx *= 0.8f;
        game->alice.slowingDown = 1;
        if (fabsf(game->alice.dx) < 0.1f)
        {
            game->alice.dx = 0;
        }
    }

    return done;
}

void cleanup(SDL_Texture* startButtonTexture, SDL_Texture* exitButtonTexture, SDL_Texture* backgroundTexture, SDL_Texture* volumeOn, SDL_Texture* volumeOff, SDL_Renderer* renderer, SDL_Window* window) {
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

