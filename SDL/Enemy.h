// ReSharper disable CppUnusedIncludeDirective
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "TextureManager.h"
#include "Player.h"

class Enemy
{
public:
	Enemy();
	Enemy(SDL_Renderer* renderer, TextureManager* enemyTex);

	void render(SDL_Renderer* renderer, const int camX, const int camY) const;
	void spawn();
	bool isAlive() const;

	void takeDamage(int damage);
	void kill();

	int getPosX() const;
	int getPosY() const;
	int getWidth() const;
	int getHeight() const;
	void setPosX(int xPos);
	void setPosY(int yPos);

	bool checkCollisionWithEnemy(const Enemy& e) const;
	//bool checkCollisionWithPlayer(const Player& e) const;


private:
	SDL_Renderer* renderer;
	TextureManager* enemyTex;

	int xPos, yPos;
	int width, height;
	double xVel, yVel;

	int health;
	bool alive;
	std::vector<Enemy> enemies;

	SDL_Rect p{};
};

#endif
