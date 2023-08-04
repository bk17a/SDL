#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine() : running(true), font(nullptr), window(nullptr), renderer(nullptr),
player1Rect({ {{0, 0, 0, 0}} }), player1RunRect({ {{0, 0, 0, 0}} }),
playerHpBar({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }),
playerHpBarBack({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }) {}

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
		cout << "Failed to load player1 idle texture!\n";
		success = false;
	}
	else
	{
		// Create the player object
		player1 = Player(renderer, &player1Tex);

		player1.spawn();

		// set player 1 rect
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
		cout << "Failed to load player1 run texture!\n";
		success = false;
	}
	else
	{
		// set the texture
		player1Run = Player(renderer, &player1RunTex);
		player1Run.spawn();

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

	if (!bulletTex.loadFromFile("gfx/laser.png", renderer))
	{
		cout << "Unable to load bullet texture!\n";
		success = false;
	}
	else
	{
		const auto bulletSpeed = Vector2(10, 10);
		const auto bulletSize = Vector2(50, 50);
		bullet = Bullet(player1.getPlayerPos(), bulletSpeed, bulletSize, renderer, &bulletTex);
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

	// render game elements
	renderPlayer();
	renderEnemies();
	renderBullets();

	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 200); // Grey color with less opacity
	SDL_RenderFillRect(renderer, &playerHpBarBack);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &playerHpBar);
}

void GameEngine::update()
{
	player1.update();
	player1.move();
	player1Run.move();

	for (auto& e : enemies)
	{
		checkPlayerEnemyCollision(e);
	}

	checkCollision();
	updateGUI();
	updateCamera();
	//updateEnemies();
	updateBullets();
	updateEnemiesKilled();
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
		if (player1.isAlive())
		{
			player1.handleEvent(e);
			player1Run.handleEvent(e);
		}

		// window events
		windowObj.handleEvent(e, renderer);

		// bullet events
		if (!enemies.empty())	// disable bullet shoot if all enemies are dead
		{
			bullet.handleEvent(e, player1.getPlayerPos());
		}
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
	bulletTex.free();

	// clear enemy vector
	enemies.clear();

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

void GameEngine::checkCollision()
{
	// Check collision between enemies
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < enemies.size(); ++j)
		{
			if (enemies[i].checkCollisionWith(enemies[j].p))
			{
				// Calculate the direction of the collision between enemies
				float directionX = enemies[i].getPosX() - enemies[j].getPosX();
				float directionY = enemies[i].getPosY() - enemies[j].getPosY();

				// Normalize the direction vector
				const float length = sqrt(directionX * directionX + directionY * directionY);
				directionX /= length;
				directionY /= length;

				// Calculate the maximum distance the enemies can move back
				constexpr float maxDistance = 2; // Change this value to adjust the maximum distance

				// Move the colliding enemies one step back in the opposite direction, but limit the distance
				const float distanceX = directionX * maxDistance;
				const float distanceY = directionY * maxDistance;
				enemies[i].setPosX(enemies[i].getPosX() + distanceX);
				enemies[i].setPosY(enemies[i].getPosY() + distanceY);
				enemies[j].setPosX(enemies[j].getPosX() - distanceX);
				enemies[j].setPosY(enemies[j].getPosY() - distanceY);
			}
		}
	}
}

void GameEngine::updateGUI()
{
	// Calculate the percentage of HP remaining
	float hpPercent = static_cast<float>(player1.getHp()) / static_cast<float>(player1.getHpMax());

	// Ensure the percentage is between 0 and 1
	hpPercent = std::max(0.0f, std::min(hpPercent, 1.0f));

	// Calculate the HP bar width based on the percentage of HP remaining
	int hpBarWidth = static_cast<int>(PLAYER_HP_BAR_WIDTH * hpPercent);

	// Make sure the HP bar width doesn't exceed its maximum width (PLAYER_HP_BAR_WIDTH)
	hpBarWidth = std::min(hpBarWidth, PLAYER_HP_BAR_WIDTH);

	// Update the HP bar size
	playerHpBar = { PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, hpBarWidth, PLAYER_HP_BAR_HEIGHT };
}

void GameEngine::updateCamera()
{
	//Center the camera over the player
	camera.x = static_cast<int>(player1.getXPos() + static_cast<float>(PLAYER1_WIDTH) / 2 - static_cast<float>(SCREEN_WIDTH) / 2);
	camera.y = static_cast<int>(player1.getYPos() + static_cast<float>(PLAYER1_HEIGHT) / 2 - static_cast<float>(SCREEN_HEIGHT) / 2);

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

void GameEngine::updateEnemies()
{
	// looping through all the enemies in the vector
	for (auto& e : enemies)
	{
		// enemy movement toward player
		if (e.getPosX() < player1.getXPos()) e.setPosX(e.getPosX() + ENEMY_VEL);
		if (e.getPosX() > player1.getXPos()) e.setPosX(e.getPosX() - ENEMY_VEL);
		if (e.getPosY() < player1.getYPos()) e.setPosY(e.getPosY() + ENEMY_VEL);
		if (e.getPosY() > player1.getYPos()) e.setPosY(e.getPosY() - ENEMY_VEL);
	}
}

void GameEngine::updateBullets()
{
	// Updating bullet
	// Find the nearest enemy to the player's position
	const Vector2 playerPosition = player1.getPlayerPos();
	Vector2 nearestEnemyPosition;

	float nearestDistance = std::numeric_limits<float>::max();

	for (const auto& e : enemies)
	{
		if (e.isAlive())
		{
			Vector2 enemyPosition(e.getPosX(), e.getPosY());
			const float distance = (enemyPosition - playerPosition).calcVecLength();

			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestEnemyPosition = enemyPosition;
			}
		}
	}

	// Update active bullets with the nearest enemy's position as the target
	if (bullet.isActive())
	{
		bullet.setTargetPos(nearestEnemyPosition);
		bullet.update();
	}
}

void GameEngine::updateEnemiesKilled()
{
	std::vector<size_t> enemiesKilled;

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (bullet.isActive() && enemies[i].checkCollisionWith(bullet.p))
		{
			enemies[i].takeDamage(25);
			bullet.reload();

			// if enemy is dead, add it to enemiesKilled vector to remove it
			if (!enemies[i].isAlive())
			{
				enemiesKilled.emplace_back(i);
			}
		}
	}

	// Remove the marked enemies using the erase-remove idiom
	for (auto it = enemiesKilled.rbegin(); it != enemiesKilled.rend(); ++it)
	{
		enemies.erase(enemies.begin() + *it);
	}

	// clear the vector
	enemiesKilled.clear();
}

void GameEngine::renderPlayer()
{
	// Check if the player is moving or not
	const bool isMoving = player1.isMoving();
	if (player1.isAlive())
	{
		// Render player
		if (isMoving)
		{
			const SDL_Rect* currentRunClip = &player1RunRect[runAnimationFrame / RUNNING_ANIMATION_FRAMES];
			player1Run.renderAnimated(renderer, currentRunClip, static_cast<float>(camera.x), static_cast<float>(camera.y), NULL, nullptr, player1.getFlipType());

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
			player1.renderAnimated(renderer, currentIdleClip, static_cast<float>(camera.x), static_cast<float>(camera.y), NULL, nullptr, player1.getFlipType());

			// Increment the animation frame for the idle animation
			++idleAnimationFrame;

			// Cycle animation
			if (idleAnimationFrame / 5 >= IDLE_ANIMATION_FRAMES)
			{
				idleAnimationFrame = 0;
			}
		}
	}

	// Health bar size factor (adjust this value to change the health bar size)
	constexpr float healthBarWidthFactor = 0.6f;

	// Render health bar on top of the player's texture
	playerHpBarBack = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
						static_cast<int>(player1.getYPos() + player1.getWidth() - static_cast<float>(camera.y)) - 30,
						static_cast<int>(player1.getWidth() * healthBarWidthFactor), // Set the width to a fraction of the player's width
						PLAYER_HP_BAR_HEIGHT };

	float hpPercent = static_cast<float>(player1.getHp()) / static_cast<float>(player1.getHpMax());
	hpPercent = std::max(0.0f, std::min(hpPercent, 1.0f));

	int hpBarWidth = static_cast<int>(player1.getWidth() * healthBarWidthFactor * hpPercent); // Set the width to player's width * factor * hpPercent
	hpBarWidth = static_cast<int>(std::min(static_cast<float>(hpBarWidth), player1.getWidth() * healthBarWidthFactor)); // Make sure the hpBarWidth doesn't exceed the adjusted width

	playerHpBar = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
					static_cast<int>(player1.getYPos() + player1.getWidth() - static_cast<float>(camera.y)) - 30,
					hpBarWidth,
					PLAYER_HP_BAR_HEIGHT };
}

void GameEngine::renderEnemies() const
{
	// Render test enemy
	for (auto& e : enemies)
	{
		if (e.isAlive())
		{
			e.render(renderer, static_cast<float>(camera.x), static_cast<float>(camera.y));
		}
	}
}

void GameEngine::renderBullets() const
{
	if (bullet.isActive())
	{
		bullet.render(renderer, static_cast<float>(camera.x), static_cast<float>(camera.y));
	}
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

std::string GameEngine::rectToString(const SDL_Rect& rect) const
{
	return "SDL_Rect { x: " + std::to_string(rect.x) +
		", y: " + std::to_string(rect.y) +
		", w: " + std::to_string(rect.w) +
		", h: " + std::to_string(rect.h) +
		" }";
}

void GameEngine::checkPlayerEnemyCollision(const Enemy& enemy)   // NOLINT(clang-diagnostic-shadow)
{
	SDL_Rect playerPositionRect;
	playerPositionRect.x = static_cast<int>(player1.getXPos());
	playerPositionRect.y = static_cast<int>(player1.getYPos());
	playerPositionRect.w = PLAYER1_WIDTH - 60;		// temporary fix for player colliding with enemy false positive
	playerPositionRect.h = PLAYER1_HEIGHT;

	if (enemy.checkCollisionWith(playerPositionRect))
	{
		player1.takeDamage(10);
	}
}