#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#include<cmath>

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const char* WINDOW_TITLE = "Máy Bay - Nền Cuộn";

//vien đạn
struct Bullet {
    int x, y;
    int speed = 5;  // Tốc độ đạn
};

//Đạn địch
struct BulletEnemy {
    int x, y;
    int speed = 2;
    float angle; // Góc bắn (đơn vị: độ)
};
// Máy bay địch
struct Enemy {
    int x, y;
    int speed = 2; // Tốc độ di chuyển của máy bay địch
    Uint32 lastShotTime; // Thời gian lần cuối máy bay địch bắn đạn
};

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

// Vẽ máy bay bằng hình ảnh
void drawPlane(SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y) {
    SDL_Rect destRect = { x, y, 50, 50 };  // Kích thước máy bay
    SDL_RenderCopy(renderer, planeTexture, NULL, &destRect);
}
// Vẽ máy bay địch
void drawPlaneEnemy (SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y) {
    SDL_Rect destRect = { x, y, 50, 50 };  // Kích thước máy bay
    SDL_RenderCopy(renderer, planeTexture, NULL, &destRect);
}
// Vẽ đạn
void drawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, int x, int y) {
    SDL_Rect bulletRect = { x + 1, y -10, 5, 20 };  // Đạn nhỏ hơn máy bay
    SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
}
//Vẽ đạn địch
void drawBulletEnemy(SDL_Renderer* renderer, SDL_Texture* bulletEnemyTexture, int x, int y) {
    SDL_Rect bulletRect = { x + 1, y - 10, 10, 10 };
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
// hàm check máy bay va chạm
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2);
}


int main(int argc, char* argv[]) {
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);

    // Load hình ảnh máy bay & đạn
    SDL_Texture* planeTexture = loadTexture("ok.png", renderer);
    SDL_Texture* bulletTexture = loadTexture("bulletpro.png", renderer);
    SDL_Texture* planeEnemyTexture = loadTexture("enemyy.png", renderer);
    SDL_Texture* bulletEnemyTexture = loadTexture("dan.png", renderer);

    if (!planeTexture || !bulletTexture || !planeEnemyTexture || !bulletEnemyTexture) {
    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, nullptr);
    return -1;
}


    // Tạo nền sao
    SDL_Texture* starBackground = createStarBackground(renderer);
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;

    int planeX = SCREEN_WIDTH / 2 - 20;
    int planeY = SCREEN_HEIGHT - 100;
    int score = 0;
    bool running = true;

    vector<Bullet> bullets;// Danh sách đạn
    vector<BulletEnemy> enemyBullets;
    vector<Enemy> enemies;

    Uint32 lastBulletTime = SDL_GetTicks();  // Lưu thời gian viên đạn cuối cùng được bắn
    Uint32 lastEnemySpawnTime = SDL_GetTicks();

    while (running) {
        handleEvents(running, planeX, planeY);

// máy bay ở trong màn hình

        SDL_Event e;
while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) running = false;
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: if (planeX > 10) planeX -= 10; break;
            case SDLK_RIGHT: if (planeX < SCREEN_WIDTH - 50) planeX += 10; break;
            case SDLK_UP: if (planeY > 10) planeY -= 10; break;
            case SDLK_DOWN: if (planeY < SCREEN_HEIGHT - 70) planeY += 10; break;
        }
    }
}


        // Tạo đạn mới mỗi 150ms
        if (SDL_GetTicks() - lastBulletTime >= 200) {
            bullets.push_back({planeX + 22, planeY});  // Đạn bắn từ giữa máy bay
            lastBulletTime = SDL_GetTicks();
        }
         // Sinh máy bay địch sau mỗi 1 giây
        if (SDL_GetTicks() - lastEnemySpawnTime >= 1000) {
            enemies.push_back({rand() % (SCREEN_WIDTH - 50), -50, 2, SDL_GetTicks()});
            lastEnemySpawnTime = SDL_GetTicks();
        }
        // Cập nhật vị trí đạn
        for (auto& bullet : bullets) {
            bullet.y -= bullet.speed;
        }

        // Xóa đạn ra khỏi màn hình
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.y < 0; }), bullets.end());

        // Cập nhật vị trí đạn và máy bay địch

       for (auto& enemy : enemies) {
            enemy.y += enemy.speed;
           if (SDL_GetTicks() - enemy.lastShotTime >= 1500) {
           vector<float> angles = {-150, -90, -30}; // Các góc lệch
           for (float angle : angles) {
           enemyBullets.push_back({enemy.x + 22, enemy.y + 50, 4, angle});
    }
    enemy.lastShotTime = SDL_GetTicks();
}

        }
        enemies.erase(remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return e.y > SCREEN_HEIGHT; }), enemies.end());

        for (auto& bullet : enemyBullets) {
    bullet.x += bullet.speed * cos(bullet.angle * M_PI / 180.0);
    bullet.y -= bullet.speed * sin(bullet.angle * M_PI / 180.0);
}

        enemyBullets.erase(remove_if(enemyBullets.begin(), enemyBullets.end(), [](BulletEnemy& b) { return b.y > SCREEN_HEIGHT; }), enemyBullets.end());

        // bắn đạn vào kẻ địch
        for (auto it = enemies.begin(); it != enemies.end();) {
    bool hit = false;
    for (auto bt = bullets.begin(); bt != bullets.end();) {
        if (checkCollision(bt->x, bt->y, 5, 20, it->x, it->y, 50, 50)) {
            bt = bullets.erase(bt);
            hit = true;
            score += 10;
            break;
        } else ++bt;
    }
    if (hit) it = enemies.erase(it); else ++it;
}

// va chạm máy bay và kẻ địch
for (auto& bullet : enemyBullets) {
    if (checkCollision(bullet.x, bullet.y, 5, 20, planeX, planeY, 50, 50)) {
        running = false;
    }
}


        // Xóa màn hình

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
        // Vẽ máy bay địch
       for (const auto& enemy : enemies) {
       drawPlaneEnemy(renderer, planeEnemyTexture, enemy.x, enemy.y);
}


        // Vẽ đạn
        for (const auto& bullet : bullets) {
            drawBullet(renderer, bulletTexture, bullet.x, bullet.y);
        }
        // Vẽ đạn của máy bay địch
        for (const auto& bullet : enemyBullets) {
        drawBulletEnemy(renderer, bulletEnemyTexture, bullet.x, bullet.y);
}


        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground);
    cout << "Game Over! Score: " << score << endl;
    return 0;
}
