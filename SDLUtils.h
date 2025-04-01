#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <SDL.h>
#include <SDL_image.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const char* WINDOW_TITLE;


SDL_Window* initSDL();
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
SDL_Texture* createStarBackground(SDL_Renderer* renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* planeTexture, SDL_Texture* bulletTexture, SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture, SDL_Texture* starBackground);
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int bgY1, int bgY2);
void updateBackground(int& bgY1, int& bgY2);
#endif // SDLUTILS_H
