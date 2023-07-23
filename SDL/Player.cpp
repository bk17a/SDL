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
	flipType = SDL_FLIP_NONE;
}

void Player::handleEvent(const SDL_Event& e)
{
	// Check if any arrow key is pressed
	const bool isArrowKeyPressed = (e.type == SDL_KEYDOWN && e.key.repeat == 0) &&
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
			yVel = -PLAYER_VEL;
			break;
		case SDLK_DOWN:
			yVel = PLAYER_VEL;
			break;
		case SDLK_LEFT:
			xVel = -PLAYER_VEL;
			flipType = SDL_FLIP_HORIZONTAL;
			break;
		case SDLK_RIGHT:
			xVel = PLAYER_VEL;
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
	if ((xPos < 0) || (xPos > LEVEL_WIDTH + PLAYER1_WIDTH - 200))
	{
		// move back
		xPos -= xVel;
	}

	// player move on y axis
	yPos += yVel;
	// if player out of range
	if ((yPos < 0) || (yPos + PLAYER1_HEIGHT > LEVEL_HEIGHT - 30))
	{
		// move back
		yPos -= yVel;
	}
}

void Player::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
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

void Player::renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, const int camX, const int camY, const double angle, const SDL_Point* center, const SDL_RendererFlip flipType) const  // NOLINT(clang-diagnostic-shadow)
{
	int renderX = xPos - camX;						
	const int renderY = yPos - camY;
	if (flipType == SDL_FLIP_HORIZONTAL)
	{
		renderX += PLAYER1_WIDTH - 200;	// flip in place
	}
	playerTexture->render(renderX, renderY, renderer, clip, angle, center, flipType);
}

bool Player::isMoving() const
{
	return moving;
}

SDL_RendererFlip Player::getFlipType() const
{
	return flipType;
}