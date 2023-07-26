#include "Player.h"

Player::Player()
{
	position = Vector2(0, 0);
	size = Vector2(PLAYER1_WIDTH, PLAYER1_HEIGHT);
	velocity = Vector2(0, 0);
	moving = false;
	renderer = nullptr;
	playerTexture = nullptr;
	flipType = SDL_FLIP_NONE;
	collider.w = PLAYER1_WIDTH;
	collider.h = PLAYER1_HEIGHT;
}

Player::Player(SDL_Renderer* renderer, TextureManager* playerTexture)
{
	position = Vector2(0, 0);
	size = Vector2(PLAYER1_WIDTH, PLAYER1_HEIGHT);
	velocity = Vector2(0, 0);
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
			velocity.y = -PLAYER_VEL;
			break;
		case SDLK_DOWN:
			velocity.y = PLAYER_VEL;
			break;
		case SDLK_LEFT:
			velocity.x = -PLAYER_VEL;
			flipType = SDL_FLIP_HORIZONTAL;
			break;
		case SDLK_RIGHT:
			velocity.x = PLAYER_VEL;
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
			velocity.y = 0;
			break;
		case SDLK_LEFT:
		case SDLK_RIGHT:
			velocity.x = 0;
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
	position.x += velocity.x;

	// if player went out of range
	if ((position.x < 0) || (position.x > LEVEL_WIDTH + PLAYER1_WIDTH - 200))
	{
		// move back
		position.x -= velocity.x;
	}

	// player move on y axis
	position.y += velocity.y;

	// if player out of range
	if ((position.y < 0) || (position.y + PLAYER1_HEIGHT > LEVEL_HEIGHT - 30))
	{
		// move back
		position.y -= velocity.y;
	}
}

void Player::render(SDL_Renderer* renderer, const int camX, const int camY) const  // NOLINT(clang-diagnostic-shadow)
{
	playerTexture->render(position.x - camX, position.y - camY, renderer);
}

int Player::getXPos() const
{
	return position.x;
}

int Player::getYPos() const
{
	return position.y;
}

int Player::getWidth() const
{
	return size.x;
}

int Player::getHeight() const
{
	return size.y;
}

void Player::setXPos(const int x)
{
	position.x = x;
}

void Player::setYPos(const int y)
{
	position.y = y;
}

void Player::renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, const int camX, const int camY, const double angle, const SDL_Point* center, const SDL_RendererFlip flipType) const  // NOLINT(clang-diagnostic-shadow)
{
	int renderX = position.x - camX;
	const int renderY = position.y - camY;
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