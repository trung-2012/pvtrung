
// Game.cpp
#include "Game.h"

// Hàm khởi tạo
GameResources initializeGame() {
    GameResources resources;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        throw runtime_error("Failed to initialize SDL");
    }

    resources.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!resources.window) {
        throw runtime_error("Failed to create window");
    }

    resources.renderer = SDL_CreateRenderer(resources.window, -1, SDL_RENDERER_ACCELERATED);
    if (!resources.renderer) {
        throw runtime_error("Failed to create renderer");
    }

    if (TTF_Init() == -1) {
        throw runtime_error("Failed to initialize TTF");
    }
    resources.font = TTF_OpenFont("font2.ttf", 80);
    if (!resources.font) {
        throw runtime_error("Failed to load font");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw runtime_error("Failed to initialize audio");
    }

    resources.planeTexture = loadTexture("ok.png", resources.renderer);
    resources.bulletTexture = loadTexture("bulletpro.png", resources.renderer);
    resources.planeEnemyTexture = loadTexture("ufo.png", resources.renderer);
    resources.bulletEnemyTexture = loadTexture("dan.png", resources.renderer);
    resources.imageSpace = loadTexture("space.png", resources.renderer);
    resources.starBackground = createStarBackground(resources.renderer);

    resources.backgroundMusic = Mix_LoadMUS("background.mp3");
    resources.shootSound = Mix_LoadWAV("amthanh1.wav");
    resources.explosionSound = Mix_LoadWAV("explosion.wav");

    return resources;
}

// Hàm giải phóng tài nguyên
void quitSDL(GameResources& resources) {
    SDL_DestroyTexture(resources.planeTexture);
    SDL_DestroyTexture(resources.bulletTexture);
    SDL_DestroyTexture(resources.planeEnemyTexture);
    SDL_DestroyTexture(resources.bulletEnemyTexture);
    SDL_DestroyTexture(resources.imageSpace);
    SDL_DestroyTexture(resources.starBackground);
    Mix_FreeMusic(resources.backgroundMusic);
    Mix_FreeChunk(resources.shootSound);
    Mix_FreeChunk(resources.explosionSound);
    TTF_CloseFont(resources.font);
    SDL_DestroyRenderer(resources.renderer);
    SDL_DestroyWindow(resources.window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
