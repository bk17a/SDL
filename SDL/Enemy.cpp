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

void Enemy::render(SDL_Renderer* renderer) const  // NOLINT(clang-diagnostic-shadow)
{
	enemyTex->render2(renderer, xPos, yPos, ENEMY_WIDTH, ENEMY_HEIGHT);
}

void Enemy::spawn()
{
	// Set random initial position within the screen boundaries
	xPos = rand() % (SCREEN_WIDTH - ENEMY_WIDTH); // Generate a random X position between 0 and (SCREEN_WIDTH - width)  // NOLINT(concurrency-mt-unsafe)
	yPos = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT); // Generate a random Y position between 0 and (SCREEN_HEIGHT - height) // NOLINT(concurrency-mt-unsafe)

	// Debug output to check random positions
	std::cout << "Spawned at (" << xPos << ", " << yPos << ")\n";

	// Set other properties as needed
	alive = true;
}

bool Enemy::isAlive() const
{
	return alive;
}

void Enemy::takeDamage(const int amount)
{
	health -= amount;
	if (health <= 0)
	{
		kill();
	}
}

void Enemy::kill()
{
	alive = false;
}

