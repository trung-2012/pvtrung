#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "SDLUtils.h"
#include <vector>
#include <string>


// Cấu trúc cho nút menu dạng hình ảnh
struct ImageButton {
    SDL_Texture* texture;
    SDL_Rect rect;
    int id;
};

// Cấu trúc để lưu trữ điểm số
struct ScoreEntry {
    std::string name;
    int score;
};

// Các ID cho nút menu
enum ButtonID {
    BTN_PLAY,
    BTN_MUSIC,
    BTN_LEADERBOARD,
    BTN_QUIT
};

// Khai báo hàm Menu
bool Menu(SDL_Renderer* renderer, SDL_Window* window);

// Khai báo các hàm bổ trợ
void updateBackground(int& bgY1, int& bgY2);
void renderBackground(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, int bgY1, int bgY2);

// Biến toàn cục sử dụng cho menu (có thể đưa vào namespace hoặc class nếu cần)
extern SDL_Texture* starBackground;
extern Mix_Music* backgroundMusic;
