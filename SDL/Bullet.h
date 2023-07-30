#ifndef BULLET_H_
#define BULLET_H_

#include <SDL.h>
#include "Vector2.h"
#include "TextureManager.h"

class Bullet
{
public:
	Bullet();
	Bullet(Vector2 position, Vector2 velocity, Vector2 size, SDL_Renderer* renderer, TextureManager* texture);
	void reload();
	void shoot(Vector2 playerPos);
	void render(SDL_Renderer* renderer, const int camX, const int camY) const;
	void handleEvent(const SDL_Event& e, Vector2 playerPos);
	void update();
	void setTargetPos(const Vector2& targetPos);

	bool isActive() const;

private:
	Vector2 pos;
	Vector2 vel;
	Vector2 size;
	Vector2 targetPos;

	SDL_Renderer* renderer;
	TextureManager* texture;

	bool active;
	int OFFSCREEN_BUFFER = 10;
};

#endif 