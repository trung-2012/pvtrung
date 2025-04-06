// Playing.cpp
#include "Playing.h"
#include "SDLUtils.h"
#include "action.h"
#include "Bullet.h"
#include "diem.h"
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// Biến toàn cục hoặc biến được truyền từ file main.cpp
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

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

bool GamePlay(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* starBackground, SDL_Texture* imageSpace,
              SDL_Texture* planeTexture, SDL_Texture* bulletTexture, SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture,
              Mix_Music* backgroundMusic, Mix_Chunk* shootSound, Mix_Chunk* explosionSound, int& score) {
    bool isPlaying = true;
    bool isRapidFire = false;
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;

    Uint32 rapidFireStartTime = 0;
    Uint32 lastBulletTime = SDL_GetTicks();
    Uint32 lastEnemySpawnTime = SDL_GetTicks();

    int planeX = SCREEN_WIDTH / 2 - 20;
    int planeY = SCREEN_HEIGHT - 100;

    vector<Bullet> bullets;
    vector<BulletEnemy> enemyBullets;
    vector<Enemy> enemies;

    Mix_PlayMusic(backgroundMusic, -1);

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
            Mix_PlayChannel(-1, shootSound, 0);
            lastBulletTime = SDL_GetTicks();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
        }

        if (SDL_GetTicks() - lastEnemySpawnTime >= 1000) {
            enemies.push_back({rand() % (SCREEN_WIDTH - 50), -50, 2, SDL_GetTicks()});
            lastEnemySpawnTime = SDL_GetTicks();
        }

        for (auto& bullet : bullets) bullet.y -= bullet.speed;
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.y < 0; }), bullets.end());

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
                Mix_PlayChannel(-1, explosionSound, 0);
                it = enemies.erase(it);
            } else ++it;
        }

        for (auto& bullet : enemyBullets) {
            if (checkCollision(bullet.x, bullet.y, 5, 20, planeX, planeY, 50, 50)) {
                return false;
            }
        }

        updateBackground(bgY1, bgY2);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderBackground(renderer, starBackground, bgY1, bgY2);
        drawPlane(renderer, planeTexture, planeX, planeY);

        for (const auto& enemy : enemies) drawPlaneEnemy(renderer, planeEnemyTexture, enemy.x, enemy.y);
        for (const auto& bullet : bullets) drawBullet(renderer, bulletTexture, bullet.x, bullet.y);
        for (const auto& bullet : enemyBullets) drawBulletEnemy(renderer, bulletEnemyTexture, bullet.x, bullet.y);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    return false;
}
