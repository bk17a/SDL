// ReSharper disable CppUnusedIncludeDirective
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <algorithm>
#include "TextureManager.h"
#include "Constants.h"
#include "Vector2.h"
#include "Bullet.h"

class Player;

class Enemy
{
public:
	SDL_Rect p{};

	Enemy();
	Enemy(SDL_Renderer* renderer, TextureManager* enemyTex);

	void render(SDL_Renderer* renderer, float camX, float camY) const;
	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, float camX, float camY, double angle, const SDL_Point* center, SDL_RendererFlip flipType) const;
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
	void setVelocity(Vector2 vec);

	bool checkCollisionWith(const SDL_Rect& rect) const;
	void setRect(SDL_Rect rect);

	bool isMoving() const;
	void setMoving(bool flag);
	static void move(Enemy& enemy, const Player& player);
	void setFlipType(SDL_RendererFlip flipType);
	SDL_RendererFlip getFlipType() const;

	void setExpPoints(int exp);
	int getExpPoints() const;

private:
	SDL_Renderer* renderer;
	TextureManager* enemyTex;

	Vector2 position;
	Vector2 size;
	Vector2 velocity;

	int health;
	bool alive;
	std::vector<Enemy> enemies;

	Bullet bullet;

	bool moving;

	SDL_Rect rect{};
	SDL_RendererFlip flipType;
	int expPoints;
};

#endif
