#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <SDL.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "TextureManager.h"
#include "Timer.h"
#include "Player.h"
#include "Constants.h"
#include "Window.h"

class GameEngine
{
public:

	// initialization
	GameEngine();  // default constructor
	~GameEngine(); // default destructor

	bool init();							// initialize renderer, window, font, image
	bool loadMedia();						// loads media (image, fonts)
	void update();
	void render();							// render textures, fonts, etc
	bool handleEvents();					// handle quit event
	void quit();							// quit out
	void close();							// free and destroy after using
	bool isRunning() const;					// check to see if program is running
	void run();								// run the program

	// getters
	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	TTF_Font* getFont() const;

private:
	int countedFrames = 0;
	int animationFrame = 0;

	// running flag
	bool running;															
	
	// grass texture
	TextureManager grass;	

	// player texture and object
	TextureManager player1Tex;
	Player player1;

	// fps texture and timer object
	Timer fpsTimer;
	TextureManager fpsTexture;

	// font
	TTF_Font* font;

	// window and renderer
	SDL_Window* window;
	SDL_Renderer* renderer;
	Window windowObj;

	// camera rect
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	
	// player animations
	std::array<SDL_Rect, WALKING_ANIMATION_FRAMES> player2Rect;
	TextureManager player2Tex;
	Player player2;
};

#endif
