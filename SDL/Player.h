// ReSharper disable CppUnusedIncludeDirective
#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include "TextureManager.h"
#include "Constants.h"
#include "Enemy.h"
#include "Vector2.h"

class Player
{
public:
	// initialize the variables
	SDL_Rect p{};

	Player();
	Player(SDL_Renderer* renderer, TextureManager* playerTexture);
	void handleEvent(const SDL_Event& e);								// handle key presses and adjust dot's velocity
	void move();														// moves the player
	void render(SDL_Renderer* renderer, float camX, float camY) const;		// render player onto screen relative to camera

	// getters and setters
	float getXPos() const;
	float getYPos() const;
	float getWidth() const;
	float getHeight() const;
	Vector2 getPlayerPos() const;
	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, float camX, float camY, const double angle = NULL, const SDL_Point* center = nullptr, SDL_RendererFlip flipType = SDL_FLIP_NONE) const;

	void setXPos(float x);
	void setYPos(float y);

	bool isMoving() const;
	SDL_RendererFlip getFlipType() const;
	void spawn();
	void setHp(int hp);
	void takeDamage(int damage);
	void kill();
	bool isAlive() const;
	void setAlive(bool alive);

	int getHp() const;
	int getHpMax() const;
	void update();

	void setScore(int score);
	int getScore() const;
	void increaseScore(int points);
	void resetScore();
	void resetHealth();

private:
	// x and y offsets
	Vector2 position;

	// width and height of player
	Vector2 size;

	// speed of player
	Vector2 velocity;

	// health and status
	int hp, hpMax;
	bool alive;

	int collisionCooldown;

	bool moving;

	SDL_Renderer* renderer;
	TextureManager* playerTexture;

	SDL_RendererFlip flipType;

	int score;
};

#endif //PLAYER_H_