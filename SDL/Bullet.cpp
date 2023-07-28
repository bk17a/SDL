#include "Bullet.h"

Bullet::Bullet(const Vector2 position, const Vector2 velocity, const Vector2 size, SDL_Renderer* renderer, TextureManager* texture)
	: pos(position), vel(velocity), size(size)
{
	this->renderer = renderer;
	this->texture = texture;
	active = false;
};

void Bullet::reload()
{
	active = false;
}

void Bullet::shoot(const Vector2 playerPos)
{
	pos = playerPos;
	vel = Vector2::ZERO_VEC();
	active = true;
}

void Bullet::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	texture->render2(renderer, pos.x - camX, pos.y - camY, size.x, size.y);
}
