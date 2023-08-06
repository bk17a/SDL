#include "TitleState.h"
#include "GameEngine.h"
#include "Constants.h"
#include "OverWorldState.h"

TitleState::TitleState() = default;  // NOLINT(cppcoreguidelines-pro-type-member-init)

TitleState::TitleState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font)
{
	this->renderer = renderer;
	this->bgTexture = bgTexture;
	this->msgTexture = msgTexture;
	this->font = font;
}

TitleState* TitleState::get()
{
	// get static instance
	return &titleState;
}

bool TitleState::enter()
{
	bool success = true;
	if (!bgTexture->loadFromFile("bg3.png", renderer))
	{
		std::cout << "Failed to load bg3 texture!\n";
		success = false;
	}

	constexpr SDL_Color textColor = { 255, 255 ,255 ,255 };
	if (!msgTexture->loadFromRenderedText("WELCOME TO ...", textColor, renderer, font))
	{
		std::cout << "Failed to load font!\n";
		success = false;
	}

	return success;
}

bool TitleState::exit()
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

void TitleState::handleEvent(SDL_Event& e)
{
	//If the user pressed enter
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_RETURN))
	{
		//Move to overworld
		GameEngine::getInstance().setNextState(OverWorldState::get());
	}
}

void TitleState::update()
{
}

void TitleState::render(SDL_Renderer* renderer) // NOLINT(clang-diagnostic-shadow)
{
	bgTexture->render(0, 0, renderer);

	msgTexture->render((SCREEN_WIDTH - msgTexture->getWidth()) / 2, (SCREEN_HEIGHT - msgTexture->getHeight()) / 2, renderer);
}

// declare static instance
TitleState TitleState::titleState;