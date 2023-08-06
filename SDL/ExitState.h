#ifndef EXIT_STATE_H
#define EXIT_STATE_H
#include <SDL_events.h>

#include <SDL_ttf.h>
#include <SDL.h>
#include "GameState.h"
#include "TextureManager.h"

class ExitState final : public GameState
{
public:
	//Static accessor
	static ExitState* get();

	bool enter() override;
	bool exit() override;

	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
private:
	static ExitState exitState;

	ExitState();
	ExitState(SDL_Renderer* renderer, TextureManager* bgTexture, TextureManager* msgTexture, TTF_Font* font);

	TextureManager* bgTexture;
	TextureManager* msgTexture;
	TTF_Font* font;
	SDL_Renderer* renderer;
};

#endif // EXIT_STATE_H