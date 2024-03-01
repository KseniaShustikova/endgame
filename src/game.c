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
const int LEDGE_HEIGHT = 78;
const int LEDGE_WIDTH = 250;
const int COLLACTABLEgcc = 48;

void loadGame(GameState* game, const char* backgroundFile, const char* musicFile)
{
    SDL_Surface* surface = NULL;

    //Load images and create rendering textures from them

    surface = IMG_Load("../resourse/Alice/Alice_stand.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_stand.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/Alice/Alice_1.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_1.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/Alice/Alice_2.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_2.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/Alice//Alice_3.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_3.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/Alice//Alice_4.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_4.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/Alice//Alice_J.png");
    if (surface == NULL)
    {
        printf("Cannot find Alice_J.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->aliceFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/textures/3-Tile.png");
    game->tiles = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(backgroundFile);
    game->backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("../resourse/items/obj.letter.png");
	game->collactbleImage = SDL_CreateTextureFromSurface(game->renderer, surface);
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

    game->counter = 0;
	game->collectable.isCollected = false;

	initCollectableAboveLedge(game);
   
}

void initCollectableAboveLedge(GameState* game)
{
	game->collectable.x = game->ledges[99].x + LEDGE_WIDTH / 2 - COLLACTABLE_SIZE / 2;
	game->collectable.y = game->ledges[99].y - LEDGE_HEIGHT - 20;
	game->collectable.w = COLLACTABLE_SIZE;
	game->collectable.h = COLLACTABLE_SIZE;
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
                    volumeButton->texture = loadTexture(renderer, "../resourse/buttons/vol1.bmp");
                }
                else {
                    volumeButton->texture = loadTexture(renderer, "../resourse/buttons/vol2.bmp");
                }
            }
        }
    }
}

void doRender(SDL_Renderer* renderer, GameState* game)
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

    renderCallectable(renderer, game);
    renderCounterText(game->counter, renderer);
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
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

// to set the text on top of the counter
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
	Sans = TTF_OpenFont("../resourse/fonts/font.ttf", 12);
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
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
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

    Collectable* collectable = &game->collectable;
	if (collide2d(
		alice->x,
		alice->y,
		collectable->x,
		collectable->y,
		alice->dx,
		alice->dy,
		collectable->w,
		collectable->h))
	{
		collectable->isCollected = true;
		collectable->x = 0;
		collectable->y = 0;
		game->counter++;
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
