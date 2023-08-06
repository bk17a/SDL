#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include <SDL_ttf.h>
#include <SDL.h>
#include "GameState.h"
#include "TextureManager.h"

class IntroState final : public GameState
{
public:
	// access state
	static IntroState* get();

	// transitions
	bool enter() override;
	bool exit() override;

	// main loop funct
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
private:
	// static instance
	static IntroState introState;

	// private constructor
	IntroState();
	IntroState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font);

	TextureManager* bgTexture;
	TextureManager* msgTexture;
	SDL_Renderer* renderer;
	TTF_Font* font;
};

#endif // INTRO_STATE_H