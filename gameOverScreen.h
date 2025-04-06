// gameOverScreen.h
#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "SDLUtils.h"
#include "diem.h"

extern Mix_Music* backgroundMusic;
extern Mix_Chunk* shootSound;
extern Mix_Chunk* explosionSound;


void showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score,
                        SDL_Texture* planeTexture, SDL_Texture* bulletTexture,
                        SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture,
                        SDL_Texture* starBackground,
                        SDL_Window* window, int argc, char* argv[]);

#endif


