#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
#include <SDL_ttf.h>


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const char* WINDOW_TITLE;

// Tài nguyên hình ảnh
extern SDL_Texture* planeTexture;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* planeEnemyTexture;
extern SDL_Texture* bulletEnemyTexture;
extern SDL_Texture* imageSpace;
extern SDL_Texture* starBackground;
// SDLUtils.h
extern SDL_Texture* buttonRestartTexture;
extern SDL_Texture* buttonMenuTexture;

// Tài nguyên âm thanh
extern Mix_Music* backgroundMusic;
extern Mix_Chunk* shootSound;
extern Mix_Chunk* explosionSound;

extern TTF_Font* font;

SDL_Window* initSDL();
SDL_Renderer* createRenderer(SDL_Window* window);
void loadAssets(SDL_Renderer* renderer);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
SDL_Texture* createStarBackground(SDL_Renderer* renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* planeTexture, SDL_Texture* bulletTexture, SDL_Texture* planeEnemyTexture, SDL_Texture* bulletEnemyTexture, SDL_Texture* starBackground);
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int bgY1, int bgY2);
void updateBackground(int& bgY1, int& bgY2);
#endif // SDLUTILS_H
