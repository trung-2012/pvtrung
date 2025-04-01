#ifndef ACTION_H
#define ACTION_H

#include <SDL.h>
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void drawPlane(SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y);
void drawPlaneEnemy(SDL_Renderer* renderer, SDL_Texture* planeTexture, int x, int y);
void drawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, int x, int y);
void drawBulletEnemy(SDL_Renderer* renderer, SDL_Texture* bulletEnemyTexture, int x, int y);
void handleEvents(bool& running, int& planeX, int& planeY);
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);


#endif // ACTION_H

