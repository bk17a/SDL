#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <SDL_ttf.h>
#include <SDL.h>
#include "GameState.h"
#include "TextureManager.h"

class TitleState final : public GameState
{
public:
	// static accessor
	static TitleState* get();

	bool enter() override;
	bool exit() override;

	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;

private:
	static TitleState titleState;

	// private constructors
	TitleState();
	TitleState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font);

	TextureManager* bgTexture;
	TextureManager* msgTexture;
	TTF_Font* font;
	SDL_Renderer* renderer;
};

#endif // TITLE_STATE_H