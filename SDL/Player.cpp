#include "Player.h"

Player::Player()
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
	moving = false;
	renderer = nullptr;
	playerTexture = nullptr;
	angle = 0;
	center.x = PLAYER2_WIDTH / 2;
	center.y = PLAYER2_HEIGHT / 2;
	flipType = SDL_FLIP_NONE;
}

Player::Player(SDL_Renderer* renderer, TextureManager* playerTexture)
{
	xPos = 0;
	yPos = 0;
	xVel = 0;
	yVel = 0;
	moving = false;
	this->renderer = renderer;
	this->playerTexture = playerTexture;
	angle = 0;
	center.x = PLAYER2_WIDTH / 2;
	center.y = PLAYER2_HEIGHT / 2;
	flipType = SDL_FLIP_NONE;
}

void Player::handleEvent(const SDL_Event& e)
{
	// Check if any arrow key is pressed
	bool isArrowKeyPressed = (e.type == SDL_KEYDOWN && e.key.repeat == 0) &&
		(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN ||
			e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT);

	// Set the moving flag based on whether any arrow key is pressed or not
	if (isArrowKeyPressed)
	{
		moving = true;
		// Adjust the velocity based on the pressed key
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
			flipType = SDL_FLIP_HORIZONTAL;
			break;
		case SDLK_RIGHT:
			xVel = DOT_VEL;
			flipType = SDL_FLIP_NONE;
			break;
		default:
			break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		// Adjust the velocity when a key is released
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
		case SDLK_DOWN:
			yVel = 0;
			break;
		case SDLK_LEFT:
		case SDLK_RIGHT:
			xVel = 0;
			break;
		default:
			break;
		}

		// Check if no arrow key is pressed, then the player is not moving
		moving = (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP] ||
			SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DOWN] ||
			SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] ||
			SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT]);
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
	playerTexture->render(xPos - camX, yPos - camY, renderer);
}

int Player::getXPos() const
{
	return xPos;
}

int Player::getYPos() const
{
	return yPos;
}

void Player::renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, int camX, int camY, double angle, SDL_Point center, SDL_RendererFlip flipType)
{
	playerTexture->render(xPos - camX, yPos - camY, renderer, clip, angle, &center, flipType);
}

bool Player::isMoving() const
{
	return moving;
}

SDL_RendererFlip Player::getFlipType() const
{
	return flipType;
}

double Player::getAngle() const
{
	return angle;
}

SDL_Point Player::getCenter() const
{
	return center;
}