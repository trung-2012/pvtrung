#include "gameOverScreen.h"
#include "SDLUtils.h"

int GameOver(SDL_Renderer* renderer, TTF_Font* font, int score, SDL_Window* window) {
    return showGameOverScreen(renderer, font, score,
                              starBackground, buttonRestartTexture, buttonMenuTexture, window);
}
