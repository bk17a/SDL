#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine() : running(true), font(nullptr), window(nullptr), renderer(nullptr),
player1Rect({ {{0, 0, 0, 0}} }), player1RunRect({ {{0, 0, 0, 0}} }) {}

GameEngine::~GameEngine()
{
	close();
}

bool GameEngine::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Failed to initialize! SDL Error: " << SDL_GetError() << endl;
		running = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!\n";
			running = false;
		}

		//Create window
		if (!windowObj.init())
		{
			cout << "Failed to create window! SDL Error: " << SDL_GetError() << endl;
			running = false;
		}
		else
		{
			//Create renderer
			renderer = windowObj.createRenderer();
			if (renderer == nullptr)
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				running = false;
			}

			// Set background color
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			// Initialize SDL_image
			constexpr int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				running = false;
			}

			//Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
				running = false;
			}
		}
	}
	return running;
}

bool GameEngine::loadMedia()
{
	bool success = true;

	// load background
	if (!grass.loadFromFile("gfx/bg.png", renderer))
	{
		cout << "Failed to load grass texture image!\n";
		success = false;
	}

	// load text
	font = TTF_OpenFont("fonts/press_start.ttf", 18);
	if (font == nullptr)
	{
		cout << "Failed to load press_start font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}

	if (!player1Tex.loadFromFile("gfx/player1Idle.png", renderer))
	{
		cout << "Failed to load player 2 idle texture!\n";
		success = false;
	}
	else
	{
		// Create the player object
		player1 = Player(renderer, &player1Tex);

		// set player 2 rect
		player1Rect[0].x = 0;
		player1Rect[0].y = 0;
		player1Rect[0].w = PLAYER1_WIDTH;
		player1Rect[0].h = PLAYER1_HEIGHT;

		player1Rect[1].x = PLAYER1_WIDTH;
		player1Rect[1].y = 0;
		player1Rect[1].w = PLAYER1_WIDTH;
		player1Rect[1].h = PLAYER1_HEIGHT;

		player1Rect[2].x = PLAYER1_WIDTH * 2;
		player1Rect[2].y = 0;
		player1Rect[2].w = PLAYER1_WIDTH;
		player1Rect[2].h = PLAYER1_HEIGHT;

		player1Rect[3].x = PLAYER1_WIDTH * 3;
		player1Rect[3].y = 0;
		player1Rect[3].w = PLAYER1_WIDTH;
		player1Rect[3].h = PLAYER1_HEIGHT;

		player1Rect[4].x = PLAYER1_WIDTH * 3;
		player1Rect[4].y = 0;
		player1Rect[4].w = PLAYER1_WIDTH;
		player1Rect[4].h = PLAYER1_HEIGHT;
	}

	if (!player1RunTex.loadFromFile("gfx/player1Run.png", renderer))
	{
		cout << "Failed to load player 2 run texture!\n";
		success = false;
	}
	else
	{
		// set the texture
		player1Run = Player(renderer, &player1RunTex);

		player1RunRect[0].x = 0;
		player1RunRect[0].y = 0;
		player1RunRect[0].w = PLAYER1_WIDTH;
		player1RunRect[0].h = PLAYER1RUN_HEIGHT;

		player1RunRect[1].x = PLAYER1_WIDTH;
		player1RunRect[1].y = 0;
		player1RunRect[1].w = PLAYER1_WIDTH;
		player1RunRect[1].h = PLAYER1RUN_HEIGHT;

		player1RunRect[2].x = PLAYER1_WIDTH * 2;
		player1RunRect[2].y = 0;
		player1RunRect[2].w = PLAYER1_WIDTH;
		player1RunRect[2].h = PLAYER1RUN_HEIGHT;

		player1RunRect[3].x = PLAYER1_WIDTH * 3;
		player1RunRect[3].y = 0;
		player1RunRect[3].w = PLAYER1_WIDTH;
		player1RunRect[3].h = PLAYER1RUN_HEIGHT;

		player1RunRect[4].x = PLAYER1_WIDTH * 3;
		player1RunRect[4].y = 0;
		player1RunRect[4].w = PLAYER1_WIDTH;
		player1RunRect[4].h = PLAYER1RUN_HEIGHT;
	}

	if (!enemyTex.loadFromFile("gfx/testEnemy.png", renderer))
	{
		cout << "Unable to load test enemy texture!\n";
		success = false;
	}
	else
	{
		//enemy = Enemy(renderer, &enemyTex);

		for (int i = 0; i < ENEMY_NUM; ++i)
		{
			enemy = Enemy(renderer, &enemyTex);
			enemies.emplace_back(enemy);
		}

		if (!enemy.isAlive())
		{
			for (auto& e : enemies)
			{
				e.spawn();
			}
		}
	}

	return success;
}

void GameEngine::render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Render background texture to screen
	grass.render(0, 0, renderer, &camera);

	// Render fps
	fpsTexture.render(SCREEN_WIDTH - fpsTexture.getWidth(), 0, renderer);

	// Check if the player is moving or not
	const bool isMoving = player1.isMoving();

	// Render player
	if (isMoving)
	{
		const SDL_Rect* currentRunClip = &player1RunRect[runAnimationFrame / RUNNING_ANIMATION_FRAMES];
		player1Run.renderAnimated(renderer, currentRunClip, camera.x, camera.y, NULL, nullptr, player1.getFlipType());

		// Increment the animation frame for the running animation
		++runAnimationFrame;

		// Cycle animation
		if (runAnimationFrame / 5 >= RUNNING_ANIMATION_FRAMES)
		{
			runAnimationFrame = 0;
		}
	}
	else
	{
		const SDL_Rect* currentIdleClip = &player1Rect[idleAnimationFrame / IDLE_ANIMATION_FRAMES];
		player1.renderAnimated(renderer, currentIdleClip, camera.x, camera.y, NULL, nullptr, player1.getFlipType());

		// Increment the animation frame for the idle animation
		++idleAnimationFrame;

		// Cycle animation
		if (idleAnimationFrame / 5 >= IDLE_ANIMATION_FRAMES)
		{
			idleAnimationFrame = 0;
		}
	}

	// Render test enemy
	for (auto& e : enemies)
	{
		if (e.isAlive())
		{
			e.render(renderer, camera.x, camera.y);
		}
	}
}

void GameEngine::update()
{
	//player1.move();
	player1.move();
	player1Run.move();

	//Center the camera over the dot
	camera.x = (player1.getXPos() + PLAYER1_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (player1.getYPos() + PLAYER1_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	//enemy.updatePos(player1, ENEMY_VEL, camera);
	for (auto& e : enemies)
	{
		// enemy movement toward player
		if (e.getPosX() < player1.getXPos()) e.setPosX(e.getPosX() + ENEMY_VEL);
		if (e.getPosX() > player1.getXPos()) e.setPosX(e.getPosX() - ENEMY_VEL);
		if (e.getPosY() < player1.getYPos()) e.setPosY(e.getPosY() + ENEMY_VEL);
		if (e.getPosY() > player1.getYPos()) e.setPosY(e.getPosY() - ENEMY_VEL);

		// check collision
	}
}

bool GameEngine::handleEvents()
{
	SDL_Event e;

	// process all events in the queue
	while (SDL_PollEvent(&e))
	{
		// quit out
		if (e.type == SDL_QUIT)
		{
			running = false;
		}

		// player movement
		player1.handleEvent(e);
		player1Run.handleEvent(e);

		// window events
		windowObj.handleEvent(e, renderer);
	}

	return running;
}

SDL_Window* GameEngine::getWindow() const
{
	return window;
}

SDL_Renderer* GameEngine::getRenderer() const
{
	return renderer;
}

TTF_Font* GameEngine::getFont() const
{
	return font;
}

bool GameEngine::isRunning() const
{
	return running;
}

void GameEngine::quit()
{
	running = false;
}

void GameEngine::close()
{
	// Free loaded images
	grass.free();
	fpsTexture.free();
	player1Tex.free();
	player1RunTex.free();

	// Close font
	TTF_CloseFont(font);
	font = nullptr;

	// Destory window
	windowObj.free();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	// Quit SDL Subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameEngine::run()
{
	if (!init())
	{
		cout << "Failed to initialize game engine!\n";
	}
	else
	{
		// Seed the random number generator with a time-based value
		srand(static_cast<unsigned int>(time(nullptr)));  // NOLINT(cert-msc51-cpp)

		if (!loadMedia())
		{
			cout << "Failed to load media!\n";
		}
		else
		{
			// show fps
			fpsTimer.start();
			stringstream timeText;
			constexpr SDL_Color textColor = { 0, 0, 0, 255 };
			Uint32 prevFrameTime = 0;

			// Game loop
			while (isRunning())
			{
				const Uint32 currentFrameTime = SDL_GetTicks();
				const Uint32 frameTime = currentFrameTime - prevFrameTime;

				// event handler
				handleEvents();

				// constants for frame rate limiting
				constexpr int TARGET_FPS = 165;
				constexpr int SCREEN_TICKS_PER_FRAME = 1000 / TARGET_FPS;

				// Calculate the time taken for the frame
				const int delayTime = SCREEN_TICKS_PER_FRAME - frameTime;  // NOLINT(cppcoreguidelines-narrowing-conversions)
				if (delayTime > 0)
				{
					SDL_Delay(delayTime);
				}

				prevFrameTime = currentFrameTime;

				// Calculate average FPS
				const float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);  // NOLINT(clang-diagnostic-implicit-int-float-conversion, cppcoreguidelines-narrowing-conversions)

				// update function
				update();

				if (!windowObj.isMin())
				{
					// Render FPS
					if (avgFPS > 0)
					{
						// Render text
						timeText.str("");
						timeText << "FPS: " << fixed << setprecision(0) << avgFPS;
						if (!fpsTexture.loadFromRenderedText(timeText.str().c_str(), textColor, renderer, font))
						{
							cout << "Unable to render FPS texture!\n";
						}
					}

					// Render function
					render();

					// Update screen
					SDL_RenderPresent(renderer);
				}

				// fps frames
				++countedFrames;
			}
		}
	}
}