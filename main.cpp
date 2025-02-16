#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include<algorithm>

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const char* WINDOW_TITLE = "Máy Bay - Nền Cuộn";

//vien đạn
struct Bullet {
    int x, y;
    int speed = 5;  // Tốc độ đạn
};
// Hàm báo lỗi
void logErrorAndExit(const char* msg, const char* error) {
    SDL_Log("%s: %s", msg, error);
    SDL_Quit();
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
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* planeTexture, SDL_Texture* starBackground) {
    SDL_DestroyTexture(planeTexture);
    SDL_DestroyTexture(starBackground);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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

// Vẽ máy bay bằng hình ảnh
void drawPlane(SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y) {
    SDL_Rect destRect = { x, y, 50, 50 };  // Kích thước máy bay
    SDL_RenderCopy(renderer, planeTexture, NULL, &destRect);
}

// Vẽ đạn
void drawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, int x, int y) {
    SDL_Rect bulletRect = { x + 1, y -10, 5, 20 };  // Đạn nhỏ hơn máy bay
    SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
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
                    if (planeX > 10) planeX -= 10;
                    break;
                case SDLK_RIGHT:
                    if (planeX < SCREEN_WIDTH - 50) planeX += 10;
                    break;
                case SDLK_UP:
                    if (planeY > 10) planeY -= 10;
                    break;
                case SDLK_DOWN:
                    if (planeY < SCREEN_HEIGHT - 70) planeY += 10;
                    break;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);

    // Load hình ảnh máy bay & đạn
    SDL_Texture* planeTexture = loadTexture("ok.png", renderer);
    SDL_Texture* bulletTexture = loadTexture("bulletpro.png", renderer);

    if (!planeTexture || !bulletTexture) {
        quitSDL(window, renderer, planeTexture, bulletTexture);
        return -1;
    }

    // Tạo nền sao
    SDL_Texture* starBackground = createStarBackground(renderer);
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;

    int planeX = SCREEN_WIDTH / 2 - 20;
    int planeY = SCREEN_HEIGHT - 100;
    bool running = true;

    vector<Bullet> bullets;  // Danh sách đạn
    Uint32 lastBulletTime = SDL_GetTicks();  // Lưu thời gian viên đạn cuối cùng được bắn

    while (running) {
        handleEvents(running, planeX, planeY);

        // Tạo đạn mới mỗi 150ms
        if (SDL_GetTicks() - lastBulletTime >= 150) {
            bullets.push_back({planeX + 22, planeY});  // Đạn bắn từ giữa máy bay
            lastBulletTime = SDL_GetTicks();
        }

        // Cập nhật vị trí đạn
        for (auto& bullet : bullets) {
            bullet.y -= bullet.speed;
        }

        // Xóa đạn ra khỏi màn hình
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.y < 0; }), bullets.end());

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Di chuyển nền sao xuống
        bgY1 += 2;
        bgY2 += 2;

        if (bgY1 >= SCREEN_HEIGHT) bgY1 = -SCREEN_HEIGHT;
        if (bgY2 >= SCREEN_HEIGHT) bgY2 = -SCREEN_HEIGHT;

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ nền sao cuộn
        renderBackground(renderer, starBackground, bgY1, bgY2);

        // Vẽ máy bay
        drawPlane(renderer, planeTexture, planeX, planeY);

        // Vẽ đạn
        for (const auto& bullet : bullets) {
            drawBullet(renderer, bulletTexture, bullet.x, bullet.y);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    quitSDL(window, renderer, planeTexture, starBackground);
    return 0;
}
