#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	void free();
	bool loadFromFile(const char* path, SDL_Renderer* renderer);
	bool loadFromRenderedText(const char* textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font);
	void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void setAlpha(Uint8 alpha);

	int getWidth() const;
	int getHeight() const;

private:
	SDL_Texture* texture;
	int width;
	int height;
};

#endif