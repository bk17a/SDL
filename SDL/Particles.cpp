#include "Particles.h"

Particles::Particles() 
{
	xPos = 0;
	yPos = 0;
	frame = 0;
	texture = NULL;
}

//Particles::Particles(int x, int y)
//{
//	texture = NULL;
//	// set offsets
//	xPos = x - 5 + (rand() % 25);
//	yPos = y - 5 + (rand() % 25);
//
//	// initialize animation
//	frame = rand() % 5;
//
//	
//
//	// set type
//	switch (rand() % 3)
//	{
//	case 0:
//		texture = &redTex;
//		break;
//	case 1:
//		texture = &greenTex;
//		break;
//	case 2:
//		texture = &blueTex;
//		break;
//	default:
//		break;
//	}
//}

//void Particles::render(SDL_Renderer* renderer)
//{
//	// show image
//	texture->render(xPos, yPos, PLAYER_WIDTH, PLAYER_HEIGHT, renderer);
//
//	// show shimmer
//	if (frame % 2 == 0)
//	{
//		shimmerTex.render(xPos, yPos, PLAYER_WIDTH, PLAYER_HEIGHT, renderer);
//	}
//	frame++;
//}

bool Particles::isDead()
{
	return frame > 10;
}