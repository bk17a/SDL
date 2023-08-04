#include "Particles.h"

Particles::Particles()
{
	xPos = 0;
	yPos = 0;
	frame = 0;
	texture = nullptr;
}

bool Particles::isDead() const
{
	return frame > 10;
}