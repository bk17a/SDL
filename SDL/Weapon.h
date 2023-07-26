#ifndef WEAPON_H
#define WEAPON_H

#include <SDL.h>
#include <vector>
#include "TextureManager.h"
#include "Vector2.h"

class Bullet
{		
public:
	Bullet();
	Bullet(SDL_Renderer* renderer, TextureManager* bulletTex);
	void render(SDL_Renderer* renderer, const int camX, const int camY) const;
	void shoot(int enemyX, int enemyY);		// shoot at direction of the enemy
	void update();
	bool isActive() const;

	void setPosX(int x);
	void setPosY(int y);
	int getPosX() const;
	int getPosY() const;

private:
	Vector2 velocity;		// speed of the bullet
	Vector2 position;		// inital position where the bullet starts from
	Vector2 size;			// size of bullet
		
	SDL_Renderer* renderer;
	TextureManager* bulletTex;
	std::vector<Bullet> bullets;

	Uint32 lastShotTime;	// time of last shot
	Uint32 shootDelay;		// time between shots in ms

	bool active;
	int OFFSCREEN_BUFFER = 10; // pixels outside screen to delete bullet
};

#endif // WEAPON_H