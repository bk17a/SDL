// ReSharper disable CppUnusedIncludeDirective
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	void free();
	bool loadFromFile(const char* path, SDL_Renderer* renderer);
	bool loadFromRenderedText(const char* textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font);
	void render(int x, int y, SDL_Renderer* renderer, const SDL_Rect* clip = nullptr, const double angle = NULL, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void render2(SDL_Renderer* renderer, const int x, const int y, const int width, const int height) const;

	void setAlpha(Uint8 alpha) const;
	int getWidth() const;
	int getHeight() const;
	SDL_Texture* getTexture() const;
	void changeTextureColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) const;

private:
	SDL_Texture* texture;
	int width;
	int height;
};

#endif