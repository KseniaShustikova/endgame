#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_mixer.h>


#define type_collectable 1
#define type_letter 2

#define WINDOW_W 1280
#define WINDOW_H 720

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define GRAVITY 0.35f;
const int LEDGE_HEIGHT;
const int LEDGE_WIDTH;
const int COLLACTABLE_SIZE;

typedef struct counter{
    int counter;
    SDL_Texture *texture;
    SDL_Rect rect;
    
} counter;

typedef struct message{
    char *text;
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isOn;
} message;

typedef struct collectable {
    SDL_Rect rect;
    SDL_Texture *image;
    bool isCollected;
}Collectable;


typedef struct {
	SDL_Texture* texture;
	SDL_Rect rect;
} Button;

Mix_Music* backgroundMusic;
int soundOn;

typedef struct
{
	float x, y;
	float dx, dy;
	int onLedge;

	int animFrame, facingLeft, slowingDown;
} object;

typedef struct
{
	int x, y, w, h;
} Ledge;


typedef struct
{
	object alice;
	Button startButton, exitButton, volumeButton;
	Ledge ledges[100];

	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;
	SDL_Texture* collactbleImage;
	SDL_Texture* backgroundTexture;
    Mix_Music* backgroundMusic;

	int time;

	SDL_Renderer* renderer;
} GameState;

counter collectableCounter;
message msg;

Collectable makeCollectable ( int x, int y, int width, int height);
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path);
void SetCounterText(counter  *counter, SDL_Renderer *renderer );
void renderCollectable (Collectable *element, SDL_Renderer *renderer, GameState *game,  int type);
void displayText (SDL_Renderer *renderer, counter *counter);
void DisplayLetterText(SDL_Renderer *renderer );
void DisplayEndPicture(SDL_Renderer *renderer, char *path);

void initCollectableAboveLedge(GameState* game);
void loadGame(GameState* game, const char *gameBackground, const char *gameMusic);
void renderCounterText(int counter, SDL_Renderer* renderer);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);
int processEvents(SDL_Window* wind, GameState* game);
void process(GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game, Collectable *letter);
void collisionDetect(GameState* game);
void renderCallectable(SDL_Renderer* renderer, GameState* game);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
void toggleSound();
void handleEvents(SDL_Renderer* renderer, Button* startButton, Button* exitButton, Button* volumeButton, int* quit, int *start);
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Button* startButton, Button* exitButton, Button* volumeButton);
void cleanup(SDL_Texture* startButtonTexture, SDL_Texture* exitButtonTexture, SDL_Texture* backgroundTexture, SDL_Texture* volumeOn, SDL_Texture* volumeOff, SDL_Renderer* renderer, SDL_Window* window);

#endif
