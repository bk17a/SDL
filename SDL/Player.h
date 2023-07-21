// ReSharper disable CppUnusedIncludeDirective
#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include "TextureManager.h"
#include "Constants.h"

class Player
{
public:

	// initialize the variables
	Player();
	Player(SDL_Renderer* renderer, TextureManager* playerTexture);
	void handleEvent(const SDL_Event& e);								// handle key presses and adjust dot's velocity
	void move();														// moves the player
	void render(SDL_Renderer* renderer, int camX, int camY) const;		// render player onto screen relative to camera

	int getXPos() const;
	int getYPos() const;
	bool isMoving() const;

	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, int camX, int camY, const double angle = NULL, const SDL_Point* center = nullptr, SDL_RendererFlip flipType = SDL_FLIP_NONE) const;

	SDL_RendererFlip getFlipType() const;

private:
	// x and y offsets
	int xPos;
	int yPos;

	// velocity of player
	int xVel;
	int yVel;

	bool moving;

	SDL_Renderer* renderer;
	TextureManager* playerTexture;

	SDL_RendererFlip flipType;
};

#endif //PLAYER_H_