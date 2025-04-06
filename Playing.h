#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

bool GamePlay(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* starBackground, SDL_Texture* imageSpace,
              SDL_Texture* planeTexture, SDL_Texture* bulletTexture, SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture,
              Mix_Music* backgroundMusic, Mix_Chunk* shootSound, Mix_Chunk* explosionSound, int& score);
