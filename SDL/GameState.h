#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <SDL.h>

class GameState
{
public:
	// state transitions
	virtual bool enter() = 0;
	virtual bool exit() = 0;

	// main loop functions
	virtual void handleEvent(SDL_Event& e) = 0;
	virtual void update() = 0;
	virtual void render(SDL_Renderer* renderer) = 0;

	// make sure to call child destructors
	virtual ~GameState() = default;
};

#endif // GAME_STATE_H