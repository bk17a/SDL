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
	void handleEvent(const SDL_Event& e);										// handle key presses and adjust dot's velocity
	//void handleEvent2(SDL_Event& e);
	void move();														// moves the player
	void render(SDL_Renderer* renderer, int camX, int camY);			// render player onto screen relative to camera

	int getXPos() const;
	int getYPos() const;

	void renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, int camX, int camY);

private:
	// x and y offsets
	int xPos, xPos2;
	int yPos, yPos2; 

	// velocity of player
	int xVel, xVel2;
	int yVel, yVel2; 

	SDL_Renderer* renderer;
	TextureManager* playerTexture;
};

#endif //PLAYER_H_