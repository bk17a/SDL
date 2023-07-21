#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine()
{
	renderer = nullptr;
	window = nullptr;
	running = true;
	font = nullptr;
	player2Rect = { 0,0,0,0 };
	player2RunRect = { 0,0,0,0 };
}

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
			int imgFlags = IMG_INIT_PNG;
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

	// Load player1 texture
	if (!player1Tex.loadFromFile("gfx/player1.png", renderer))
	{
		cout << "Failed to load player texture image!\n";
		success = false;
	}
	else
	{
		// Create the player object
		player1 = Player(renderer, &player1Tex);
	}

	if (!player2Tex.loadFromFile("gfx/player2Idle.png", renderer))
	{
		cout << "Fauled to load player 2 idle texture!\n";
		success = false;
	}
	else
	{
		// Create the player object
		player2 = Player(renderer, &player2Tex);

		// set player 2 rect
		player2Rect[0].x = 0;
		player2Rect[0].y = 0;
		player2Rect[0].w = PLAYER2_WIDTH;
		player2Rect[0].h = PLAYER2_HEIGHT;

		player2Rect[1].x = PLAYER2_WIDTH;
		player2Rect[1].y = 0;
		player2Rect[1].w = PLAYER2_WIDTH;
		player2Rect[1].h = PLAYER2_HEIGHT;

		player2Rect[2].x = PLAYER2_WIDTH * 2;
		player2Rect[2].y = 0;
		player2Rect[2].w = PLAYER2_WIDTH;
		player2Rect[2].h = PLAYER2_HEIGHT;

		player2Rect[3].x = PLAYER2_WIDTH * 3;
		player2Rect[3].y = 0;
		player2Rect[3].w = PLAYER2_WIDTH;
		player2Rect[3].h = PLAYER2_HEIGHT;

		player2Rect[4].x = PLAYER2_WIDTH * 3;
		player2Rect[4].y = 0;
		player2Rect[4].w = PLAYER2_WIDTH;
		player2Rect[4].h = PLAYER2_HEIGHT;
	}

	if (!player2RunTex.loadFromFile("gfx/player2Run.png", renderer))
	{
		cout << "Failed to load player 2 run texture!\n";
		success = false;	
	}
	else
	{
		// set the texture
		player2Run = Player(renderer, &player2RunTex);

		player2RunRect[0].x = 0;
		player2RunRect[0].y = 0;
		player2RunRect[0].w = PLAYER2_WIDTH;
		player2RunRect[0].h = PLAYER2RUN_HEIGHT;

		player2RunRect[1].x = PLAYER2_WIDTH;
		player2RunRect[1].y = 0;
		player2RunRect[1].w = PLAYER2_WIDTH;
		player2RunRect[1].h = PLAYER2RUN_HEIGHT;

		player2RunRect[2].x = PLAYER2_WIDTH * 2;
		player2RunRect[2].y = 0;
		player2RunRect[2].w = PLAYER2_WIDTH;
		player2RunRect[2].h = PLAYER2RUN_HEIGHT;

		player2RunRect[3].x = PLAYER2_WIDTH * 3;
		player2RunRect[3].y = 0;
		player2RunRect[3].w = PLAYER2_WIDTH;
		player2RunRect[3].h = PLAYER2RUN_HEIGHT;

		player2RunRect[4].x = PLAYER2_WIDTH * 3;
		player2RunRect[4].y = 0;
		player2RunRect[4].w = PLAYER2_WIDTH;
		player2RunRect[4].h = PLAYER2RUN_HEIGHT;
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
	bool isMoving = player2.isMoving();

	// Render player
	if (isMoving)
	{
		const SDL_Rect* currentRunClip = &player2RunRect[runAnimationFrame / RUNNING_ANIMATION_FRAMES];
		player2Run.renderAnimated(renderer, currentRunClip, camera.x, camera.y, player2.getAngle(), player2.getCenter(), player2.getFlipType());

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
		const SDL_Rect* currentIdleClip = &player2Rect[idleAnimationFrame / IDLE_ANIMATION_FRAMES];
		player2.renderAnimated(renderer, currentIdleClip, camera.x, camera.y, player2.getAngle(), player2.getCenter(), player2.getFlipType());

		// Increment the animation frame for the idle animation
		++idleAnimationFrame;

		// Cycle animation
		if (idleAnimationFrame / 5 >= IDLE_ANIMATION_FRAMES)
		{
			idleAnimationFrame = 0;
		}
	}

	// Update screen
	SDL_RenderPresent(renderer);
}


void GameEngine::update()
{
	//player1.move();
	player2.move();
	player2Run.move();

	//Center the camera over the dot
	camera.x = (player2.getXPos() + PLAYER2_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (player2.getYPos() + PLAYER2_HEIGHT / 2) - SCREEN_HEIGHT / 2;

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
		//player1.handleEvent(e);
		player2.handleEvent(e);
		player2Run.handleEvent(e);

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
	player2Tex.free();

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
		if (!loadMedia())
		{
			cout << "Failed to load media!\n";
		}
		else
		{
			// show fps
			fpsTimer.start();
			stringstream timeText;
			SDL_Color textColor = { 0, 0, 0, 255 };
			Uint32 prevFrameTime = 0;

			// Game loop
			while (isRunning())
			{
				Uint32 currentFrameTime = SDL_GetTicks();
				Uint32 frameTime = currentFrameTime - prevFrameTime;

				// event handler
				handleEvents();

				// constants for frame rate limiting
				const int TARGET_FPS = 165;
				const int SCREEN_TICKS_PER_FRAME = 1000 / TARGET_FPS;

				// Calculate the time taken for the frame
				int delayTime = SCREEN_TICKS_PER_FRAME - frameTime;
				if (delayTime > 0)
				{
					SDL_Delay(delayTime);
				}

				prevFrameTime = currentFrameTime;

				// Calculate average FPS
				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);

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
				}

				// fps frames
				++countedFrames;
			}
		}
	}
}
