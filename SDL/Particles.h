#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include "TextureManager.h"
#include "Constants.h"

class Particles 
{
public:
	Particles();											// default constructor
	Particles(int x, int y);		// initialize position and animation
	void render(SDL_Renderer* renderer);					// render particles
	bool isDead();											// check to see if particles are dead

private:
	int xPos, yPos;					// offsets
	int frame;						// frame of animation
	TextureManager* texture;		// type of particle
};

#endif