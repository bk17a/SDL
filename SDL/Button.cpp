// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "Button.h"

Button::Button() = default;

Button::Button(SDL_Renderer* renderer, TextureManager* texture, const Vector2 pos)
{
	this->renderer = renderer;
	this->texture = texture;
	this->pos = pos;

	size.x = static_cast<float>(this->texture->getWidth());
	size.y = static_cast<float>(this->texture->getHeight());
}

void Button::render(SDL_Renderer* renderer) const
{
	const int renderX = static_cast<int>(pos.x);
	const int renderY = static_cast<int>(pos.y);

	texture->render(renderX, renderY, renderer);
}

bool Button::handleEvents(const SDL_Event& e) const
{
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		// get mouse pos
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;
		if (static_cast<float>(x) < pos.x || static_cast<float>(x) > pos.x + size.x || static_cast<float>(y) < pos.y || static_cast<float>(y) > pos.y + size.y) inside = false;

		return inside;
	}
	return false;
}