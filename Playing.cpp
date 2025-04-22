// Playing.cpp
#include "Playing.h"
#include "SDLUtils.h"
#include "action.h"
#include "Bullet.h"
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Texture* planeTexture;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* planeEnemyTexture;
extern SDL_Texture* bulletEnemyTexture;
extern SDL_Texture* starBackground;
extern Mix_Music* backgroundMusic;
extern Mix_Chunk* shootSound;
extern Mix_Chunk* explosionSound;
extern int score;
extern TTF_Font* font;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//kiemtra chuot
bool isPointInRect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x < rect.x + rect.w &&
            y >= rect.y && y < rect.y + rect.h);
}
//hien thi diem
void renderScore(SDL_Renderer* renderer, int score) {
    SDL_Color textColor = {255, 255, 255}; // Trắng

    string scoreText = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) return;

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect renderQuad = {SCREEN_WIDTH - textWidth - 20, 20, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_DestroyTexture(textTexture);
}

GameResult GamePlay(SDL_Renderer* renderer, SDL_Window* window, int& score)
 {
    // Tải texture cho các nút
    SDL_Texture* pauseButtonTexture = loadTexture("pause.png", renderer);
    SDL_Texture* continueButtonTexture = loadTexture("countinue.png", renderer);
    SDL_Texture* menuButtonTexture = loadTexture("backtomenu.png", renderer);

    // Kiểm tra lỗi khi tải texture
    if (!pauseButtonTexture || !continueButtonTexture || !menuButtonTexture) {
        cout << "Failed to load button textures: " << SDL_GetError() << endl;
        return GR_TO_GAME_OVER;
    }

    // Lấy kích thước của các texture nút
    int pauseButtonWidth, pauseButtonHeight;
    int continueButtonWidth, continueButtonHeight;
    int menuButtonWidth, menuButtonHeight;

    SDL_QueryTexture(pauseButtonTexture, nullptr, nullptr, &pauseButtonWidth, &pauseButtonHeight);
    SDL_QueryTexture(continueButtonTexture, nullptr, nullptr, &continueButtonWidth, &continueButtonHeight);
    SDL_QueryTexture(menuButtonTexture, nullptr, nullptr, &menuButtonWidth, &menuButtonHeight);

    // Tạo các rect cho nút với kích thước được scale lại
    float scaleFactorPause = 0.06f; // Scale nhỏ hơn cho nút pause
    float scaleFactorButtons = 0.2f; // Scale nhỏ hơn cho nút menu và continue

    SDL_Rect pauseButtonRect = {
        20,
        20,
        static_cast<int>(pauseButtonWidth * scaleFactorPause),
        static_cast<int>(pauseButtonHeight * scaleFactorPause)
    };

    SDL_Rect continueButtonRect = {
        SCREEN_WIDTH/2 - static_cast<int>(continueButtonWidth * scaleFactorButtons)/2,
        SCREEN_HEIGHT/2 - 60,
        static_cast<int>(continueButtonWidth * scaleFactorButtons),
        static_cast<int>(continueButtonHeight * scaleFactorButtons)
    };

    SDL_Rect menuButtonRect = {
        SCREEN_WIDTH/2 - static_cast<int>(menuButtonWidth * scaleFactorButtons)/2,
        SCREEN_HEIGHT/2 + 10,
        static_cast<int>(menuButtonWidth * scaleFactorButtons),
        static_cast<int>(menuButtonHeight * scaleFactorButtons)
    };

    bool isPlaying = true;
    bool isPaused = false;
    bool isRapidFire = false;
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;

    Uint32 rapidFireStartTime = 0;
    Uint32 lastBulletTime = SDL_GetTicks();
    Uint32 lastEnemySpawnTime = SDL_GetTicks();

    //vi tri ban dau
    int planeX = SCREEN_WIDTH / 2 - 20;
    int planeY = SCREEN_HEIGHT - 100;

    vector<Bullet> bullets;
    vector<BulletEnemy> enemyBullets;
    vector<Enemy> enemies;

    if (!isMuted) Mix_PlayMusic(backgroundMusic, -1);

    while (isPlaying) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return GR_TO_GAME_OVER;

            // Xử lý sự kiện chuột
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isPointInRect(mouseX, mouseY, pauseButtonRect) && !isPaused) {
                    isPaused = true;
                    if (!isMuted) Mix_PauseMusic();
                }

                if (isPaused) {
                    if (isPointInRect(mouseX, mouseY, continueButtonRect)) {
                        isPaused = false;
                        if (!isMuted) Mix_ResumeMusic();
                    } else if (isPointInRect(mouseX, mouseY, menuButtonRect)) {
                        // Giải phóng texture trước khi thoát
                        if (!isMuted) Mix_HaltMusic();
                        SDL_DestroyTexture(pauseButtonTexture);
                        SDL_DestroyTexture(continueButtonTexture);
                        SDL_DestroyTexture(menuButtonTexture);
                        return GR_TO_MENU;; // Trở về menu
                    }
                }
            }
        }

        // xu ly game khi pauseee
        if (!isPaused) {
            handleEvents(isPlaying, planeX, planeY);

            // DU DIEM THI BẮN NHANH
            if (score % 150 == 0 && score > 0 && !isRapidFire) {
                isRapidFire = true;
                rapidFireStartTime = SDL_GetTicks();
                system("cls");
                cout << "Ban nhan duoc buff trong 3s" << flush;
            }

            //TẮT BẮN NHANH 3S
            if (isRapidFire && SDL_GetTicks() - rapidFireStartTime >= 3000) {
                isRapidFire = false;
            }

            // BẮN ĐẠN
            Uint32 bulletCooldown = isRapidFire ? 100 : 225;
            if (SDL_GetTicks() - lastBulletTime >= bulletCooldown) {
                bullets.push_back({planeX + 22, planeY});
                if (!isMuted) Mix_PlayChannel(-1, shootSound, 0);
                lastBulletTime = SDL_GetTicks();
            }

            // TẠO MÁY BAY ĐỊCH
            if (SDL_GetTicks() - lastEnemySpawnTime >= 1000) {
                enemies.push_back({rand() % (SCREEN_WIDTH - 50), -50, 2, SDL_GetTicks()});
                lastEnemySpawnTime = SDL_GetTicks();
            }

            //CẬP NHẬT VỊ TRÍ NGƯỜI CHƠI
            for (auto& bullet : bullets) bullet.y -= bullet.speed;
            bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.y < 0; }), bullets.end());

            //CẬP NHẬT ĐỊCH VÀ ĐẠN ĐỊCH
            for (auto& enemy : enemies) {
                enemy.y += enemy.speed;
                if (SDL_GetTicks() - enemy.lastShotTime >= 1500) {
                    vector<float> angles = {-150, -90, -30};
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

            //BẮN TRÚNG ĐỊCH
            for (auto it = enemies.begin(); it != enemies.end();) {
                bool hit = false;
                for (auto bt = bullets.begin(); bt != bullets.end();) {
                    if (checkCollision(bt->x, bt->y, 5, 20, it->x, it->y, 50, 50)) {
                        bt = bullets.erase(bt);
                        hit = true;
                        score += 10;
                        system("cls");
                        cout << "Diem cua ban la : " << score << " " << flush;
                        break;
                    } else ++bt;
                }
                if (hit) {
                    if (!isMuted) Mix_PlayChannel(-1, explosionSound, 0);
                    it = enemies.erase(it);
                } else ++it;
            }

            // BỊ ĂN ĐẠN
            for (auto& bullet : enemyBullets) {
                if (checkCollision(bullet.x, bullet.y, 5, 20, planeX, planeY, 50, 50)) {
                    // Giải phóng texture trước khi thoát
                    SDL_DestroyTexture(pauseButtonTexture);
                    SDL_DestroyTexture(continueButtonTexture);
                    SDL_DestroyTexture(menuButtonTexture);
                    return GR_TO_GAME_OVER;
                }
            }

            updateBackground(bgY1, bgY2);
        }

        // Phần render
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderBackground(renderer, starBackground, bgY1, bgY2);
        drawPlane(renderer, planeTexture, planeX, planeY);

        for (const auto& enemy : enemies) drawPlaneEnemy(renderer, planeEnemyTexture, enemy.x, enemy.y);
        for (const auto& bullet : bullets) drawBullet(renderer, bulletTexture, bullet.x, bullet.y);
        for (const auto& bullet : enemyBullets) drawBulletEnemy(renderer, bulletEnemyTexture, bullet.x, bullet.y);

        renderScore(renderer, score);

        // Vẽ nút pause ở góc trên bên trái
        SDL_RenderCopy(renderer, pauseButtonTexture, nullptr, &pauseButtonRect);

        // Vẽ menu pause khi game đang pause
        if (isPaused) {
            // Làm mờ màn hình
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &fullScreen);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            // Vẽ tiêu đề PAUSE
            SDL_Color textColor = {255, 255, 255};
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, "PAUSED", textColor);
            if (textSurface) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                if (textTexture) {
                    int textWidth, textHeight;
                    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
                    SDL_Rect textRect = {SCREEN_WIDTH/2 - textWidth/2, SCREEN_HEIGHT/2 - 120, textWidth, textHeight};
                    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
            }

            // Vẽ các nút continue và menu
            SDL_RenderCopy(renderer, continueButtonTexture, nullptr, &continueButtonRect);
            SDL_RenderCopy(renderer, menuButtonTexture, nullptr, &menuButtonRect);

        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    // Giải phóng texture trước khi thoát
    SDL_DestroyTexture(pauseButtonTexture);
    SDL_DestroyTexture(continueButtonTexture);
    SDL_DestroyTexture(menuButtonTexture);

    return GR_CONTINUE;
}
