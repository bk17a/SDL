#ifndef BULLET_H_
#define BULLET_H_

#include <SDL.h>
#include "Vector2.h"
#include "TextureManager.h"

class Bullet
{
public:
	SDL_Rect p{};
	Bullet();
	Bullet(Vector2 position, Vector2 velocity, Vector2 size, SDL_Renderer* renderer, TextureManager* texture);
	void reload();
	void shoot(Vector2 playerPos);
	void render(SDL_Renderer* renderer, const float camX, const float camY) const;
	void handleEvent(const SDL_Event& e, Vector2 playerPos);
	void update();
	void setTargetPos(const Vector2& targetPos);

	bool isActive() const;
	void setActive(bool active);
	Vector2 getPos() const;
	void setPos(Vector2 pos);

private:
	Vector2 pos;
	Vector2 vel;
	Vector2 size;
	Vector2 targetPos;

	SDL_Renderer* renderer;
	TextureManager* texture;

	bool active;
	float OFFSCREEN_BUFFER = 10;
};

#endif 