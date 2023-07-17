#include "Player.h"

Player::Player() 
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
}

Player::Player(SDL_Renderer* renderer, TextureManager* playerTexture)
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
	this->renderer = renderer;
	this->playerTexture = playerTexture;
}

void Player::handleEvent(SDL_Event& e)
{
	// if a key is pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		// adjusting the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			yVel = -DOT_VEL;
			break;
		case SDLK_DOWN:
			yVel = DOT_VEL;
			break;
		case SDLK_LEFT:
			xVel = -DOT_VEL;
			break;
		case SDLK_RIGHT:
			xVel = DOT_VEL;
			break;
		default:
			break;
		}
	}
	// adjusting velocity when a key is released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		// adjusting the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			yVel = 0;
			break;
		case SDLK_DOWN:
			yVel = 0;
			break;
		case SDLK_LEFT:
			xVel = 0;
			break;
		case SDLK_RIGHT:
			xVel = 0;
			break;
		default:
			break;
		}
	}
}


void Player::move()
{
	// player move on x axis
	xPos += xVel;
	// if player went out of range
	if ((xPos < 0) || (xPos > LEVEL_WIDTH + PLAYER_WIDTH - 100))
	{
		// move back
		xPos -= xVel;
	}

	// player move on y axis
	yPos += yVel;
	// if player out of range
	if ((yPos < 0) || (yPos + PLAYER_HEIGHT > LEVEL_HEIGHT))
	{
		// move back
		yPos -= yVel;
	}

}

void Player::render(SDL_Renderer* renderer, int camX, int camY)
{
	playerTexture->render(xPos - camX, yPos - camY, PLAYER_WIDTH, PLAYER_HEIGHT, renderer);
}

int Player::getXPos() const
{
	return xPos;
}

int Player::getYPos() const
{
	return yPos;
}