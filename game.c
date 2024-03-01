#include "./header.h"
#include "./collectable.h"

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

void loadGame(GameState* game, const char* backgroundFile, const char* musicFile)
{
    SDL_Surface* surface = NULL;

    //Load images and create rendering textures from them

    surface = IMG_Load("./resourse/Alice/Alice_stand.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_stand.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/Alice/Alice_1.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_1.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/Alice/Alice_2.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_2.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/Alice//Alice_3.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_3.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/Alice//Alice_4.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_4.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/Alice//Alice_J.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_J.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resourse/textures/3-Tile.png");
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

    //init ledges
    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 250;
        game->ledges[i].h = 78;
        game->ledges[i].x = i * 200;
        game->ledges[i].y = 650;
    }
   
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
                    volumeButton->texture = loadTexture(renderer, "./resourse/buttons/vol1.bmp");
                }
                else {
                    volumeButton->texture = loadTexture(renderer, "./resourse/buttons/vol2.bmp");
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

    //draw a rectangle at man's position
    SDL_Rect rect = { game->alice.x, game->alice.y, 42, 74 };
    SDL_RenderCopyEx(renderer, game->aliceFrames[game->alice.animFrame],
        NULL, &rect, 0, NULL, (game->alice.facingLeft == 0));
    renderCollectable(letter, renderer, game, type_letter);
    if(msg.isOn)
        DisplayLetterText(renderer);
    //We are done drawing, "present" or show to the screen what we've drawn
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
        float mw = 32, mh = 64;
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
    //add time
    game->time++;

    //man movement
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
