// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "GameEngine.h"

using namespace std;

GameEngine& GameEngine::getInstance()
{
	static GameEngine instance; // singleton instance
	return instance;
}

GameEngine::GameEngine() : running(true), font(nullptr), window(nullptr), renderer(nullptr),
player1Rect({ {{0, 0, 0, 0}} }), player1RunRect({ {{0, 0, 0, 0}} }),
playerHpBar({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }),
playerHpBarBack({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }),
playerXpBarBack({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }),
playerXpBar({ PLAYER_HP_BAR_X, PLAYER_HP_BAR_Y, PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT }),
enemy1WalkRect({ {{0, 0, 0, 0}} }), start(false),
hide(false),
pause(false) {}

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
	font = TTF_OpenFont("fonts/press_start.ttf", 20);
	if (font == nullptr)
	{
		cout << "Failed to load press_start font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	else
	{
		constexpr SDL_Color textColor{ 0, 0, 0, 255 };
		stringstream text;
		text.str("Press ENTER to start");
		if (!startText.loadFromRenderedText(text.str().c_str(), textColor, renderer, font))
		{
			cout << "Failed to render start text texture!\n";
			success = false;
		}

		text.str("Press SPACE to toggle auto shoot on/off");
		if (!helpText.loadFromRenderedText(text.str().c_str(), textColor, renderer, font))
		{
			cout << "Failed to render help text texture!\n";
			success = false;
		}

		text.str("YOU DIED!");
		if (!deathText.loadFromRenderedText(text.str().c_str(), textColor, renderer, font))
		{
			cout << "Failed to render death text texture!\n";
			success = false;
		}
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

	if (!enemy1WalkTex.loadFromFile("gfx/enemy1.png", renderer))
	{
		cout << "Unable to load enemy1 walk texture!\n";
		success = false;
	}
	else
	{
		for (int i = 0; i < ENEMY_NUM; ++i)
		{
			enemy1 = Enemy(renderer, &enemy1WalkTex);
			//enemy1.spawn();
			enemy1Vec.emplace_back(enemy1);
		}

		if (!enemy1.isAlive())
		{
			for (auto& e : enemy1Vec)
			{
				e.spawn();
			}
		}

		constexpr int walkingFrames = 6;
		for (int i = 0; i < walkingFrames; ++i)
		{
			enemy1WalkRect[i].x = i * ENEMY1RUN_WIDTH;
			enemy1WalkRect[i].y = 0;
			enemy1WalkRect[i].w = ENEMY1RUN_WIDTH;
			enemy1WalkRect[i].h = ENEMY1RUN_HEIGHT;
		}
	}

	if (!continueButtonTex.loadFromFile("gfx/continueButton.png", renderer))
	{
		cout << "Unable to load continue button texture!\n";
		success = false;
	}
	else
	{
		constexpr int yOffset = 15;
		const int xPos = ((SCREEN_WIDTH + continueButtonTex.getWidth()) / 2) - continueButtonTex.getWidth();
		const int yPos = ((SCREEN_HEIGHT - continueButtonTex.getHeight()) / 2) - continueButtonTex.getHeight() - yOffset;
		const Vector2 pos(static_cast<float>(xPos), static_cast<float>(yPos));
		continueButton = Button(renderer, &continueButtonTex, pos);
	}

	if (!restartButtonTex.loadFromFile("gfx/restartButton.png", renderer))
	{
		cout << "Unable to load continue button texture!\n";
		success = false;
	}
	else
	{
		const int xPos = (SCREEN_WIDTH - restartButtonTex.getWidth()) / 2;
		const int yPos = (SCREEN_HEIGHT - restartButtonTex.getHeight()) / 2;
		const Vector2 pos(static_cast<float>(xPos), static_cast<float>(yPos));
		restartButton = Button(renderer, &restartButtonTex, pos);
	}

	if (!quitButtonTex.loadFromFile("gfx/quitButton.png", renderer))
	{
		cout << "Unable to load continue button texture!\n";
		success = false;
	}
	else
	{
		constexpr int yOffset = 15;
		const int xPos = ((SCREEN_WIDTH + quitButtonTex.getWidth()) / 2) - quitButtonTex.getWidth();
		const int yPos = ((SCREEN_HEIGHT - quitButtonTex.getHeight()) / 2) + quitButtonTex.getHeight() + yOffset;
		const Vector2 pos(static_cast<float>(xPos), static_cast<float>(yPos));
		quitButton = Button(renderer, &quitButtonTex, pos);
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

	if (pause) // Assuming `pause` is a flag indicating if the game is paused
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Semi-transparent black overlay
		constexpr SDL_Rect overlayRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderFillRect(renderer, &overlayRect);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}

	// render game elements
	if (player1.isAlive() && player1Run.isAlive())
	{
		renderPlayer();
		renderEnemies();
		renderBullets();
	}

	renderText();

	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 200); // Grey color with less opacity
	SDL_RenderFillRect(renderer, &playerHpBarBack);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &playerHpBar);

	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 200); // Grey color with less opacity
	SDL_RenderFillRect(renderer, &playerXpBarBack);

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &playerXpBar);

	renderXpBarLines();

	renderPauseScreen();
	renderDeathScreen();
}

void GameEngine::update()
{
	if (start)
	{
		if (!pause)
		{
			if (player1.isAlive())
			{
				player1.update();
				player1.move();
				player1Run.update();
				player1Run.move();

				updatePlayerLevel();
				updateEnemies();
				checkPlayerEnemyCollision();
				updateCollision();
				updateGUI();
				updateCamera();
				updateBullets();
				updateEnemiesKilled();

				// Check if any enemies are still alive
				bool anyEnemiesAlive = false;
				for (auto& e : enemy1Vec)
				{
					if (e.isAlive())
					{
						anyEnemiesAlive = true;
						break; // No need to continue checking if one enemy is alive
					}
				}

				// If no enemies are alive, spawn a new wave
				if (!anyEnemiesAlive)
				{
					spawnEnemyWave(5);
				}
			}
		}
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

		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_RETURN) start = true;
			if (e.key.keysym.sym == SDLK_SPACE) hide = true;
			if (e.key.keysym.sym == SDLK_ESCAPE) pause = !pause;
		}

		if (pause || !player1.isAlive())
		{
			if (continueButton.handleEvents(e))
			{
				pause = !pause;
			}

			if (restartButton.handleEvents(e))
			{
				restartGame();
				pause = !pause;
				start = false;
			}

			if (quitButton.handleEvents(e))
			{
				running = false;
			}
		}

		// for bullet auto shoot time
		currentFrameTime = SDL_GetTicks();
		frameTime = currentFrameTime - lastShotTime;

		// window events
		windowObj.handleEvent(e, renderer);

		// player movement
		if (player1.isAlive())
		{
			if (!pause)
			{
				player1.handleEvent(e);
				player1Run.handleEvent(e);
			}
		}

		if (start)
		{
			if (!pause)
			{
				bullet.handleEvent(e, player1.getPlayerPos());
			}
		}

		if (bullet.isAutoShootingEnabled() && (!enemies.empty() || !enemy1Vec.empty()) && (frameTime >= shootInterval) && player1.isAlive())
		{
			if (!pause)
			{
				bullet.shoot(player1.getPlayerPos());
				lastShotTime = currentFrameTime; // Update the last shot time
			}
		}
	}

	// Ensure bullets continue to shoot even if no events are processed
	if (bullet.isAutoShootingEnabled() && (!enemies.empty() || !enemy1Vec.empty()) && (currentFrameTime - lastShotTime >= shootInterval) && player1.isAlive())
	{
		if (!pause)
		{
			bullet.shoot(player1.getPlayerPos());
			lastShotTime = currentFrameTime; // Update the last shot time
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
	enemy1WalkTex.free();
	startText.free();

	enemies.clear();
	enemy1Vec.clear();

	TTF_CloseFont(font);
	font = nullptr;

	windowObj.free();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameEngine::updateCollision()
{
	// Check collision between enemies
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < enemies.size(); ++j)
		{
			if (enemies[i].checkCollisionWith(enemies[j].p))
			{
				handleCollision(enemies[i], enemies[j]);
			}
		}
	}

	// Check collision between enemy1Vec elements and enemies
	for (auto& enemy : enemies)
	{
		for (auto& enemy1Walk : enemy1Vec)
		{
			if (enemy.checkCollisionWith(enemy1Walk.p))
			{
				handleCollision(enemy, enemy1Walk);
			}
		}
	}

	// Check collision between enemy1Vec elements
	for (size_t i = 0; i < enemy1Vec.size(); ++i)
	{
		for (size_t j = i + 1; j < enemy1Vec.size(); ++j)
		{
			if (enemy1Vec[i].checkCollisionWith(enemy1Vec[j].p))
			{
				handleCollision(enemy1Vec[i], enemy1Vec[j]);
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
		Enemy::move(e, player1);
	}

	for (auto& enemy : enemy1Vec)
	{
		// Update enemy movement toward player
		Enemy::move(enemy, player1);
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

	for (const auto& e : enemy1Vec)
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
	std::vector<size_t> enemy1Killed;
	std::vector<size_t> enemy1WalkKilled;

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (bullet.isActive() && enemies[i].checkCollisionWith(bullet.p))
		{
			enemies[i].takeDamage(50);
			bullet.reload();

			if (!enemies[i].isAlive())
			{
				player1.increaseScore(100);
				player1.increaseExp(enemies[i].getExpPoints());
				enemiesKilled.emplace_back(i);
			}
		}
	}

	for (size_t i = 0; i < enemy1Vec.size(); ++i)
	{
		if (bullet.isActive() && enemy1Vec[i].checkCollisionWith(bullet.p))
		{
			enemy1Vec[i].takeDamage(50);
			bullet.reload();

			if (!enemy1Vec[i].isAlive())
			{
				player1.increaseScore(100);
				player1.increaseExp(enemy1Vec[i].getExpPoints());
				enemy1WalkKilled.emplace_back(i);
			}
		}
	}

	// Remove the marked enemies using the erase-remove idiom
	for (auto it = enemiesKilled.rbegin(); it != enemiesKilled.rend(); ++it)
	{
		enemies.erase(enemies.begin() + *it);
	}

	for (auto it = enemy1WalkKilled.rbegin(); it != enemy1WalkKilled.rend(); ++it)
	{
		enemy1Vec.erase(enemy1Vec.begin() + *it);
	}

	// clear the vectors
	enemiesKilled.clear();
	enemy1WalkKilled.clear();
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
			const SDL_Rect* currentRunClip = &player1RunRect[runAnimationFrame / PLAYER_RUNNING_ANIMATION_FRAMES];
			player1Run.renderAnimated(renderer, currentRunClip, static_cast<float>(camera.x), static_cast<float>(camera.y), NULL, nullptr, player1.getFlipType());

			// Increment the animation frame for the running animation
			++runAnimationFrame;

			// Cycle animation
			if (runAnimationFrame / 4 >= PLAYER_RUNNING_ANIMATION_FRAMES)
			{
				runAnimationFrame = 0;
			}
		}
		else
		{
			const SDL_Rect* currentIdleClip = &player1Rect[idleAnimationFrame / PLAYER_IDLE_ANIMATION_FRAMES];
			player1.renderAnimated(renderer, currentIdleClip, static_cast<float>(camera.x), static_cast<float>(camera.y), NULL, nullptr, player1.getFlipType());

			// Increment the animation frame for the idle animation
			++idleAnimationFrame;

			// Cycle animation
			if (idleAnimationFrame / 5 >= PLAYER_IDLE_ANIMATION_FRAMES)
			{
				idleAnimationFrame = 0;
			}
		}
	}

	// Health bar size factor (adjust this value to change the health bar size)
	constexpr float healthBarWidthFactor = 0.5f;

	// Render health bar on top of the player's texture
	playerHpBarBack = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
						static_cast<int>(player1.getYPos() - static_cast<float>(camera.y)) - PLAYER_HP_BAR_HEIGHT - 10, // Adjust the Y coordinate to be above the player
						static_cast<int>(player1.getWidth() * healthBarWidthFactor),									// Set the width to a fraction of the player's width
						PLAYER_HP_BAR_HEIGHT };

	float hpPercent = static_cast<float>(player1.getHp()) / static_cast<float>(player1.getHpMax());
	hpPercent = std::max(0.0f, std::min(hpPercent, 1.0f));

	int hpBarWidth = static_cast<int>(player1.getWidth() * healthBarWidthFactor * hpPercent);										// Set the width to player's width * factor * hpPercent
	hpBarWidth = static_cast<int>(std::min(static_cast<float>(hpBarWidth), player1.getWidth() * healthBarWidthFactor)); // Make sure the hpBarWidth doesn't exceed the adjusted width

	playerHpBar = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
					 static_cast<int>(player1.getYPos() - static_cast<float>(camera.y)) - PLAYER_HP_BAR_HEIGHT - 10, // Same adjustment for the Y coordinate
					hpBarWidth,
					PLAYER_HP_BAR_HEIGHT };

	// Experience bar size factor (adjust this value to change the XP bar size)
	constexpr float xpBarWidthFactor = 0.5f;

	// Calculate position for the XP bar below the player
	const float xpBarYPos = (player1.getYPos() + player1.getHeight() + 10) - static_cast<float>(camera.y);

	// Render XP bar below the player's texture
	playerXpBarBack = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
						static_cast<int>(xpBarYPos),
						static_cast<int>(player1.getWidth() * xpBarWidthFactor),
						PLAYER_HP_BAR_HEIGHT };

	float xpPercent = static_cast<float>(player1.getExp()) / static_cast<float>(player1.getMaxExp());
	xpPercent = std::max(0.0f, std::min(xpPercent, 1.0f));

	int xpBarWidth = static_cast<int>(player1.getWidth() * xpBarWidthFactor * xpPercent);
	xpBarWidth = static_cast<int>(std::min(static_cast<float>(xpBarWidth), player1.getWidth() * xpBarWidthFactor));

	playerXpBar = { static_cast<int>(player1.getXPos() - static_cast<float>(camera.x)),
					static_cast<int>(xpBarYPos),
					xpBarWidth,
					PLAYER_HP_BAR_HEIGHT };
}

void GameEngine::renderEnemies()
{
	// Render test enemy
	for (auto& e : enemies)
	{
		if (e.isAlive())
		{
			e.render(renderer, static_cast<float>(camera.x), static_cast<float>(camera.y));
		}
	}

	for (const auto& enemy : enemy1Vec)
	{
		if (enemy.isAlive())
		{
			const SDL_Rect* currentWalkClip = &enemy1WalkRect[enemyWalkAnimationFrame / ENEMY_WALK_ANIMATION_FRAMES];
			enemy.renderAnimated(renderer, currentWalkClip, static_cast<float>(camera.x), static_cast<float>(camera.y), NULL, nullptr, enemy.getFlipType());
		}
	}

	// Increment animation frame for the idle animation
	++enemyWalkAnimationFrame;
	if (enemyWalkAnimationFrame / 6 >= ENEMY_WALK_ANIMATION_FRAMES)
	{
		enemyWalkAnimationFrame = 0;
	}
}

void GameEngine::renderBullets() const
{
	if (bullet.isActive())
	{
		bullet.render(renderer, static_cast<float>(camera.x), static_cast<float>(camera.y));
	}
}

void GameEngine::checkPlayerEnemyCollision()
{
	SDL_Rect playerPositionRect;
	playerPositionRect.x = static_cast<int>(player1.getXPos());
	playerPositionRect.y = static_cast<int>(player1.getYPos());
	playerPositionRect.w = PLAYER1_WIDTH - 60;		// temporary fix for player colliding with enemy false positive
	playerPositionRect.h = PLAYER1_HEIGHT;

	SDL_Rect enemy1PositionRect;
	enemy1PositionRect.x = static_cast<int>(enemy1.getPosX());
	enemy1PositionRect.y = static_cast<int>(enemy1.getPosY());
	enemy1PositionRect.w = ENEMY1RUN_WIDTH;
	enemy1PositionRect.h = ENEMY1RUN_HEIGHT;

	for (auto& e : enemies)
	{
		if (e.checkCollisionWith(playerPositionRect))
		{
			player1.takeDamage(5);
		}
	}

	for (auto& e : enemy1Vec)
	{
		e.setRect(enemy1PositionRect);
		if (e.checkCollisionWith(playerPositionRect))
		{
			player1.takeDamage(5);
		}
	}
}

bool GameEngine::checkCollision(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	// Check for horizontal collision
	const bool xCollision = (rectA.x + rectA.w) >= rectB.x && (rectB.x + rectB.w) >= rectA.x;

	// Check for vertical collision
	const bool yCollision = (rectA.y + rectA.h) >= rectB.y && (rectB.y + rectB.h) >= rectA.y;

	// If there is both horizontal and vertical collision, the rectangles overlap
	return xCollision && yCollision;
}

void GameEngine::handleCollision(Enemy& object1, Enemy& object2)
{
	// Calculate the direction of the collision between objects
	float directionX = object1.getPosX() - object2.getPosX();
	float directionY = object1.getPosY() - object2.getPosY();

	// Normalize the direction vector
	const float length = sqrt(directionX * directionX + directionY * directionY);
	directionX /= length;
	directionY /= length;

	// Calculate the maximum distance the objects can move back
	constexpr float maxDistance = 2; // Change this value to adjust the maximum distance

	// Move the colliding objects one step back in the opposite direction, but limit the distance
	const float distanceX = directionX * maxDistance;
	const float distanceY = directionY * maxDistance;

	object1.setPosX(object1.getPosX() + distanceX);
	object1.setPosY(object1.getPosY() + distanceY);

	object2.setPosX(object2.getPosX() - distanceX);
	object2.setPosY(object2.getPosY() - distanceY);
}

void GameEngine::renderText() const
{
	if (!start)
	{
		startText.render((SCREEN_WIDTH - startText.getWidth()) / 2, (SCREEN_HEIGHT - startText.getHeight()) / 2, renderer);
	}
	else
	{
		scoreText.render(0, 0, renderer);
		levelText.render(0, scoreText.getHeight(), renderer);
	}

	if (!hide)
	{
		helpText.render(0, SCREEN_HEIGHT - 50, renderer);
	}

	if (!player1.isAlive())
	{
		deathText.render((SCREEN_WIDTH - deathText.getWidth()) / 2, (SCREEN_HEIGHT - deathText.getHeight()) / 2, renderer);
	}
}

void GameEngine::renderPauseScreen() const
{
	if (pause)
	{
		restartButton.render(renderer);
		continueButton.render(renderer);
		quitButton.render(renderer);
	}
}

void GameEngine::renderDeathScreen()
{
	if (!pause && !player1.isAlive())
	{
		const int yOffset = (restartButtonTex.getHeight() * 3) - 30;
		const int xPos = (SCREEN_WIDTH - restartButtonTex.getWidth()) / 2;
		const int yPos = ((SCREEN_HEIGHT - restartButtonTex.getHeight()) / 2) + yOffset;
		const Vector2 pos(static_cast<float>(xPos), static_cast<float>(yPos));
		restartButton = Button(renderer, &restartButtonTex, pos);
		restartButton.render(renderer);

		const int xOffset = quitButtonTex.getWidth();
		const int yOffset2 = quitButtonTex.getHeight() + 15;
		const int xPos2 = ((SCREEN_WIDTH + quitButtonTex.getWidth()) / 2) - xOffset;
		const int yPos2 = ((SCREEN_HEIGHT - quitButtonTex.getHeight()) / 2) + yOffset2;
		const Vector2 pos2(static_cast<float>(xPos2), static_cast<float>(yPos2));
		quitButton = Button(renderer, &quitButtonTex, pos2);
		quitButton.render(renderer);
	}
}

void GameEngine::restartGame()
{
	// reset default enemy
	enemies.clear();
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

	// reset other enemy
	enemy1Vec.clear();
	for (int i = 0; i < ENEMY_NUM; ++i)
	{
		enemy1 = Enemy(renderer, &enemy1WalkTex);
		enemy1Vec.emplace_back(enemy1);
	}

	if (!enemy1.isAlive())
	{
		for (auto& e : enemy1Vec)
		{
			e.spawn();
		}
	}

	constexpr int walkingFrames = 6;
	for (int i = 0; i < walkingFrames; ++i)
	{
		enemy1WalkRect[i].x = i * ENEMY1RUN_WIDTH;
		enemy1WalkRect[i].y = 0;
		enemy1WalkRect[i].w = ENEMY1RUN_WIDTH;
		enemy1WalkRect[i].h = ENEMY1RUN_HEIGHT;
	}

	// spawn player at default location
	player1.spawn();
	player1Run.spawn();

	player1.resetScore();
	player1.resetHealth();
	player1.resetExp();
	player1.resetLevel();
	player1.resetMaxExp();

	bullet.setActive(false);
	bullet.disableAutoShooting();

	hide = false;
}

void GameEngine::updatePlayerLevel()
{
	if (player1.levelUp())
	{
		player1.increaseMaxExp(150);
	}
}

void GameEngine::renderXpBarLines() const
{
	// Number of lines you want to display on the XP bar
	constexpr int numLines = 5;
	const int lineSpacing = playerXpBarBack.w / numLines;

	// Render lines on the XP bar background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 1; i < numLines; ++i)
	{
		constexpr int lineHeight = 4;
		const int xPos = playerXpBarBack.x + i * lineSpacing;
		const int startY = playerXpBarBack.y + (PLAYER_HP_BAR_HEIGHT - lineHeight) / 2; // Adjust the starting Y position
		const int endY = startY + lineHeight;											// Calculate the ending Y position
		SDL_RenderDrawLine(renderer, xPos, startY, xPos, endY);
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

			// score text
			stringstream score;

			// level text
			stringstream level;

			// Game loop
			while (isRunning())
			{
				currentFrameTime = SDL_GetTicks();
				frameTime = currentFrameTime - prevFrameTime;

				// event handler
				handleEvents();

				// constants for frame rate limiting
				constexpr int TARGET_FPS = 170;
				constexpr int SCREEN_TICKS_PER_FRAME = 1000 / TARGET_FPS;

				// Calculate the time taken for the frame
				const int delayTime = SCREEN_TICKS_PER_FRAME - frameTime;
				// NOLINT(cppcoreguidelines-narrowing-conversions)
				if (delayTime > 0)
				{
					SDL_Delay(delayTime);
				}

				prevFrameTime = currentFrameTime;

				// Calculate average FPS
				const float avgFPS = static_cast<float>(countedFrames) / (static_cast<float>(fpsTimer.getTicks()) /
					1000.0f);

				// update function
				update();

				if (!windowObj.isMin())
				{
					// Render FPS
					if (avgFPS > 0)
					{
						// Render fps text
						timeText.str("");
						timeText << "FPS: " << fixed << setprecision(0) << avgFPS;
						if (!fpsTexture.loadFromRenderedText(timeText.str().c_str(), textColor, renderer, font))
						{
							cout << "Unable to render FPS texture!\n";
						}
					}

					// Render score
					score.str("");
					score << "Score: " << player1.getScore();
					if (!scoreText.loadFromRenderedText(score.str().c_str(), textColor, renderer, font))
					{
						cout << "Failed to render score text texture!\n";
					}

					// Render level
					level.str("");
					level << "Level: " << player1.getLevel();
					if (!levelText.loadFromRenderedText(level.str().c_str(), textColor, renderer, font))
					{
						cout << "Failed to render level text texture!\n";
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

void GameEngine::spawnEnemyWave(const int numEnemiesToSpawn)
{
	// Get the current time
	currentFrameTime = SDL_GetTicks();

	// Check if it's time to spawn a new enemy and if the maximum number of enemies is not reached
	if (currentFrameTime - lastSpawnTime >= spawnInterval && enemy1Vec.size() < MAX_ENEMIES)
	{
		// Spawn multiple enemies
		for (int i = 0; i < numEnemiesToSpawn; ++i)
		{
			enemy1 = Enemy(renderer, &enemy1WalkTex);
			enemy1Vec.emplace_back(enemy1);
		}

		// Update the last spawn time
		lastSpawnTime = currentFrameTime;
	}

	if (!enemy1.isAlive())
	{
		for (auto& e : enemy1Vec)
		{
			e.spawn();
		}
	}

	constexpr int walkingFrames = 6;
	for (int i = 0; i < walkingFrames; ++i)
	{
		enemy1WalkRect[i].x = i * ENEMY1RUN_WIDTH;
		enemy1WalkRect[i].y = 0;
		enemy1WalkRect[i].w = ENEMY1RUN_WIDTH;
		enemy1WalkRect[i].h = ENEMY1RUN_HEIGHT;
	}
}