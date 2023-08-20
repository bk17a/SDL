#include <iostream>
#include "GameEngine.h"

int main(int argc, char* argv[])
{
	// Create game engine
	GameEngine game;

	// Run game
	game.run();

	// Clean up and exit
	game.close();

	return 0;
}