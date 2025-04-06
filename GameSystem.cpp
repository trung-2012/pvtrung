#include "GameSystem.h"
#include "SDLUtils.h" // Chứa loadTexture, createStarBackground, etc.
#include <iostream>

bool initGame(GameContext& ctx) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (TTF_Init() == -1) return false;
    ctx.font = TTF_OpenFont("font2.ttf", 80);
    if (!ctx.font) return false;

    ctx.window = SDL_CreateWindow("Máy Bay - Nền Cuộn", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 750, SDL_WINDOW_SHOWN);
    ctx.renderer = SDL_CreateRenderer(ctx.window, -1, SDL_RENDERER_ACCELERATED);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    ctx.backgroundMusic = Mix_LoadMUS("background.mp3");
    ctx.shootSound = Mix_LoadWAV("amthanh1.wav");
    ctx.explosionSound = Mix_LoadWAV("explosion.wav");

    ctx.planeTexture = loadTexture("ok.png", ctx.renderer);
    ctx.bulletTexture = loadTexture("bulletpro.png", ctx.renderer);
    ctx.planeEnemyTexture = loadTexture("ufo.png", ctx.renderer);
    ctx.bulletEnemyTexture = loadTexture("dan.png", ctx.renderer);
    ctx.imageSpace = loadTexture("space.png", ctx.renderer);
    ctx.starBackground = createStarBackground(ctx.renderer);

    return true;
}

void cleanUp(GameContext& ctx) {
    SDL_DestroyTexture(ctx.planeTexture);
    SDL_DestroyTexture(ctx.bulletTexture);
    SDL_DestroyTexture(ctx.planeEnemyTexture);
    SDL_DestroyTexture(ctx.bulletEnemyTexture);
    SDL_DestroyTexture(ctx.imageSpace);
    SDL_DestroyTexture(ctx.starBackground);

    Mix_FreeMusic(ctx.backgroundMusic);
    Mix_FreeChunk(ctx.shootSound);
    Mix_FreeChunk(ctx.explosionSound);
    Mix_CloseAudio();

    TTF_CloseFont(ctx.font);
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    TTF_Quit();
    SDL_Quit();
}

