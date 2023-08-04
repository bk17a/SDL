#include "Enemy.h"
#include "Player.h"

Enemy::Enemy() = default; // NOLINT(cppcoreguidelines-pro-type-member-init)

Enemy::Enemy(SDL_Renderer* renderer, TextureManager* enemyTex)
{
	position = Vector2(0.f, 0.f);
	size = Vector2(ENEMY_WIDTH, ENEMY_HEIGHT);
	velocity = Vector2(0.f, 0.f);
	this->renderer = renderer;
	this->enemyTex = enemyTex;
	health = 100;
	alive = false;

	// Initialize the SDL_Rect p with the correct values
	p.x = static_cast<int>(position.x);
	p.y = static_cast<int>(position.y);
	p.w = static_cast<int>(size.x);
	p.h = static_cast<int>(size.y);
}

void Enemy::render(SDL_Renderer* renderer, const float camX, const float camY) const  // NOLINT(clang-diagnostic-shadow)
{
	const int renderX = static_cast<int>(position.x - camX);
	const int renderY = static_cast<int>(position.y - camY);
	enemyTex->render2(renderer, renderX, renderY, ENEMY_WIDTH, ENEMY_HEIGHT);
}

void Enemy::spawn()
{
	// Set random initial position within the screen boundaries
	position.x = static_cast<float>(rand() % (SCREEN_WIDTH - ENEMY_WIDTH));	 // NOLINT(concurrency-mt-unsafe)
	position.y = static_cast<float>(rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT));  // NOLINT(concurrency-mt-unsafe)

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

float Enemy::getPosX() const
{
	return position.x;
}

float Enemy::getPosY() const
{
	return position.y;
}

void Enemy::setPosX(const float xPos)  // NOLINT(clang-diagnostic-shadow)
{
	position.x = xPos;
	p.x = static_cast<int>(xPos);			// update p.x to update new pos
}

void Enemy::setPosY(const float yPos) // NOLINT(clang-diagnostic-shadow)
{
	position.y = yPos;
	p.y = static_cast<int>(yPos);			// update p.y to update new pos
}

float Enemy::getHeight() const
{
	return  size.y;
}

float Enemy::getWidth() const
{
	return  size.x;
}

Vector2 Enemy::getEnemyPos() const
{
	return position;
}

void Enemy::setVelocityX(const float xVel) // NOLINT(clang-diagnostic-shadow)
{
	velocity.x = xVel;
}

void Enemy::setVelocityY(const float yVel)  // NOLINT(clang-diagnostic-shadow)
{
	velocity.y = yVel;
}

bool Enemy::checkCollisionWith(const SDL_Rect& rect) const
{
	// Calculate sides of the enemy (current enemy)
	const auto leftA = static_cast<float>(p.x);
	const auto rightA = static_cast<float>(p.x + p.w);
	const auto topA = static_cast<float>(p.y);
	const auto botA = static_cast<float>(p.y + p.h);

	// Calculate sides of the other SDL_Rect
	const auto leftB = static_cast<float>(rect.x);
	const auto rightB = static_cast<float>(rect.x + rect.w);
	const auto topB = static_cast<float>(rect.y);
	const auto botB = static_cast<float>(rect.y + rect.h);

	// Check if the rectangles are overlapping on both axes
	const bool xOverlap = (leftA < rightB) && (rightA > leftB);
	const bool yOverlap = (topA < botB) && (botA > topB);

	// If both axes are overlapping, the two objects are colliding
	return xOverlap && yOverlap;
}