#include "SDLUtils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL.h>


using namespace std;

// Biến toàn cục
SDL_Texture* planeTexture = nullptr;
SDL_Texture* bulletTexture = nullptr;
SDL_Texture* planeEnemyTexture = nullptr;
SDL_Texture* bulletEnemyTexture = nullptr;
SDL_Texture* imageSpace = nullptr;
SDL_Texture* starBackground = nullptr;

Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* shootSound = nullptr;
Mix_Chunk* explosionSound = nullptr;
TTF_Font* font = nullptr;


// Hàm báo lỗi
void logErrorAndExit(const char* msg, const char* error) {
    SDL_Log("%s: %s", msg, error);
    IMG_Quit(); // Đảm bảo SDL_image được thoát
    SDL_Quit();
    exit(1);
}


// Khởi tạo SDL
SDL_Window* initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) logErrorAndExit("SDL_Init", SDL_GetError());
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) logErrorAndExit("IMG_Init", IMG_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) logErrorAndExit("CreateWindow", SDL_GetError());

    return window;
}

// Tạo renderer
SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) logErrorAndExit("CreateRenderer", SDL_GetError());

    return renderer;
}

// Nạp hình ảnh vào texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }
    return texture;

}

void loadAssets(SDL_Renderer* renderer) {
    planeTexture = loadTexture("ok.png", renderer);
    bulletTexture = loadTexture("bulletpro.png", renderer);
    planeEnemyTexture = loadTexture("ufo.png", renderer);
    bulletEnemyTexture = loadTexture("dan.png", renderer);
    imageSpace = loadTexture("space.png", renderer);
    starBackground = createStarBackground(renderer);

    backgroundMusic = Mix_LoadMUS("background.mp3");
    shootSound = Mix_LoadWAV("amthanh1.wav");
    explosionSound = Mix_LoadWAV("explosion.wav");

    if (!planeTexture || !bulletTexture || !planeEnemyTexture || !bulletEnemyTexture ||
        !imageSpace || !starBackground || !backgroundMusic || !shootSound || !explosionSound) {
        SDL_Log("Lỗi khi load tài nguyên!");
        exit(1);
    }
    font = TTF_OpenFont("font2.ttf", 30);
if (!font) {
    cout << "Failed to load font\n";
}

}



// Tạo nền sao
SDL_Texture* createStarBackground(SDL_Renderer* renderer) {
    SDL_Texture* background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, background);

    // Tô nền đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ các ngôi sao
    srand(time(0));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 1000; i++) {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        SDL_RenderDrawPoint(renderer, x, y);
    }

    SDL_SetRenderTarget(renderer, oldTarget);
    return background;
}

// Thoát SDL
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* planeTexture, SDL_Texture* bulletTexture, SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture, SDL_Texture* starBackground) {
    if (planeTexture) SDL_DestroyTexture(planeTexture);
    if (bulletTexture) SDL_DestroyTexture(bulletTexture);
    if (planeEnemyTexture) SDL_DestroyTexture(planeEnemyTexture);
    if (bulletEnemyTexture) SDL_DestroyTexture(bulletEnemyTexture);
    if (starBackground) SDL_DestroyTexture(starBackground);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}


// Vẽ nền sao cuộn
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int bgY1, int bgY2) {
    SDL_Rect bg1Rect = { 0, bgY1, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bg2Rect = { 0, bgY2, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, background, NULL, &bg1Rect);
    SDL_RenderCopy(renderer, background, NULL, &bg2Rect);
}
void updateBackground(int& bgY1, int& bgY2) {
    bgY1 += 2;
    bgY2 += 2;
    if (bgY1 >= SCREEN_HEIGHT) bgY1 = -SCREEN_HEIGHT;
    if (bgY2 >= SCREEN_HEIGHT) bgY2 = -SCREEN_HEIGHT;
}


