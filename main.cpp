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


int main(int argc, char* argv[]) {
    srand(time(0));

    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);

    if (TTF_Init() == -1) return -1;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return -1;

    loadAssets(renderer); // Gọi sau Mix_OpenAudio

    enum GameState { MENU, PLAYING, GAME_OVER };
    GameState gameState = MENU;
    bool isRunning = true;
    int score = 0;

    while (isRunning) {
        switch (gameState) {
            case MENU:
                isRunning = Menu(renderer, window); // đã gọn hơn
                if (isRunning) gameState = PLAYING;
                break;
            case PLAYING:
                isRunning = GamePlay(renderer, window, score); // không cần truyền texture
                if (!isRunning) {
                    gameState = GAME_OVER;
                    isRunning = true;
                }
                break;
           case GAME_OVER: {
    int result = GameOver(renderer, font, score, window);
    if (result == GO_RESTART) {
        score = 0;
        gameState = PLAYING;
        isRunning = true;
    } else if (result == GO_MENU) {
        score = 0;
        gameState = MENU;
        isRunning = true;
    } else {
        isRunning = false; // thoát nếu người dùng tắt cửa sổ
    }
    break;
}



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
