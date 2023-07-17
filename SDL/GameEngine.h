#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <SDL.h>
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
	int countedFrames = 0;

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

	TextureManager getRedTex() const;
	TextureManager getGreenTex() const;
	TextureManager getBlueTex() const;
	TextureManager getShimmerTex() const;

	// getters
	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	TTF_Font* getFont() const;


	static GameEngine* getGameEngineInstance();
	TextureManager redTex;
	TextureManager greenTex;
	TextureManager blueTex;
	TextureManager shimmerTex;
private:
	bool running;

	TextureManager grass;

	TextureManager playerTex;
	Player player1;

	Timer fpsTimer;
	TextureManager fpsTexture;

	TTF_Font* font;

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Window windowObj;
};

#endif
