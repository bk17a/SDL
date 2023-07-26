// ReSharper disable CppUnusedIncludeDirective
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "TextureManager.h"
#include "Constants.h"
#include "Vector2.h"

class Player;

class Enemy
{
public:
	SDL_Rect p{};

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
	void setVelocityX(int xVel);
	void setVelocityY(int yVel);

	bool checkCollisionWithEnemy(const Enemy& e) const;

private:
	SDL_Renderer* renderer;
	TextureManager* enemyTex;

	Vector2 position;
	Vector2 size;
	Vector2 velocity;

	int health;
	bool alive;
	std::vector<Enemy> enemies;
};

#endif
