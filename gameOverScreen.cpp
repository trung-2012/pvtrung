#include "gameOverScreen.h"
#include <string>
#include "HighScore.h"

int showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score,
                       SDL_Texture* starBackground, SDL_Texture* buttonRestartTexture,
                       SDL_Texture* buttonMenuTexture, SDL_Window* window) {
    bool quit = false;
    int result = GO_NONE;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int centerX = windowWidth / 2;
    int restartY = windowHeight / 2 + 50;
    int menuY = windowHeight / 2 + 120;

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color scoreColor = {255, 215, 0, 255};

    std::string scoreText = "Your Score: " + std::to_string(score);

    // Load title font
    TTF_Font* titleFont = TTF_OpenFont("assets/fonts/your_font.ttf", 48);
    if (!titleFont) titleFont = font;

    // Calculate button rects
    SDL_Rect restartRect = {centerX - 100, restartY - 25, 200, 80};
    SDL_Rect menuRect = {centerX - 100, menuY - 25, 200, 80};

    int bgY1 = 0;
    int bgY2 = -windowHeight;


    while (!quit) {
        SDL_Event e;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        bool restartHovered = (mouseX >= restartRect.x && mouseX <= restartRect.x + restartRect.w &&
                               mouseY >= restartRect.y && mouseY <= restartRect.y + restartRect.h);
        bool menuHovered = (mouseX >= menuRect.x && mouseX <= menuRect.x + menuRect.w &&
                            mouseY >= menuRect.y && mouseY <= menuRect.y + menuRect.h);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                result = GO_MENU;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (restartHovered) {
                    result = GO_RESTART;
                    quit = true;
                } else if (menuHovered) {
                    result = GO_MENU;
                    quit = true;
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
                    result = GO_RESTART;
                    quit = true;
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    result = GO_MENU;
                    quit = true;
                }
            }
        }

         // ✅ Update nền cuộn
        bgY1 += 2;
        bgY2 += 2;
        if (bgY1 >= windowHeight) bgY1 = -windowHeight;
        if (bgY2 >= windowHeight) bgY2 = -windowHeight;

        // Vẽ nền cuộn
        SDL_RenderClear(renderer);
        SDL_Rect bgRect1 = {0, bgY1, windowWidth, windowHeight};
        SDL_Rect bgRect2 = {0, bgY2, windowWidth, windowHeight};
        SDL_RenderCopy(renderer, starBackground, NULL, &bgRect1);
        SDL_RenderCopy(renderer, starBackground, NULL, &bgRect2);

        // Draw "GAME OVER"
        SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "GAME OVER", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect = {
            centerX - titleSurface->w / 2,
            windowHeight / 4 - titleSurface->h / 2,
            titleSurface->w,
            titleSurface->h
        };
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        // Draw score
        SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, scoreText.c_str(), scoreColor);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_Rect scoreRect = {
            centerX - scoreSurface->w / 2,
            windowHeight / 3,
            scoreSurface->w,
            scoreSurface->h
        };
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        // Draw buttons
        SDL_RenderCopy(renderer, buttonRestartTexture, NULL, &restartRect);
        SDL_RenderCopy(renderer, buttonMenuTexture, NULL, &menuRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // ✅ Kiểm tra và cập nhật điểm cao nhất sau khi vòng lặp kết thúc
    HighScoreEntry highScore = loadHighScore();
    if (score > highScore.score) {
        highScore.name = "HighScore";  // Bạn có thể thay bằng tên nhập vào nếu muốn
        highScore.score = score;
        saveHighScore(highScore);
    }

    if (titleFont != font) {
        TTF_CloseFont(titleFont);
    }

    return result;
}
