#include "Window.h"

using namespace std;

Window::Window()
{
	window = nullptr;				// initializing all variables
	width = 0;
	height = 0;
	mouseFocus = false;
	kbFocus = false;
	fullScreen = false;
	min = false;
}

bool Window::init()
{
	bool success = true;

	// create window
	window = SDL_CreateWindow("GAME V0.1,", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		cout << "Failed to create window!\n";
		success = false;
	}
	else
	{
		mouseFocus = true;
		kbFocus = true;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}

	return success;
}

SDL_Renderer* Window::createRenderer() const
{
	return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event& e, SDL_Renderer* renderer)
{
	if (e.type == SDL_WINDOWEVENT) // window event occured
	{
		// caption update flag
		bool updateCaption = false;

		switch (e.window.event)
		{
			// get new dimensions and repain on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			width = e.window.data1;
			height = e.window.data2;
			SDL_RenderPresent(renderer);
			break;
			// repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(renderer);
			break;
			// mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mouseFocus = true;
			updateCaption = true;
			break;
			// mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mouseFocus = false;
			updateCaption = true;
			break;
			// keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			kbFocus = true;
			updateCaption = true;
			break;
			// lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			kbFocus = false;
			updateCaption = true;
			break;
			// window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			min = true;
			break;
			// window maximeized
		case SDL_WINDOWEVENT_MAXIMIZED:

		case SDL_WINDOWEVENT_RESTORED:
			min = false;
			break;
		default:
			break;
		}

		//Update window caption with new data
		if (updateCaption)
		{
			stringstream caption;
			caption << "GAME V0.1 - MouseFocus:" << ((mouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((kbFocus) ? "On" : "Off");
			SDL_SetWindowTitle(window, caption.str().c_str());
		}
	}
	// enter exit full screen on f11 key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)
	{
		if (fullScreen)
		{
			SDL_SetWindowFullscreen(window, SDL_FALSE);
			fullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(window, SDL_TRUE);
			fullScreen = true;
			min = false;
		}
	}
}

void Window::free()
{
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	mouseFocus = false;
	kbFocus = false;
	width = 0;
	height = 0;
}

int Window::getWidth() const
{
	return width;
}

int Window::getHeight() const
{
	return height;
}

bool Window::hasKbFocus()
{
	return kbFocus;
}

bool Window::hasMouseFocus()
{
	return mouseFocus;
}

bool Window::isMin()
{
	return min;
}

SDL_Window* Window::getWindow() const {
	return window;
}