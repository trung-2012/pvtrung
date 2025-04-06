#include "Menu.h"
#include "Playing.h"
#include "GameOver.h"
#include "SDLUtils.h"
#include "Bullet.h"
#include "action.h"
#include "diem.h"
#include "gameOverScreen.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const char* WINDOW_TITLE = "Máy Bay - Nền Cuộn";

Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* shootSound = nullptr;
Mix_Chunk* explosionSound = nullptr;

int main(int argc, char* argv[]) {
    srand(time(0));

    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);


    if (TTF_Init() == -1) return -1;
    TTF_Font* font = TTF_OpenFont("font2.ttf", 80);
    if (!font) return -1;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return -1;

    SDL_Texture* planeTexture = loadTexture("ok.png", renderer);
    SDL_Texture* bulletTexture = loadTexture("bulletpro.png", renderer);
    SDL_Texture* planeEnemyTexture = loadTexture("ufo.png", renderer);
    SDL_Texture* bulletEnemyTexture = loadTexture("dan.png", renderer);
    SDL_Texture* imageSpace = loadTexture("space.png", renderer);
    SDL_Texture* starBackground = createStarBackground(renderer);

    backgroundMusic = Mix_LoadMUS("background.mp3");
    shootSound = Mix_LoadWAV("amthanh1.wav");
    explosionSound = Mix_LoadWAV("explosion.wav");

    enum GameState { MENU, PLAYING, GAME_OVER };
    GameState gameState = MENU;
    bool isRunning = true;
    int score = 0;

    while (isRunning) {
        switch (gameState) {
            case MENU:
                isRunning = Menu(renderer, starBackground, imageSpace, backgroundMusic, window);
                if (isRunning) gameState = PLAYING;
                break;
            case PLAYING:
                isRunning = GamePlay(renderer, window, starBackground, imageSpace,
                                     planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture,
                                     backgroundMusic, shootSound, explosionSound, score);
                if (!isRunning) {
                    gameState = GAME_OVER;
                    isRunning = true;
                }
                break;
            case GAME_OVER:
                isRunning = GameOver(renderer, font, score,
                                     planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture,
                                     starBackground, window);
                break;
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground);
    system("cls");
    SDL_Delay(1000);
    cout << "Game Over! Score: " << score << endl;
    return 0;
}
