#include "Menu.h"
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "HighScore.h"

bool Menu(SDL_Renderer* renderer, SDL_Window* window) {
    bool isMenu = true;
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;
    bool showLeaderboard = false;

    // Ban đầu bật nhạc
    if (!isMuted && backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1);
    }

    // Tải hình ảnh cho các nút menu - với đúng đường dẫn
    SDL_Texture* playButtonTexture        = IMG_LoadTexture(renderer, "start.png");
    SDL_Texture* musicOnButtonTexture     = IMG_LoadTexture(renderer, "musicon.png");
    SDL_Texture* musicOffButtonTexture    = IMG_LoadTexture(renderer, "musicoff.png");
    SDL_Texture* leaderboardButtonTexture = IMG_LoadTexture(renderer, "highscore.png");
    SDL_Texture* quitButtonTexture        = IMG_LoadTexture(renderer, "quit.png");
    SDL_Texture* backButtonTexture        = IMG_LoadTexture(renderer, "exit.png");
    SDL_Texture* leaderboardBgTexture     = IMG_LoadTexture(renderer, "play.png");

    // Kiểm tra lỗi khi tải hình ảnh
    if (!playButtonTexture || !musicOnButtonTexture || !musicOffButtonTexture ||
        !leaderboardButtonTexture || !quitButtonTexture ||
        !backButtonTexture || !leaderboardBgTexture) {
        printf("Lỗi khi tải hình ảnh nút: %s\n", IMG_GetError());
        return false;
    }

    // Tạo các nút menu
    std::vector<ImageButton> buttons;
    int buttonWidth  = 180;
    int buttonHeight = 50;
    int centerX      = SCREEN_WIDTH / 2 - buttonWidth / 2;
    int startY       = 200;
    int buttonSpacing = 70;

    buttons.push_back({playButtonTexture,        {centerX, startY, buttonWidth, buttonHeight}, BTN_PLAY});
    buttons.push_back({!isMuted ? musicOnButtonTexture : musicOffButtonTexture,
                       {centerX, startY + buttonSpacing, buttonWidth, buttonHeight}, BTN_MUSIC});
    buttons.push_back({leaderboardButtonTexture, {centerX, startY + buttonSpacing * 2, buttonWidth, buttonHeight}, BTN_LEADERBOARD});
    buttons.push_back({quitButtonTexture,        {centerX, startY + buttonSpacing * 3, buttonWidth, buttonHeight}, BTN_QUIT});

    // Nút Back cho bảng xếp hạng
    int backButtonWidth  = 100;
    int backButtonHeight = 40;
    ImageButton backButton = {
        backButtonTexture,
        {SCREEN_WIDTH / 2 - backButtonWidth / 2, SCREEN_HEIGHT - 80, backButtonWidth, backButtonHeight},
        -1
    };

    // Tải điểm cao nhất
    HighScoreEntry highScore = loadHighScore();

    // Font chữ
    TTF_Font* menuFont = font;
    if (!menuFont) {
        menuFont = TTF_OpenFont("font2.ttf", 20);
        if (!menuFont) {
            printf("Lỗi TTF_OpenFont: %s\n", TTF_GetError());
            SDL_DestroyTexture(playButtonTexture);
            SDL_DestroyTexture(musicOnButtonTexture);
            SDL_DestroyTexture(musicOffButtonTexture);
            SDL_DestroyTexture(leaderboardButtonTexture);
            SDL_DestroyTexture(quitButtonTexture);
            SDL_DestroyTexture(backButtonTexture);
            SDL_DestroyTexture(leaderboardBgTexture);
            return false;
        }
    }

    SDL_Color textColor = {255, 255, 255, 255};

    while (isMenu) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                isMenu = false;
                goto CLEANUP;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // xem diem xep hang
                if (showLeaderboard) {
                    if (mouseX >= backButton.rect.x && mouseX <= backButton.rect.x + backButton.rect.w &&
                        mouseY >= backButton.rect.y && mouseY <= backButton.rect.y + backButton.rect.h) {
                        showLeaderboard = false;
                    }
                } else {
                    for (auto& button : buttons) {
                        if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
                            mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) {

                            switch (button.id) {
                                case BTN_PLAY:
                                    goto CLEANUP_RETURN_TRUE;

                                case BTN_MUSIC:
                                    isMuted = !isMuted;
                                    if (!isMuted) {
                                        Mix_PlayMusic(backgroundMusic, -1);
                                    } else {
                                        Mix_HaltMusic();
                                    }

                                    for (auto& b : buttons) {
                                        if (b.id == BTN_MUSIC) {
                                            b.texture = !isMuted ? musicOnButtonTexture : musicOffButtonTexture;
                                            break;
                                        }
                                    }
                                    break;

                                case BTN_LEADERBOARD:
                                    showLeaderboard = true;
                                    break;

                                case BTN_QUIT:
                                    goto CLEANUP;
                            }
                            break;
                        }
                    }
                }
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (showLeaderboard) {
                    showLeaderboard = false;
                } else {
                    isMenu = false;
                    goto CLEANUP;
                }
            }
        }

        updateBackground(bgY1, bgY2);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderBackground(renderer, starBackground, bgY1, bgY2);

        if (showLeaderboard) {
            SDL_Rect leaderboardRect = {SCREEN_WIDTH / 2 - 200, 50, 400, 550};
            SDL_RenderCopy(renderer, leaderboardBgTexture, nullptr, &leaderboardRect);

            SDL_Surface* titleSurface = TTF_RenderText_Solid(menuFont, "Record", textColor);
            if (titleSurface) {
                SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
                if (titleTexture) {
                    SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 90, 80, 180, 40};
                    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
                    SDL_DestroyTexture(titleTexture);
                }
                SDL_FreeSurface(titleSurface);
            }

            std::string entry = highScore.name + ": " + std::to_string(highScore.score);
            SDL_Surface* scoreSurface = TTF_RenderText_Solid(menuFont, entry.c_str(), textColor);
            if (scoreSurface) {
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                if (scoreTexture) {
                    SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - 130, 180, 260, 40};
                    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                    SDL_DestroyTexture(scoreTexture);
                }
                SDL_FreeSurface(scoreSurface);
            }

            SDL_RenderCopy(renderer, backButton.texture, nullptr, &backButton.rect);
        } else {
            for (auto& button : buttons) {
                SDL_RenderCopy(renderer, button.texture, nullptr, &button.rect);
            }
        }

        SDL_RenderPresent(renderer);
    }

CLEANUP:
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(musicOnButtonTexture);
    SDL_DestroyTexture(musicOffButtonTexture);
    SDL_DestroyTexture(leaderboardButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(leaderboardBgTexture);

    if (menuFont != font) {
        TTF_CloseFont(menuFont);
    }
    return false;

CLEANUP_RETURN_TRUE:
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(musicOnButtonTexture);
    SDL_DestroyTexture(musicOffButtonTexture);
    SDL_DestroyTexture(leaderboardButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(leaderboardBgTexture);

    if (menuFont != font) {
        TTF_CloseFont(menuFont);
    }
    return true;
}
