#include "GameOver.h"
#include "gameOverScreen.h"
#include <SDL.h>
#include "SDLUtils.h"

bool GameOver(SDL_Renderer* renderer, TTF_Font* font, int score, SDL_Window* window) {
    SDL_Delay(500);
    showGameOverScreen(renderer, font, score, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground, window, 0, nullptr);
    return false;
}

