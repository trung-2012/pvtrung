#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

//vien đạn
struct Bullet {
    int x, y;
    int speed = 5;  // Tốc độ đạn
};

//Đạn địch
struct BulletEnemy {
    int x, y;
    int speed = 2;
    float angle; // Góc bắn (đơn vị: độ)
};
// Máy bay địch
struct Enemy {
    int x, y;
    int speed = 2; // Tốc độ di chuyển của máy bay địch
    Uint32 lastShotTime; // Thời gian lần cuối máy bay địch bắn đạn
};

#endif // BULLET_H
