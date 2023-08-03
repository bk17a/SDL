// ReSharper disable CppUnusedIncludeDirective
#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include <array>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Constants.h"
#include "Player.h"
#include "TextureManager.h"
#include "Timer.h"
#include "Window.h"
#include "Enemy.h"
#include "Bullet.h"

class GameEngine
{
public:
	Uint32 lastShotTime = 0;
	// initialization
	GameEngine();  // default constructor
	~GameEngine(); // default destructor

	bool init();											// initialize renderer, window, font, image
	bool loadMedia();										// loads media (image, fonts)
	void update();
	void render();											// render textures, fonts, etc
	bool handleEvents();									// handle quit event

	// getters
	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	TTF_Font* getFont() const;

	bool isRunning() const;									// check to see if program is running
	void quit();											// quit out
	void close();											// free and destroy after using

	void checkCollision();
	void updateGUI();
	void updateCamera();
	void updateEnemies();
	void updateBullets();
	void updateEnemiesKilled();
	void renderPlayer();
	void renderEnemies() const;
	void renderBullets() const;

	void run();												// run the program

	// Helper function to convert SDL_Rect to a string
	std::string rectToString(const SDL_Rect& rect) const;

	void checkPlayerEnemyCollision(const Enemy& enemy);
private:
	int countedFrames = 0;
	int idleAnimationFrame = 0;
	int runAnimationFrame = 0;
	bool running;

	// grass texture
	TextureManager grass;

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

	// player idle animation
	std::array<SDL_Rect, IDLE_ANIMATION_FRAMES> player1Rect;
	TextureManager player1Tex;
	Player player1;

	// player run animation
	std::array<SDL_Rect, RUNNING_ANIMATION_FRAMES> player1RunRect;
	TextureManager player1RunTex;
	Player player1Run;

	// enemy members
	Enemy enemy;
	TextureManager enemyTex;
	std::vector<Enemy> enemies;

	// bullet members
	TextureManager bulletTex;
	Bullet bullet;

	// PlayerGUI
	SDL_Rect playerHpBar;
	SDL_Rect playerHpBarBack;
};

#endif