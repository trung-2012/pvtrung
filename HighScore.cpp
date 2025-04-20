#include "HighScore.h"
#include <fstream>
#include <iostream>
HighScoreEntry loadHighScore() {
    std::ifstream file("highscore.txt");
    HighScoreEntry entry;

    if (!file.is_open()) {
        // Nếu file không tồn tại, tạo file mới với điểm mặc định là 0
        std::ofstream out("highscore.txt");
        out << "Player 0\n";
        out.close();
        entry.name = "Player";
        entry.score = 0;
    } else {
        file >> entry.name >> entry.score;
        file.close();
    }

    return entry;
}


void saveHighScore(const HighScoreEntry& entry) {
    std::ofstream file("highscore.txt");
    file << entry.name << " " << entry.score << "\n";
    file.close();
}

