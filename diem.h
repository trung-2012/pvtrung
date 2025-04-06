#ifndef DIEM_H
#define DIEM_H

#include <SDL.h>
#include <SDL_ttf.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;


void showScore(SDL_Renderer* renderer, TTF_Font* font, int score);

#endif
