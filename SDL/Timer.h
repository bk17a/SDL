#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>

class Timer
{
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();
	Uint32 getTicks();
	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks; // clock when timer started
	Uint32 pausedTicks; // clock when timer was paused

	// timer status
	bool paused;
	bool started;
};

#endif // TIMER_H_