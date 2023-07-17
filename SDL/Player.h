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
	void handleEvent(SDL_Event& e);										// handle key presses and adjust dot's velocity
	void move();														// moves the player
	void render(SDL_Renderer* renderer, int camX, int camY);			// render player onto screen relative to camera

	int getXPos() const;
	int getYPos() const;

private:
	int xPos, yPos; // x and y offsets
	int xVel, yVel; // velocity of player
	SDL_Renderer* renderer;
	TextureManager* playerTexture;

};

#endif //PLAYER_H_