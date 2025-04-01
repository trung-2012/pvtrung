#include "action.h"
#include <SDL.h>
// Vẽ máy bay bằng hình ảnh
void drawPlane(SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y) {
    SDL_Rect destRect = { x, y, 50, 50 };  // Kích thước máy bay
    SDL_RenderCopy(renderer, planeTexture, NULL, &destRect);
}
// Vẽ máy bay địch
void drawPlaneEnemy (SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y) {
    SDL_Rect destRect = { x, y, 50, 40 };  // Kích thước máy bay
    SDL_RenderCopy(renderer, planeTexture, NULL, &destRect);
}
// Vẽ đạn
void drawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, int x, int y) {
    SDL_Rect bulletRect = { x + 1, y -10, 5, 20 };  // Đạn nhỏ hơn máy bay

    SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
}
//Vẽ đạn địch
void drawBulletEnemy(SDL_Renderer* renderer, SDL_Texture* bulletEnemyTexture, int x, int y) {
    SDL_Rect bulletRect = { x + 1, y -15, 10, 10 };
    SDL_RenderCopy(renderer, bulletEnemyTexture, NULL, &bulletRect);
}
// Xử lý sự kiện di chuyển
void handleEvents(bool& running, int& planeX, int& planeY) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    if (planeX > 10) planeX -= 12;
                    break;
                case SDLK_RIGHT:
                    if (planeX < SCREEN_WIDTH - 50) planeX += 12;
                    break;
                case SDLK_UP:
                    if (planeY > 10) planeY -= 12;
                    break;
                case SDLK_DOWN:
                    if (planeY < SCREEN_HEIGHT - 70) planeY += 12;
                    break;
            }
        }
    }
}
// hàm check máy bay va chạm
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2);
}


