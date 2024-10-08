// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "Player.h"

Player::Player() = default; // NOLINT(cppcoreguidelines-pro-type-member-init)

Player::Player(SDL_Renderer* renderer, TextureManager* playerTexture)
{
	position = Vector2(0, 0);
	size = Vector2(PLAYER1_WIDTH, PLAYER1_HEIGHT);
	velocity = Vector2(0, 0);
	moving = false;
	this->renderer = renderer;
	this->playerTexture = playerTexture;
	flipType = SDL_FLIP_NONE;
	alive = false;
	hpMax = 100;
	hp = hpMax;
	p.x = static_cast<int>(position.x);
	p.y = static_cast<int>(position.y);
	p.w = static_cast<int>(size.x);
	p.h = static_cast<int>(size.y);
	collisionCooldown = 0;
	score = 0;

	exp = 0;
	level = 1;
	maxExp = 100;
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

void Player::render(SDL_Renderer* renderer, const float camX, const float camY) const
{
	const int renderX = static_cast<int>(position.x - camX);
	const int renderY = static_cast<int>(position.y - camY);

	playerTexture->render(renderX, renderY, renderer);
}

void Player::renderAnimated(SDL_Renderer* renderer, const SDL_Rect* clip, const float camX, const float camY, const double angle, const SDL_Point* center, const SDL_RendererFlip flipType) const
{
	int renderX = static_cast<int>(position.x - camX);
	const int renderY = static_cast<int>(position.y - camY);
	if (flipType == SDL_FLIP_HORIZONTAL)
	{
		renderX += PLAYER1_WIDTH - 200;	// flip in place
	}
	playerTexture->render(renderX, renderY, renderer, clip, angle, center, flipType);
}

float Player::getXPos() const
{
	return position.x;
}

float Player::getYPos() const
{
	return position.y;
}

float Player::getWidth() const
{
	return size.x;
}

float Player::getHeight() const
{
	return size.y;
}

Vector2 Player::getPlayerPos() const
{
	return position;
}

void Player::setXPos(const float x)
{
	position.x = x;
}

void Player::setYPos(const float y)
{
	position.y = y;
}

bool Player::isMoving() const
{
	return moving;
}

SDL_RendererFlip Player::getFlipType() const
{
	return flipType;
}

void Player::spawn()
{
	position.x = (SCREEN_WIDTH - size.x) / 2;
	position.y = (SCREEN_HEIGHT - size.y) / 2;
	alive = true;
}

void Player::setHp(const int hp)
{
	this->hp = hp;
	if (hp <= 0)
	{
		kill();
	}
}

void Player::takeDamage(const int damage)
{
	if (collisionCooldown <= 0)
	{
		hp -= damage;
		collisionCooldown = 60; // set collision cooldown to 60 frames
		if (hp <= 0)
		{
			kill();
		}
	}
}

void Player::kill()
{
	alive = false;
}

bool Player::isAlive() const
{
	return alive;
}

void Player::setAlive(const bool alive)
{
	this->alive = alive;
}

int Player::getHp() const
{
	return hp;
}

int Player::getHpMax() const
{
	return hpMax;
}

void Player::update()
{
	if (collisionCooldown > 0)
		collisionCooldown--;
}

void Player::setScore(const int score)
{
	this->score = score;
}

int Player::getScore() const
{
	return score;
}

void Player::increaseScore(const int points)
{
	score += points;
}

void Player::resetScore()
{
	score = 0;
}

void Player::resetHealth()
{
	hp = 100;
}

void Player::increaseExp(const int exp)
{
	this->exp += exp;
}

void Player::resetExp()
{
	this->exp = 0;
}

void Player::increaseMaxExp(const int exp)
{
	maxExp += exp;
}

void Player::resetMaxExp()
{
	maxExp = 100;
}

bool Player::levelUp()
{
	if (exp >= maxExp)
	{
		level += 1;
		return true;
	}
	return false;
}

void Player::resetLevel()
{
	level = 1;
}

int Player::getLevel() const
{
	return level;
}

int Player::getExp() const
{
	return exp;
}

int Player::getMaxExp() const
{
	return maxExp;
}