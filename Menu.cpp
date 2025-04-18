#include "Menu.h"
#include <SDL_mixer.h>
#include <vector>
#include <string>

bool Menu(SDL_Renderer* renderer, SDL_Window* window) {
    bool isMenu = true;
    int bgY1 = 0;
    int bgY2 = -SCREEN_HEIGHT;
    bool musicOn = true;
    bool showLeaderboard = false;

    // Ban đầu bật nhạc
    if (musicOn && backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1);
    }

    // Tải hình ảnh cho các nút menu - với đúng đường dẫn
    SDL_Texture* playButtonTexture = IMG_LoadTexture(renderer, "play.png");
    SDL_Texture* musicOnButtonTexture = IMG_LoadTexture(renderer, "on.png");
    SDL_Texture* musicOffButtonTexture = IMG_LoadTexture(renderer, "off.png");
    SDL_Texture* leaderboardButtonTexture = IMG_LoadTexture(renderer, "bxh.png");
    SDL_Texture* quitButtonTexture = IMG_LoadTexture(renderer, "quit.png");
    SDL_Texture* backButtonTexture = IMG_LoadTexture(renderer, "exit.png");
    SDL_Texture* leaderboardBgTexture = IMG_LoadTexture(renderer, "play.png");

    // Kiểm tra lỗi khi tải hình ảnh
    if (!playButtonTexture || !musicOnButtonTexture || !musicOffButtonTexture ||
        !leaderboardButtonTexture || !quitButtonTexture || !backButtonTexture || !leaderboardBgTexture) {
        printf("Lỗi khi tải hình ảnh nút: %s\n", IMG_GetError());
        // Xử lý dọn dẹp và thoát
        return false;
    }

    // Tạo các nút menu
    std::vector<ImageButton> buttons;

    // Kích thước nút - điều chỉnh phù hợp với màn hình nhỏ hơn
    int buttonWidth = 180;
    int buttonHeight = 50;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;
    int startY = 200;
    int buttonSpacing = 70;

    // Thêm nút Play
    buttons.push_back({playButtonTexture, {centerX, startY, buttonWidth, buttonHeight}, BTN_PLAY});

    // Thêm nút Music (sử dụng texture phù hợp dựa vào trạng thái music)
    buttons.push_back({musicOn ? musicOnButtonTexture : musicOffButtonTexture,
                      {centerX, startY + buttonSpacing, buttonWidth, buttonHeight}, BTN_MUSIC});

    // Thêm nút Leaderboard
    buttons.push_back({leaderboardButtonTexture, {centerX, startY + buttonSpacing * 2, buttonWidth, buttonHeight}, BTN_LEADERBOARD});

    // Thêm nút Quit
    buttons.push_back({quitButtonTexture, {centerX, startY + buttonSpacing * 3, buttonWidth, buttonHeight}, BTN_QUIT});

    // Nút Back cho màn hình bảng xếp hạng
    ImageButton backButton = {backButtonTexture, {centerX, SCREEN_HEIGHT - 100, buttonWidth, buttonHeight}, -1};

    // Dữ liệu bảng xếp hạng mẫu - trong game thực tế, sẽ được tải từ file
    std::vector<ScoreEntry> leaderboard = {
        {"Người chơi 1", 1000},
        {"Người chơi 2", 850},
        {"Người chơi 3", 700},
        {"Người chơi 4", 550},
        {"Người chơi 5", 400}
    };

    // Sử dụng font đã được tải trong SDLUtils
    TTF_Font* menuFont = font; // Sử dụng font toàn cục từ SDLUtils
    if (!menuFont) {
        menuFont = TTF_OpenFont("font2.ttf", 20); // Nếu chưa tải, tải với kích thước nhỏ hơn
        if (!menuFont) {
            printf("Lỗi TTF_OpenFont: %s\n", TTF_GetError());

            // Dọn dẹp textures
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

                // Dọn dẹp textures
                SDL_DestroyTexture(playButtonTexture);
                SDL_DestroyTexture(musicOnButtonTexture);
                SDL_DestroyTexture(musicOffButtonTexture);
                SDL_DestroyTexture(leaderboardButtonTexture);
                SDL_DestroyTexture(quitButtonTexture);
                SDL_DestroyTexture(backButtonTexture);
                SDL_DestroyTexture(leaderboardBgTexture);

                // Chỉ đóng font nếu chúng ta đã tạo mới, không phải font toàn cục
                if (menuFont != font) {
                    TTF_CloseFont(menuFont);
                }

                return false;
            }

            // Xử lý sự kiện chuột
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (showLeaderboard) {
                    // Kiểm tra nếu người dùng nhấp vào nút Back
                    if (mouseX >= backButton.rect.x && mouseX <= backButton.rect.x + backButton.rect.w &&
                        mouseY >= backButton.rect.y && mouseY <= backButton.rect.y + backButton.rect.h) {
                        showLeaderboard = false;
                    }
                } else {
                    // Kiểm tra người dùng nhấp vào nút nào trong menu chính
                    for (auto& button : buttons) {
                        if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
                            mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) {

                            // Xử lý nhấp chuột dựa vào ID nút
                            switch (button.id) {
                                case BTN_PLAY:
                                    // Dọn dẹp trước khi bắt đầu game
                                    SDL_DestroyTexture(playButtonTexture);
                                    SDL_DestroyTexture(musicOnButtonTexture);
                                    SDL_DestroyTexture(musicOffButtonTexture);
                                    SDL_DestroyTexture(leaderboardButtonTexture);
                                    SDL_DestroyTexture(quitButtonTexture);
                                    SDL_DestroyTexture(backButtonTexture);
                                    SDL_DestroyTexture(leaderboardBgTexture);

                                    // Chỉ đóng font nếu chúng ta đã tạo mới, không phải font toàn cục
                                    if (menuFont != font) {
                                        TTF_CloseFont(menuFont);
                                    }

                                    return true; // Bắt đầu trò chơi

                                case BTN_MUSIC:
                                    musicOn = !musicOn;
                                    if (musicOn) {
                                        Mix_PlayMusic(backgroundMusic, -1);
                                        button.texture = musicOnButtonTexture;
                                    } else {
                                        Mix_HaltMusic();
                                        button.texture = musicOffButtonTexture;
                                    }
                                    break;

                                case BTN_LEADERBOARD:
                                    showLeaderboard = true;
                                    break;

                                case BTN_QUIT:
                                    // Dọn dẹp trước khi thoát
                                    SDL_DestroyTexture(playButtonTexture);
                                    SDL_DestroyTexture(musicOnButtonTexture);
                                    SDL_DestroyTexture(musicOffButtonTexture);
                                    SDL_DestroyTexture(leaderboardButtonTexture);
                                    SDL_DestroyTexture(quitButtonTexture);
                                    SDL_DestroyTexture(backButtonTexture);
                                    SDL_DestroyTexture(leaderboardBgTexture);

                                    // Chỉ đóng font nếu chúng ta đã tạo mới, không phải font toàn cục
                                    if (menuFont != font) {
                                        TTF_CloseFont(menuFont);
                                    }

                                    return false; // Thoát game
                            }
                            break; // Thoát vòng lặp sau khi tìm thấy nút được nhấp
                        }
                    }
                }
            }

            // Vẫn giữ khả năng sử dụng phím tắt ESC để thoát
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (showLeaderboard) {
                    showLeaderboard = false;
                } else {
                    isMenu = false;

                    // Dọn dẹp textures
                    SDL_DestroyTexture(playButtonTexture);
                    SDL_DestroyTexture(musicOnButtonTexture);
                    SDL_DestroyTexture(musicOffButtonTexture);
                    SDL_DestroyTexture(leaderboardButtonTexture);
                    SDL_DestroyTexture(quitButtonTexture);
                    SDL_DestroyTexture(backButtonTexture);
                    SDL_DestroyTexture(leaderboardBgTexture);

                    // Chỉ đóng font nếu chúng ta đã tạo mới, không phải font toàn cục
                    if (menuFont != font) {
                        TTF_CloseFont(menuFont);
                    }

                    return false;
                }
            }
        }

        // Cập nhật hoạt ảnh nền - sử dụng hàm có sẵn từ SDLUtils
        updateBackground(bgY1, bgY2);

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Hiển thị nền cuộn - sử dụng hàm có sẵn từ SDLUtils
        renderBackground(renderer, starBackground, bgY1, bgY2);

        if (showLeaderboard) {
            // Hiển thị bảng xếp hạng với kích thước phù hợp
            SDL_Rect leaderboardRect = {SCREEN_WIDTH / 2 - 200, 50, 400, 550};
            SDL_RenderCopy(renderer, leaderboardBgTexture, nullptr, &leaderboardRect);

            // Vẽ tiêu đề
            SDL_Surface* titleSurface = TTF_RenderText_Solid(menuFont, "BẢNG XẾP HẠNG", textColor);
            if (titleSurface) {
                SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
                if (titleTexture) {
                    SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 90, 80, 180, 40};
                    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
                    SDL_DestroyTexture(titleTexture);
                }
                SDL_FreeSurface(titleSurface);
            }

            // Hiển thị điểm số
            for (int i = 0; i < leaderboard.size(); i++) {
                std::string entry = std::to_string(i+1) + ". " + leaderboard[i].name + ": " + std::to_string(leaderboard[i].score);
                SDL_Surface* scoreSurface = TTF_RenderText_Solid(menuFont, entry.c_str(), textColor);
                if (scoreSurface) {
                    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                    if (scoreTexture) {
                        SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - 130, 150 + i * 45, 260, 35};
                        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                        SDL_DestroyTexture(scoreTexture);
                    }
                    SDL_FreeSurface(scoreSurface);
                }
            }

            // Hiển thị nút Back
            SDL_RenderCopy(renderer, backButton.texture, nullptr, &backButton.rect);
        } else {
            // Hiển thị các nút menu
            for (auto& button : buttons) {
                SDL_RenderCopy(renderer, button.texture, nullptr, &button.rect);
            }
        }

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(musicOnButtonTexture);
    SDL_DestroyTexture(musicOffButtonTexture);
    SDL_DestroyTexture(leaderboardButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(leaderboardBgTexture);

    // Chỉ đóng font nếu chúng ta đã tạo mới, không phải font toàn cục
    if (menuFont != font) {
        TTF_CloseFont(menuFont);
    }

    return false;
}
