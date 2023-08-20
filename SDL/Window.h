// ReSharper disable CppUnusedIncludeDirective
#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <SDL.h>
#include <sstream>
#include <string>
#include "Constants.h"

class Window
{
public:
	Window();

	bool init();
	SDL_Renderer* createRenderer() const;										// renderer from internal window
	void handleEvent(const SDL_Event& e, SDL_Renderer* renderer);				// window events
	void free();														// deallocate internals

	int getWidth() const;
	int getHeight() const;

	bool hasMouseFocus() const;												// check for mouse focus
	bool hasKbFocus() const;													// check for keyboard focus
	bool isMin() const;														// check for minimized
	SDL_Window* getWindow() const;

private:
	SDL_Window* window;

	// window dimensions
	int width;
	int height;

	// window focus
	bool mouseFocus;
	bool kbFocus;
	bool fullScreen;
	bool min;
};

#endif // WINDOW_H