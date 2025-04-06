// gameOverScreen.cpp
#include "gameOverScreen.h"

void showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score,
                        SDL_Texture* planeTexture, SDL_Texture* bulletTexture,
                        SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture,
                        SDL_Texture* starBackground,
                        SDL_Window* window, int argc, char* argv[]) {

    SDL_RenderClear(renderer);

    // Hiển thị điểm số
    showScore(renderer, font, score);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Press SPACE to Exit or R to Restart", textColor);
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);

    SDL_Rect messageRect = {500 / 2 - 180, 750 / 2 + 50, 360, 30};
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);

    bool waitingForInput = true;
    while (waitingForInput) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                waitingForInput = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    waitingForInput = false;
                } else if (e.key.keysym.sym == SDLK_r) {
                    waitingForInput = false;

                    quitSDL(window, renderer, planeTexture, bulletTexture, planeEnemyTexture, bulletEnemyTexture, starBackground);

                    Mix_FreeMusic(backgroundMusic);
                    Mix_FreeChunk(shootSound);
                    Mix_FreeChunk(explosionSound);
                    Mix_CloseAudio();

                    main(argc, argv); // Restart game
                    exit(0);
                }
            }
        }
    }
}
