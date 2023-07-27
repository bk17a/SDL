#include "Timer.h"
#include <SDL_image.h>
#include <iostream>

using namespace std;

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	// start timer
	started = true;
	// unpause timer
	paused = false;

	// get the current clock time
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop()
{
	// stop timer
	started = false;
	// unpause timer
	paused = false;

	// clear tick
	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause()
{
	if (started && !paused)
	{
		// pause the timer
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause()
{
	if (paused && started)
	{
		// start the timer
		paused = false;

		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

Uint32 Timer::getTicks() const
{
	// the actua ltimer time
	Uint32 time = 0;

	// if time is running
	if (started)
	{
		// if time is paused
		if (paused)
		{
			// return the number of ticks when the timer was paused
			time = pausedTicks;
		}
		else
		{
			// return the current time minus started time
			time = SDL_GetTicks() - startTicks;
		}
	}
	return time;
}

bool Timer::isPaused() const
{
	return paused && started;
}

bool Timer::isStarted() const
{
	return started;
}