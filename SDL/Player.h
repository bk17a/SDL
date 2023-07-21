#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include <iostream>
#include <iomanip>
#include <SDL_ttf.h>
#include <SDL_image.h>
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
	void render(SDL_Renderer* renderer, int camX, int camY);			// render player onto screen relative to camera

	int getXPos() const;
	int getYPos() const;
	bool isMoving() const;

	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, int camX, int camY, double angle, SDL_Point center, SDL_RendererFlip flipType);

	SDL_RendererFlip getFlipType() const;
	double getAngle() const;
	SDL_Point getCenter() const;
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
	double angle;
	SDL_Point center;
};

#endif //PLAYER_H_