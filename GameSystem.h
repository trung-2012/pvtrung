#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

struct GameContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    Mix_Music* backgroundMusic;
    Mix_Chunk* shootSound;
    Mix_Chunk* explosionSound;

    SDL_Texture* planeTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* planeEnemyTexture;
    SDL_Texture* bulletEnemyTexture;
    SDL_Texture* imageSpace;
    SDL_Texture* starBackground;
};

bool initGame(GameContext& ctx);
void cleanUp(GameContext& ctx);

