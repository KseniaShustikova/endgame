#pragma once

#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#pragma warning(disable : 4996)

#define type_collectable 1
#define type_letter 2

#define WINDOW_W 1280
#define WINDOW_H 720

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define GRAVITY 0.4f;

const int COLLACTABLE_SIZE;

typedef struct {
	SDL_Texture* texture;
	SDL_Rect rect;
} Button;

typedef struct counter{
    int counter;
    SDL_Texture *texture;
    SDL_Rect rect;
    
} counter;

Mix_Music* backgroundMusic;
int soundOn;

typedef struct
{
	float x, y;
	float dx, dy;
	int onLedge;

	int animFrame, facingLeft, slowingDown;
} object;

typedef struct Door
{
	int x, y, w, h;
} Door;

typedef struct Collectable
{
	SDL_Rect rect;
    SDL_Texture *image;
	int x, y, w, h;
	bool isCollected;
} Collectable;

typedef struct message{
    char *text;
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isOn;
} message;

typedef struct
{
	int x, y, w, h;
} Ledge;

typedef struct
{
	object alice;
	Button startButton, exitButton, volumeButton;
	Ledge ledges[100];
	Door door1;
	Door door;
	float scrollX;

	Collectable collectable;
	int collactableCounter;
	int levelCounter;

	Mix_Music* backgroundMusic;

	SDL_Texture* aliceFrames[6];
	SDL_Texture* tiles;
	SDL_Texture* doorImage;
	SDL_Texture* doorImageLevel3;
	SDL_Texture* collactbleImage;
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundLeve1Texture;
	SDL_Texture* backgroundLevel2Texture;
	SDL_Texture* backgroundLevel3Texture;
	SDL_Texture* texture; //end

	int time;

	SDL_Renderer* renderer;
	bool isEnd;
} GameState;

counter collectableCounter;
message msg;

int isAliceRichedCollectable(object* alice, Collectable* collectable);
bool isAliceRichedTheDoor(object* alice, Door* door);
bool isAliceRichedTheDoor1(object* alice, Door* door1);

void clearLedges(GameState* game);
void initLevel2Door(GameState* game);
void initLevel1Collectable(GameState* game);
void initLevel2Collectable(GameState* game);
void initLevel3Collectable(GameState* game);
void initLevel3Ledges(GameState* game);
void initLevel2Ledges(GameState* game);
void renderCallectable(SDL_Renderer* renderer, GameState* game);
void renderCounterText(int counter, SDL_Renderer* renderer);
void DisplayEndPicture(SDL_Renderer* renderer, char* path, GameState* game);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);

void teleportAliceToStartPosition(object* alice);
bool isAliceOnTheFloor(object* alice);
void diana_doRender(SDL_Renderer* renderer, GameState* game);
void diana_process(GameState* game);
int diana_processEvents(SDL_Window* window, GameState* game);

void displayText (SDL_Renderer *renderer, counter *counter);
void renderCollectable (Collectable *element, SDL_Renderer *renderer, GameState* game, int type);
void DisplayLetterText(SDL_Renderer *renderer);
void SetCollectableImage(Collectable *element, SDL_Renderer *renderer, char* path);
void SetCounterText(counter  *counter, SDL_Renderer *renderer );

Collectable makeCollectable ( int x, int y, int width, int height);
void initCollectableAboveLedge(GameState* game);
int processEvents(SDL_Window* wind, GameState* game);
void doRender(SDL_Renderer* renderer, GameState* game, Collectable *letter);
void collisionDetect(GameState* game);
void process(GameState* game);
void load_lvl_0(GameState* game, const char* gameBackground, const char* gameMusic);
void load_lvl_1(GameState* game, const char* musicFile);
void diana_collisionDetect(GameState* game);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
void toggleSound();
void handleEvents(SDL_Renderer* renderer, Button* startButton, Button* exitButton, Button* volumeButton, int* quit, int* start);
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Button* startButton, Button* exitButton, Button* volumeButton);
void cleanup(SDL_Texture* startButtonTexture, SDL_Texture* exitButtonTexture, SDL_Texture* backgroundTexture, SDL_Texture* volumeOn, SDL_Texture* volumeOff, SDL_Renderer* renderer, SDL_Window* window);



#endif
