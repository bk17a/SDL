#include <iostream>
#include "GameEngine.h"

using namespace std;

int main(int argc, char* args[])
{
	// Create game engine
	GameEngine game;

	// Run game
	game.run();

	// Clean up and exit
	game.close();

	return 0;
}