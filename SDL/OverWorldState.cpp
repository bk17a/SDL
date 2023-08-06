#include "OverWorldState.h"
#include "Constants.h"

OverWorldState::OverWorldState() = default;  // NOLINT(cppcoreguidelines-pro-type-member-init)

OverWorldState::OverWorldState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font)
{
	this->renderer = renderer;
	this->bgTexture = bgTexture;
	this->msgTexture = msgTexture;
	this->font = font;
}

OverWorldState* OverWorldState::get()
{
	// get static instance
	return &overWorldState;
}

bool OverWorldState::enter()
{
	bool success = true;

	if (!bgTexture->loadFromFile("bg", renderer))
	{
		std::cout << "Failed to load bg texture!\n";
		success = false;
	}

	// load player and set rect

	// load enemies

	// load bullets

	return success;
}

bool OverWorldState::exit()
{
	// free bg and text
	bgTexture->free();
	msgTexture->free();

	// close font
	TTF_CloseFont(font);
	font = nullptr;

	// close renderer
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	return true;
}

void OverWorldState::handleEvent(SDL_Event& e)
{
	// handle player input
}

void OverWorldState::render(SDL_Renderer* renderer)  // NOLINT(clang-diagnostic-shadow)
{
	// render bg, player, enemies, bullets
	// render from the main game engine function
}

void OverWorldState::update()
{
	// update player, enemies, camera, etc
	// update from the main game engine function
}

OverWorldState OverWorldState::overWorldState;