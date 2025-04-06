#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

bool GameOver(SDL_Renderer* renderer, TTF_Font* font, int score,
              SDL_Texture* planeTexture, SDL_Texture* bulletTexture,
              SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture,
              SDL_Texture* starBackground, SDL_Window* window);

