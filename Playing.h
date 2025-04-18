// Playing.h
#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>

// Khai báo các hàm tiện ích
bool isPointInRect(int x, int y, SDL_Rect rect);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

// Khai báo hàm GamePlay chính
bool GamePlay(SDL_Renderer* renderer, SDL_Window* window, int& score);
