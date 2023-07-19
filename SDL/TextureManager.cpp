#include "TextureManager.h"

using namespace std;

TextureManager::TextureManager()
{
	texture = nullptr;
	width = 0;
	height = 0;
}

TextureManager::~TextureManager()
{
	free();
}

void TextureManager::free()
{
	//Free texture if it exists
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

bool TextureManager::loadFromFile(const char* path, SDL_Renderer* renderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == nullptr)
	{
		cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr)
		{
			cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	texture = newTexture;
	return texture != nullptr;
}

bool TextureManager::loadFromRenderedText(const char* textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font)
{
	// get rid of preexisting texture
	free();

	// render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, textColor);
	if (textSurface == nullptr)
	{
		cout << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		// create texture from surface
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr)
		{
			cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			width = textSurface->w;
			height = textSurface->h;
		}

		// get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	return texture != nullptr;
}

void TextureManager::render(int x, int y, SDL_Renderer* renderer, const SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(renderer, texture, clip, &renderQuad);
}

int TextureManager::getHeight() const
{
	return height;
}

int TextureManager::getWidth() const
{
	return width;
}

void TextureManager::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(texture, alpha);
}
