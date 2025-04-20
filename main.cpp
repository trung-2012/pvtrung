#include "Menu.h"
#include "Playing.h"
#include "GameOver.h"
#include "gameOverScreen.h"

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
            case MENU: {
                isRunning = Menu(renderer, window);
                if (isRunning) gameState = PLAYING;
                break;
            }

            case PLAYING: {
                GameResult result = GamePlay(renderer, window, score);
                if (result == GR_TO_GAME_OVER) {
                    gameState = GAME_OVER;
                } else if (result == GR_TO_MENU) {
                    gameState = MENU;
                }
                break;
            }

            case GAME_OVER: {
                int result = GameOver(renderer, font, score, window);
                if (result == GO_RESTART) {
                    score = 0;
                    gameState = PLAYING;
                } else if (result == GO_MENU) {
                    score = 0;
                    gameState = MENU;
                } else {
                    isRunning = false;
                }
                break;
            }
        }
    }

    // Clean up
    TTF_CloseFont(font);
    TTF_Quit();
    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground);

    system("cls");
    SDL_Delay(1000);
    cout << "Game Over! Score: " << score << endl;

    return 0;
}
