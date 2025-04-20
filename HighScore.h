#pragma once
#include <string>

struct HighScoreEntry {
    std::string name;
    int score;
};

HighScoreEntry loadHighScore();
void saveHighScore(const HighScoreEntry& entry);

