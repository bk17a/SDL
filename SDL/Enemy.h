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

	void render(SDL_Renderer* renderer, const float camX, const float camY) const;
	void spawn();
	bool isAlive() const;

	void takeDamage(int damage);
	void kill();

	float getPosX() const;
	float getPosY() const;
	float getWidth() const;
	float getHeight() const;
	Vector2 getEnemyPos() const;
	void setPosX(float xPos);
	void setPosY(float yPos);
	void setVelocityX(float xVel);
	void setVelocityY(float yVel);

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
