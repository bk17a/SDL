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
		/*window = SDL_CreateWindow("GAME V0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)*/
		if(!windowObj.init())
		{
			cout << "Failed to create window! SDL Error: " << SDL_GetError() << endl;
			running = false;
		}
		else
		{
			//Create renderer
			//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	if (!playerTex.loadFromFile("gfx/single.png", renderer))
	{
		cout << "Failed to load player texture image!\n";
		success = false;
	}
	else
	{
		const int playerWidth = 50;
		const int playerHeight = 50;
		// Create the player object
		player1 = Player(renderer, &playerTex);
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
    int x1 = 0;
    int y1 = 0;
    int screenWidth, screenHeight;
    SDL_GetWindowSize(windowObj.getWindow(), &screenWidth, &screenHeight);
    grass.render(x1, y1, screenWidth, screenHeight, renderer, &camera);

    // Render fps
    fpsTexture.render(screenWidth - fpsTexture.getWidth(), 0, fpsTexture.getWidth(), fpsTexture.getHeight(), renderer);

    // Render player
    player1.render(renderer, camera.x, camera.y);

    // Update Screen
    SDL_RenderPresent(renderer);
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
	playerTex.free();

	// Close font
	TTF_CloseFont(font);
	font = NULL;

	// Destory window
	windowObj.free();
	//SDL_DestroyWindow(window);
	//window = NULL;
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
				if (avgFPS > 0)
				{
					//Render text
					timeText.str("");
					timeText << "FPS: " << fixed << setprecision(0) << avgFPS;
					if (!fpsTexture.loadFromRenderedText(timeText.str().c_str(), textColor, renderer, font))
					{
						cout << "Unable to render FPS texture!\n";
					}
				}
				// update function
				update();
				if (!windowObj.isMin())
				{
					// render function
					render();
				}
				++countedFrames;
			}
		}
	}
}

TextureManager GameEngine::getRedTex() const
{
	return redTex;
}

TextureManager GameEngine::getGreenTex() const
{
	return greenTex;
}

TextureManager GameEngine::getBlueTex() const
{
	return blueTex;
}

TextureManager GameEngine::getShimmerTex() const
{
	return shimmerTex;
}

GameEngine* GameEngine::getGameEngineInstance()
{
	static GameEngine gameEngine; // Create a static instance of GameEngine

	return &gameEngine;
}