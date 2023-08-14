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
	static GameEngine& getInstance();

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

	void updateCollision();
	void updateGUI();
	void updateCamera();
	void updateEnemies();
	void updateBullets();
	void updateEnemiesKilled();
	void renderPlayer();
	void renderEnemies();
	void renderBullets() const;
	void checkPlayerEnemyCollision();
	static bool checkCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
	void handleCollision(Enemy& object1, Enemy& object2) const;
	void renderFPS();

	void run();													// run the program

private:
	int countedFrames = 0;
	int idleAnimationFrame = 0;
	int runAnimationFrame = 0;
	int enemyWalkAnimationFrame = 0;
	bool running;

	Uint32 currentFrameTime = 0;
	Uint32 frameTime = 0;
	Uint32 prevFrameTime = 0;
	Uint32 shootInterval = 700;
	Uint32 lastShotTime = 0;

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
	std::array<SDL_Rect, PLAYER_IDLE_ANIMATION_FRAMES> player1Rect;
	TextureManager player1Tex;
	Player player1;

	// player run animation
	std::array<SDL_Rect, PLAYER_RUNNING_ANIMATION_FRAMES> player1RunRect;
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

	// enemy1 members
	std::array<SDL_Rect, ENEMY_WALK_ANIMATION_FRAMES> enemy1WalkRect;
	std::vector<Enemy> enemy1WalkVec;
	TextureManager enemy1WalkTex;
	Enemy enemy1Walk;
};

#endif