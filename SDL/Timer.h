#ifndef TIMER_H_
#define TIMER_H_

#include <SDL.h>

class Timer
{
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();
	Uint32 getTicks() const;
	bool isStarted() const;
	bool isPaused() const;

	float getDeltaTime() const;
	void update();
private:
	Uint32 startTicks; // clock when timer started
	Uint32 pausedTicks; // clock when timer was paused

	float deltaTime;
	Uint32 elapsedTicks;

	// timer status
	bool paused;
	bool started;
};

#endif // TIMER_H_