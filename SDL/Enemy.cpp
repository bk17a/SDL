// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
// ReSharper disable CppClangTidyClangDiagnosticDoublePromotion
// ReSharper disable CppClangTidyClangDiagnosticFloatConversion
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
	position = Vector2(0, 0);
	size = Vector2(ENEMY_WIDTH, ENEMY_HEIGHT);
	velocity = Vector2(0, 0);
	renderer = nullptr;
	enemyTex = nullptr;
	health = 100;
	alive = false;

	// Initialize SDL_Rect p
	p.x = position.x;
	p.y = position.y;
	p.w = size.x;
	p.h = size.y;
}

Enemy::Enemy(SDL_Renderer* renderer, TextureManager* enemyTex)
{
	position = Vector2(0, 0);
	size = Vector2(ENEMY_WIDTH, ENEMY_HEIGHT);
	velocity = Vector2(0, 0);
	this->renderer = renderer;
	this->enemyTex = enemyTex;
	health = 100;
	alive = false;

	// Initialize the SDL_Rect p with the correct values
	p.x = position.x;
	p.y = position.y;
	p.w = size.x;
	p.h = size.y;
}

void Enemy::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	enemyTex->render2(renderer, position.x - camX, position.y - camY, ENEMY_WIDTH, ENEMY_HEIGHT);
}

void Enemy::spawn()
{
	// Set random initial position within the screen boundaries
	position.x = rand() % (SCREEN_WIDTH - ENEMY_WIDTH);	 // NOLINT(concurrency-mt-unsafe)
	position.y = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT);  // NOLINT(concurrency-mt-unsafe)

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
	return position.x;
}

int Enemy::getPosY() const
{
	return position.y;
}

void Enemy::setPosX(const int xPos)  // NOLINT(clang-diagnostic-shadow)
{
	position.x = xPos;
	p.x = xPos;			// update p.x to update new pos
}

void Enemy::setPosY(const int yPos) // NOLINT(clang-diagnostic-shadow)
{
	position.y = yPos;
	p.y = yPos;			// update p.y to update new pos
}

int Enemy::getHeight() const
{
	return  size.y;
}

int Enemy::getWidth() const
{
	return  size.x;
}

bool Enemy::checkCollisionWithEnemy(const Enemy& e) const
{
	// calculate sides of enemy1
	const int leftA = p.x;
	const int rightA = p.x + p.w;
	const int topA = p.y;
	const int botA = p.y + p.h;

	// calculate sides of enemy2
	const int leftB = e.p.x;
	const int rightB = e.p.x + e.p.w;
	const int topB = e.p.y;
	const int botB = e.p.y + e.p.h;

	// check if any sides from A is not colliding with B
	// not colliding if sides of A are outside of B
	if (botA <= topB || topA >= botB || leftA >= rightB || (rightA <= leftB))
	{
		return false;
	}

	// if no sides are outside of B
	return true;
}

void Enemy::setVelocityX(const int xVel) // NOLINT(clang-diagnostic-shadow)
{
	velocity.x = xVel;
}

void Enemy::setVelocityY(const int yVel)  // NOLINT(clang-diagnostic-shadow)
{
	velocity.y = yVel;
}