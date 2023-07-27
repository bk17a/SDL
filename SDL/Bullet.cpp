#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet()
{
	velocity = Vector2(0.0f, 0.0f);
	position = Vector2(0.0f, 0.0f);
	renderer = nullptr;
	bulletTex = nullptr;
	size = Vector2(BULLET_SIZE, BULLET_SIZE);
	lastShotTime = 0;
	shootDelay = 2000; // 2 seconds
	active = true;
}

Bullet::Bullet(SDL_Renderer* renderer, TextureManager* bulletTex)
{
	velocity = Vector2(0.0f, 0.0f);
	position = Vector2(0.0f, 0.0f);
	this->renderer = renderer;
	this->bulletTex = bulletTex;
	size = Vector2(BULLET_SIZE, BULLET_SIZE);
	lastShotTime = 0;
	shootDelay = 2000; // 2 seconds
	active = true;
}

void Bullet::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	bulletTex->render2(renderer, position.x - camX, position.y - camY, size.x, size.y);
}

bool Bullet::canShoot() const
{
	const unsigned int currentTime = SDL_GetTicks();
	return (currentTime - lastShotTime >= shootDelay);
}


void Bullet::shoot(const Vector2& playerPos, const Vector2& enemyPos)
{
	position = playerPos;		// starting position at player
	active = true;
	lastShotTime = SDL_GetTicks();
}

void Bullet::reload()
{
	active = false;
}

void Bullet::update()
{
	// Update the positions of all bullets while bullet is active
	if (active)
	{
		position.x += velocity.x;
		position.y += velocity.y;

		if (position.x < -OFFSCREEN_BUFFER || position.x > SCREEN_WIDTH + OFFSCREEN_BUFFER || position.y < -OFFSCREEN_BUFFER || position.y > SCREEN_HEIGHT + OFFSCREEN_BUFFER)
		{
			reload();
		}
	}
}

bool Bullet::isActive() const
{
	return active;
}

unsigned int Bullet::getLastShotTime() const
{
	return lastShotTime;
}

void Bullet::setLastShotTime(const unsigned time)
{
	lastShotTime = time;
}
