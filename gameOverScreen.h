#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

enum GameOverAction {
    GO_NONE = 0,
    GO_RESTART = 1,
    GO_MENU = 2
};

int showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score,
                       SDL_Texture* starBackground, SDL_Texture* buttonRestartTexture,
                       SDL_Texture* buttonMenuTexture, SDL_Window* window);
