// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
// ReSharper disable CppClangTidyClangDiagnosticDoublePromotion
// ReSharper disable CppClangTidyClangDiagnosticFloatConversion
#include "Enemy.h"

Enemy::Enemy()
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
	renderer = nullptr;
	enemyTex = nullptr;
	health = 100;
	alive = false;
}

Enemy::Enemy(SDL_Renderer* renderer, TextureManager* enemyTex)
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
	this->renderer = renderer;
	this->enemyTex = enemyTex;
	health = 100;
	alive = false;
}

void Enemy::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	enemyTex->render2(renderer, xPos - camX, yPos - camY, ENEMY_WIDTH, ENEMY_HEIGHT);
}

void Enemy::spawn()
{
	// Set random initial position within the screen boundaries
	xPos = rand() % (SCREEN_WIDTH - ENEMY_WIDTH);	 // NOLINT(concurrency-mt-unsafe)
	yPos = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT);  // NOLINT(concurrency-mt-unsafe)

	alive = true;
}

bool Enemy::isAlive() const
{
	return alive;
}

void Enemy::takeDamage(const int damage)
{
	health -= damage;
	if (health <= 0)
	{
		kill();
	}
}

void Enemy::kill()
{
	alive = false;
}

int Enemy::getPosX() const
{
	return xPos;
}

int Enemy::getPosY() const
{
	return yPos;
}

void Enemy::setPosX(const int xPos)  // NOLINT(clang-diagnostic-shadow)
{
	this->xPos = xPos;
}

void Enemy::setPosY(const int yPos) // NOLINT(clang-diagnostic-shadow)
{
	this->yPos = yPos;
}