#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet() = default;  // NOLINT(cppcoreguidelines-pro-type-member-init)

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

void Bullet::render(SDL_Renderer* renderer, const float camX, const float camY) const  // NOLINT(clang-diagnostic-shadow)
{
	const int renderX = static_cast<int>(pos.x - camX);
	const int renderY = static_cast<int>(pos.y - camY);
	texture->render2(renderer, renderX, renderY, static_cast<int>(size.x), static_cast<int>(size.y));
}

void Bullet::handleEvent(const SDL_Event& e, const Vector2 playerPos)
{
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
	{
		active = !active;
		if (active)
		{
			shoot(playerPos);
		}
	}
}

void Bullet::update()
{
	Vector2 dir = targetPos - pos;
	dir.normalize();

	vel = dir * BULLET_SPEED;	// multiply for bullet speed

	pos += vel;

	// check if bullet went out of screen
	if (pos.x > static_cast<float>(LEVEL_WIDTH) + OFFSCREEN_BUFFER || pos.x < 0.0f - OFFSCREEN_BUFFER || 
		pos.y > static_cast<float>(LEVEL_HEIGHT) + OFFSCREEN_BUFFER || pos.y < 0.0f - OFFSCREEN_BUFFER)
	{
		reload();
	}

}

bool Bullet::isActive() const
{
	return active;
}

void Bullet::setTargetPos(const Vector2& targetPos)  // NOLINT(clang-diagnostic-shadow)
{
	this->targetPos = targetPos;
}