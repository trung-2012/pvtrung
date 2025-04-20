#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>

// Khai báo enum để xác định kết quả chơi
enum GameResult {
    GR_CONTINUE,
    GR_TO_MENU,
    GR_TO_GAME_OVER
};

// Khai báo các hàm tiện ích
bool isPointInRect(int x, int y, SDL_Rect rect);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

// Sửa lại kiểu trả về của GamePlay
GameResult GamePlay(SDL_Renderer* renderer, SDL_Window* window, int& score);
