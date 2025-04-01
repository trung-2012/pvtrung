#include "SDLUtils.h"
#include "nencuon.h"
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 500; // Kích thước màn hình
const int SCREEN_HEIGHT = 750;

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

// Hàm cập nhật vị trí nền
void updateBackground(int& bgY1, int& bgY2) {
    bgY1 += 2;
    bgY2 += 2;
    if (bgY1 >= SCREEN_HEIGHT) bgY1 = -SCREEN_HEIGHT;
    if (bgY2 >= SCREEN_HEIGHT) bgY2 = -SCREEN_HEIGHT;
}

// Vẽ nền sao cuộn
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int bgY1, int bgY2) {
    SDL_Rect bg1Rect = { 0, bgY1, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bg2Rect = { 0, bgY2, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, background, NULL, &bg1Rect);
    SDL_RenderCopy(renderer, background, NULL, &bg2Rect);
}
void quitSDL(SDL_Texture* starBackground){
    if (starBackground) SDL_DestroyTexture(starBackground);
}
