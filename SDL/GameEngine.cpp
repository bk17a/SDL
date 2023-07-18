#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine()
{
	renderer = NULL;
	window = NULL;
	running = true;
	font = NULL;
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
			if (renderer == NULL)
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
	if (font == NULL)
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
		// set player 2 rect
		player2Rect[0].x = 0;
		player2Rect[0].y = 0;
		player2Rect[0].w = 127;
		player2Rect[0].h = 70;

		player2Rect[1].x = 127;
		player2Rect[1].y = 0;
		player2Rect[1].w = 127;
		player2Rect[1].h = 70;

		player2Rect[2].x = 254;
		player2Rect[2].y = 0;
		player2Rect[2].w = 127;
		player2Rect[2].h = 70;

		player2Rect[3].x = 381;
		player2Rect[3].y = 0;
		player2Rect[3].w = 127;
		player2Rect[3].h = 70;

		player2Rect[4].x = 381;
		player2Rect[4].y = 0;
		player2Rect[4].w = 127;
		player2Rect[4].h = 70;
	}
	return success;
}

void GameEngine::update()
{
	player1.move();

	//Center the camera over the dot
	camera.x = (player1.getXPos() + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (player1.getYPos() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

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

void GameEngine::render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Render background texture to screen
	grass.render(0, 0, renderer, &camera);

	//// Render fps
	fpsTexture.render(SCREEN_WIDTH - fpsTexture.getWidth(), 0, renderer);

	// Render player
	player1.render(renderer, camera.x, camera.y);

	// Render current frame
	SDL_Rect* currentClip = &player2Rect[animationFrame / 5];
	player2Tex.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, renderer, currentClip);
}

bool GameEngine::handleEvents()
{
	SDL_Event e;

	// quit out
	SDL_PollEvent(&e);
	switch (e.type)
	{
	case SDL_QUIT:
		running = false;
		break;

	default:
		break;
	}

	// player movement
	player1.handleEvent(e);

	// window events
	windowObj.handleEvent(e, renderer);

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
	font = NULL;

	// Destory window
	windowObj.free();
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// Quit SDL Subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameEngine::run()
{
	if (!init())
	{
		cout << "Failed to initialize game engine!\n ";
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

			// Variables for frame rate limiting
			const int displayIndex = 0;  // Index of the display to query
			SDL_DisplayMode displayMode;

			SDL_GetCurrentDisplayMode(displayIndex, &displayMode);
			const int SCREEN_TICKS_PER_FRAME = 1000 / displayMode.refresh_rate;

			Uint32 prevFrameTime = 0;

			// current animation frame
			animationFrame = 0;

			// Game loop
			while (isRunning())
			{
				Uint32 currentFrameTime = SDL_GetTicks();
				Uint32 frameTime = currentFrameTime - prevFrameTime;

				// event handler
				handleEvents();

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

					// Update screen
					SDL_RenderPresent(renderer);
				}

				// fps frames
				++countedFrames;

				// Go to next frame every 5th frame
				if (countedFrames % 8 == 0)
				{
					++animationFrame;

					// Cycle animation
					if (animationFrame / 5 >= WALKING_ANIMATION_FRAMES)
					{
						animationFrame = 0;
					}
				}
			}
		}
	}
}