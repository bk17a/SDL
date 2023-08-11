// ReSharper disable CppClangTidyClangDiagnosticShadow
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

	moving = false;
	flipType = SDL_FLIP_NONE;
}

void Enemy::render(SDL_Renderer* renderer, const float camX, const float camY) const
{
	const int renderX = static_cast<int>(position.x - camX);
	const int renderY = static_cast<int>(position.y - camY);
	enemyTex->render2(renderer, renderX, renderY, ENEMY_WIDTH, ENEMY_HEIGHT);
}

void Enemy::renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, const float camX, const float camY, const double angle, const SDL_Point* center, const SDL_RendererFlip flipType) const
{
	int renderX = static_cast<int>(position.x - camX);
	const int renderY = static_cast<int>(position.y - camY);
	if (flipType == SDL_FLIP_HORIZONTAL)
	{
		renderX += ENEMY1_WIDTH - 270;	// flip with offset
	}
	enemyTex->render(renderX, renderY, renderer, clip, angle, center, flipType);
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

void Enemy::setPosX(const float xPos)
{
	position.x = xPos;
	p.x = static_cast<int>(xPos);			// update p.x to update new pos
}

void Enemy::setPosY(const float yPos)
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

void Enemy::setVelocityX(const float xVel)
{
	velocity.x = xVel;
}

void Enemy::setVelocityY(const float yVel)
{
	velocity.y = yVel;
}

void Enemy::setVelocity(const Vector2 vec)
{
	velocity = vec;
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

void Enemy::setRect(const SDL_Rect rect)
{
	this->rect = rect;
}

bool Enemy::isMoving() const
{
	return moving;
}

void Enemy::setMoving(const bool flag)
{
	moving = flag;
}

void Enemy::move(Enemy& enemy, const Player& player)
{
	// if (enemy.getPosX() < player.getXPos()) enemy.setPosX(enemy.getPosX() + ENEMY_VEL);
	// if (enemy.getPosX() > player.getXPos()) enemy.setPosX(enemy.getPosX() - ENEMY_VEL);
	// if (enemy.getPosY() < player.getYPos()) enemy.setPosY(enemy.getPosY() + ENEMY_VEL);
	// if (enemy.getPosY() > player.getYPos()) enemy.setPosY(enemy.getPosY() - ENEMY_VEL);
	bool moved = false; // Flag to track if any movement occurred

	if (enemy.getPosX() < player.getXPos())
	{
		enemy.setPosX(enemy.getPosX() + ENEMY_VEL);
		moved = true;
		enemy.setFlipType(SDL_FLIP_NONE); // Not flipped when moving right
	}
	if (enemy.getPosX() > player.getXPos())
	{
		enemy.setPosX(enemy.getPosX() - ENEMY_VEL);
		moved = true;
		enemy.setFlipType(SDL_FLIP_HORIZONTAL); // Flipped when moving left
	}
	if (enemy.getPosY() < player.getYPos())
	{
		enemy.setPosY(enemy.getPosY() + ENEMY_VEL);
		moved = true;
	}
	if (enemy.getPosY() > player.getYPos())
	{
		enemy.setPosY(enemy.getPosY() - ENEMY_VEL);
		moved = true;
	}

	enemy.setMoving(moved); // Set moving flag based on whether any movement occurred
}

void Enemy::setFlipType(const SDL_RendererFlip flipType)
{
	this->flipType = flipType;
}

SDL_RendererFlip Enemy::getFlipType() const
{
	return flipType;
}