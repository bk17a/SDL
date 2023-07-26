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
	Player();
	Player(SDL_Renderer* renderer, TextureManager* playerTexture);
	void handleEvent(const SDL_Event& e);								// handle key presses and adjust dot's velocity
	void move();														// moves the player
	void render(SDL_Renderer* renderer, int camX, int camY) const;		// render player onto screen relative to camera

	// getters and setters
	int getXPos() const;
	int getYPos() const;
	int getWidth() const;
	int getHeight() const;
	void setXPos(int x);
	void setYPos(int y);

	bool isMoving() const;
	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, int camX, int camY, const double angle = NULL, const SDL_Point* center = nullptr, SDL_RendererFlip flipType = SDL_FLIP_NONE) const;
	SDL_RendererFlip getFlipType() const;

private:
	// x and y offsets
	Vector2 position;
	// width and height of player
	Vector2 size;
	// speed of player
	Vector2 velocity;

	bool moving;

	SDL_Renderer* renderer;
	TextureManager* playerTexture;

	SDL_RendererFlip flipType;

	// player's collision box
	SDL_Rect collider{};
};

#endif //PLAYER_H_