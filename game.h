#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class ResourceManager {
public:
    static SDL_Window* initSDL();
    static SDL_Renderer* createRenderer(SDL_Window* window);
    static TTF_Font* loadFont(const char* fontPath, int fontSize);
    static SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
    static Mix_Music* loadMusic(const char* filePath);
    static Mix_Chunk* loadSound(const char* filePath);
    static void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
};

#endif // RESOURCEMANAGER_H
