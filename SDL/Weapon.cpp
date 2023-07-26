#include "Weapon.h"
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
	active = false;
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
	active = false;
}

void Bullet::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	bulletTex->render2(renderer, position.x - camX, position.y, size.x, size.y);
}


void Bullet::shoot(const int enemyX, const int enemyY)
{
	// const Uint32 currentTime = SDL_GetTicks();
	// if (currentTime - lastShotTime >= shootDelay)
	// {
	// 	// create new bullet at player pos
	// 	Bullet toShoot(renderer, bulletTex);
	// 	toShoot.initPosX = enemyX;
	// 	toShoot.initPosY = enemyY;
	// 	// set bullet speed
	// 	toShoot.xVel = BULLET_SPEED;
	// 	toShoot.yVel = BULLET_SPEED;
	// 	active = true;
	// 	bullets.emplace_back(toShoot);
	//
	// 	// update last shot time
	// 	lastShotTime = currentTime;
	// }

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
			active = false;
		}
	}
}

void Bullet::setPosX(const int x)
{
	position.x = x;
}

void Bullet::setPosY(const int y)
{
	position.y = y;
}


int Bullet::getPosX() const
{
	return position.x;
}

int Bullet::getPosY() const
{
	return position.y;
}

bool Bullet::isActive() const
{
	return active;
}
