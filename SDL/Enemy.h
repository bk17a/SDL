// ReSharper disable CppUnusedIncludeDirective
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <stdlib.h>
#include "TextureManager.h"
#include "Player.h"

class Enemy
{
public:
	Enemy();
	Enemy(SDL_Renderer* renderer, TextureManager* enemyTex);

	void render(SDL_Renderer* renderer) const;
	void spawn();
	bool isAlive() const;

	void takeDamage(int amount);
	void kill();

private:
	SDL_Renderer* renderer;
	TextureManager* enemyTex;

	int xPos, yPos;
	int xVel, yVel;

	int health;
	bool alive;
};

#endif
	