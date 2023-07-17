#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include "Constants.h"
#include <iostream>
#include <string>
#include <sstream>

class Window
{
public:
	Window();

	bool init();
	SDL_Renderer* createRenderer();										// renderer from internal window
	void handleEvent(SDL_Event& e, SDL_Renderer* renderer);				// window events
	void free();														// deallocate internals

	int getWidth() const;
	int getHeight() const;

	bool hasMouseFocus();												// check for mouse focus
	bool hasKbFocus();													// check for keyboard focus
	bool isMin();														// check for minimized
	SDL_Window* getWindow() const; 

private:
	SDL_Window* window;													// window data
	int width, height;													// window dimensions
	bool mouseFocus, kbFocus, fullScreen, min;							// window focus


};

#endif // WINDOW_H