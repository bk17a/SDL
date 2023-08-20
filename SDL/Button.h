#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "TextureManager.h"
#include "Vector2.h"

class Button
{
public:
	Button();
	Button(SDL_Renderer* renderer, TextureManager* texture, Vector2 pos);

	void render(SDL_Renderer* renderer) const;
	bool handleEvents(const SDL_Event& e) const;

private:
	Vector2 size;
	Vector2 pos;

	SDL_Renderer* renderer;
	TextureManager* texture;
};

#endif // BUTTON_H