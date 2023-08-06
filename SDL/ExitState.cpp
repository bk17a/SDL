#include "ExitState.h"

ExitState::ExitState() = default;  // NOLINT(cppcoreguidelines-pro-type-member-init)

ExitState::ExitState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font)
{
	this->renderer = renderer;
	this->bgTexture = bgTexture;
	this->msgTexture = msgTexture;
	this->font = font;
}

//Hollow exit state
ExitState* ExitState::get()
{
	return &exitState;
}

bool ExitState::enter()
{
	return true;
}

bool ExitState::exit()
{
	return true;
}

void ExitState::handleEvent(SDL_Event& e)
{
}

void ExitState::update()
{
}

void ExitState::render(SDL_Renderer* renderer)  // NOLINT(clang-diagnostic-shadow)
{
}

ExitState ExitState::exitState;