#ifndef NENCUON_H
#define NENCUON_H

#include <SDL.h>

// Hàm tạo nền sao
SDL_Texture* createStarBackground(SDL_Renderer* renderer);

// Hàm cập nhật vị trí nền
void updateBackground(int& bgY1, int& bgY2);

// Hàm vẽ nền
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int bgY1, int bgY2);

void quitSDL(SDL_Texture* starBackground);

#endif // NENCUON_H
