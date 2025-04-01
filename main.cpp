#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#include<cmath>
#include "Bullet.h"
#include "SDLUtils.h"
#include "action.h"

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const char* WINDOW_TITLE = "Máy Bay - Nền Cuộn";


Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* shootSound = nullptr;
Mix_Chunk* explosionSound = nullptr;

// Hàm hiển thị điểm số
void showScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = {255, 255, 255}; // Màu trắng
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect renderQuad = {SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 50, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


int main(int argc, char* argv[]) {
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return -1;
    }

    // Tải font
    TTF_Font* font = TTF_OpenFont("font1.ttf", 80); // Thay "font.ttf" bằng đường dẫn đến file font của bạn
    if (!font) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        return -1;
    }


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
    return -1;
}


    // Load hình ảnh máy bay & đạn
    SDL_Texture* planeTexture = loadTexture("ok.png", renderer);
    SDL_Texture* bulletTexture = loadTexture("bulletpro.png", renderer);
    SDL_Texture* planeEnemyTexture = loadTexture("ufo.png", renderer);
    SDL_Texture* bulletEnemyTexture = loadTexture("dan.png", renderer);
    SDL_Texture* imageSpace = loadTexture("space.png", renderer);

    // Tải nhạc nền và hiệu ứng âm thanh
backgroundMusic = Mix_LoadMUS("background.mp3");  // Nhạc nền
shootSound = Mix_LoadWAV("amthanh1.wav");            // Âm thanh bắn đạn
explosionSound = Mix_LoadWAV("explosion.wav");    // Âm thanh nổ

if (!backgroundMusic || !shootSound || !explosionSound) {
    cout << "Failed to load sound! Error: " << Mix_GetError() << endl;
    return -1;
}

// Phát nhạc nền liên tục
Mix_PlayMusic(backgroundMusic, -1);

    if (!planeTexture || !bulletTexture || !planeEnemyTexture || !bulletEnemyTexture ) {
    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture,nullptr);
    return -1;
}
SDL_Texture* starBackground = createStarBackground(renderer);
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;

   bool isPlaying = false;

   bool isGameOver = false;

while (!isPlaying) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture,nullptr);
            return 0;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            isPlaying = true;
        }
    }

    // Di chuyển nền sao xuống
    updateBackground(bgY1,bgY2);
    // Xóa màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ nền sao cuộn
    renderBackground(renderer, starBackground, bgY1, bgY2);

    // Hiển thị thông báo "Press SPACE to start"
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect imageRect = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 20, 300, 50 };
    SDL_RenderCopy(renderer, imageSpace, NULL, &imageRect);

    SDL_RenderPresent(renderer);
}

    int planeX = SCREEN_WIDTH / 2 - 20;
    int planeY = SCREEN_HEIGHT - 100;
    int score = 0;

    vector<Bullet> bullets;// Danh sách đạn
    vector<BulletEnemy> enemyBullets;
    vector<Enemy> enemies;

    Uint32 lastBulletTime = SDL_GetTicks();  // Lưu thời gian viên đạn cuối cùng được bắn
    Uint32 lastEnemySpawnTime = SDL_GetTicks();

    bool isRapidFire = false;
    Uint32 rapidFireStartTime = 0;

    while (isPlaying) {
        handleEvents(isPlaying, planeX, planeY);


        if (score % 150 == 0 && score > 0 && !isRapidFire) {
        isRapidFire = true;
        rapidFireStartTime = SDL_GetTicks();
        system("cls");
        cout << "Ban nhan duoc buff trong 3s" << flush;
    }

    if (isRapidFire && SDL_GetTicks() - rapidFireStartTime >= 3000) {
        isRapidFire = false;
    }

    Uint32 bulletCooldown = isRapidFire ? 100 : 225;

    if (SDL_GetTicks() - lastBulletTime >= bulletCooldown) {
        bullets.push_back({planeX + 22, planeY});
        Mix_PlayChannel(-1, shootSound, 0); // Âm thanh bắn đạn
        lastBulletTime = SDL_GetTicks();
    }
// máy bay ở trong màn hình

        SDL_Event e;
while (SDL_PollEvent(&e)) {
    int a = 1;
    if (e.type == SDL_QUIT) isPlaying = false;
    else if (e.type == SDL_KEYDOWN) {
            a+=1;
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: if (planeX > 10) planeX -= a; break;
            case SDLK_RIGHT: if (planeX < SCREEN_WIDTH - 50) planeX += a; break;
            case SDLK_UP: if (planeY > 10) planeY -= a; break;
            case SDLK_DOWN: if (planeY < SCREEN_HEIGHT - 70) planeY += a; break;
        }
    }
}


        // Tạo đạn mới mỗi 150ms
        if (SDL_GetTicks() - lastBulletTime >= 200) {
            bullets.push_back({planeX + 22, planeY});  // Đạn bắn từ giữa máy bay
            Mix_PlayChannel(-1, shootSound, 0); // Âm thanh bắn đạn
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
            system("cls");
            cout << "Diem cua ban la : " << score  << " " << flush;
            break;
        } else ++bt;
    }
    if (hit){
        Mix_PlayChannel(-1, explosionSound, 0); // Âm thanh nổ khi máy bay địch bị bắn
        it = enemies.erase(it);
        } else ++it;
}

// va chạm máy bay và kẻ địch
for (auto& bullet : enemyBullets) {
    if (checkCollision(bullet.x, bullet.y, 5, 20, planeX, planeY, 50, 50)) {
        isPlaying = false;
        isGameOver = true;
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
    if(isGameOver = true){
        SDL_Delay(500);
    }
    if (isGameOver) {
    SDL_RenderClear(renderer);
    showScore(renderer, font, score); // Hiển thị điểm số
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Đợi 2 giây trước khi thoát
}
TTF_CloseFont(font);
TTF_Quit();


    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground);
    system("cls");
    SDL_Delay(1000);
    cout << "Game Over! Score: " << score << endl;
    return 0;
}
