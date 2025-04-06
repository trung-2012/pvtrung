#include "Menu.h"
#include <SDL_mixer.h>

bool Menu(SDL_Renderer* renderer, SDL_Texture* starBackground, SDL_Texture* imageSpace, Mix_Music* backgroundMusic, SDL_Window* window) {
    bool isMenu = true;
    int bgY1 = 0;
    int bgY2 = -750; // SCREEN_HEIGHT

    Mix_PlayMusic(backgroundMusic, -1);

    while (isMenu) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                return true;
            }
        }

        updateBackground(bgY1, bgY2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderBackground(renderer, starBackground, bgY1, bgY2);

        SDL_Rect imageRect = { 500 / 2 - 150, 750 / 2 - 20, 300, 50 };
        SDL_RenderCopy(renderer, imageSpace, NULL, &imageRect);

        SDL_RenderPresent(renderer);
    }
    return false;
}

