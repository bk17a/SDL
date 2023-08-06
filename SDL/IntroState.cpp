#include "IntroState.h"
#include "Constants.h"
#include "GameEngine.h"
#include "TitleState.h"

IntroState::IntroState() = default;  // NOLINT(cppcoreguidelines-pro-type-member-init)

IntroState::IntroState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font)
{
	this->renderer = renderer;
	this->bgTexture = bgTexture;
	this->msgTexture = msgTexture;
	this->font = font;
}

IntroState* IntroState::get()
{
	// get static instance
	return &introState;
}

bool IntroState::enter()
{
	// sucess flag
	bool success = true;

	// load bg
	if (!bgTexture->loadFromFile("bg2.png", renderer))
	{
		std::cout << "Failed to load bg2 texture!\n";
		success = false;
	}

	// load text
	constexpr SDL_Color textColor = { 255, 255 ,255 ,255 };
	if (!msgTexture->loadFromRenderedText("PRESS ENTER TO START", textColor, renderer, font))
	{
		std::cout << "Failed to load intro text!\n";
		success = false;
	}

	return success;
}

bool IntroState::exit()
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

void IntroState::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		// move onto next game state (title)
		GameEngine::getInstance().setNextState(TitleState::get());
	}
}

void IntroState::update()
{
}

void IntroState::render(SDL_Renderer* renderer) // NOLINT(clang-diagnostic-shadow)
{
	bgTexture->render(0, 0, renderer);

	msgTexture->render((SCREEN_WIDTH - msgTexture->getWidth()) / 2, (SCREEN_HEIGHT - msgTexture->getHeight()) / 2, renderer);
}

// declare static instance
IntroState IntroState::introState;