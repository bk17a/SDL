#ifndef OVER_WORLD_STATE_H
#define OVER_WORLD_STATE_H

#include <SDL_ttf.h>
#include <SDL.h>
#include "GameState.h"
#include "TextureManager.h"

class OverWorldState final : public GameState
{
public:
	// static accessor
	static OverWorldState* get();

	bool enter() override;
	bool exit() override;

	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
private:
	// static instance
	static OverWorldState overWorldState;

	OverWorldState();
	OverWorldState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font);

	TextureManager* bgTexture;
	TextureManager* msgTexture;
	TTF_Font* font;
	SDL_Renderer* renderer;
};

#endif // OVER_WORLD_STATE_H
