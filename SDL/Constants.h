#ifndef CONSTANTS_H
#define CONSTANTS_H

// dimension of screen/window
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1050;

// dimensions of level
static constexpr int LEVEL_WIDTH = 3840;
static constexpr int LEVEL_HEIGHT = 2160;

// dimensions of the player1 sprite
static constexpr int PLAYER1_WIDTH = 127;
static constexpr int PLAYER1_HEIGHT = 70;

static constexpr int PLAYER1RUN_HEIGHT = 90;

// maximum axis velocity of player
static constexpr int DOT_VEL = 1;

// amount of particles
static constexpr int TOTAL_PARTICLES = 20;

// animation frames
static constexpr int IDLE_ANIMATION_FRAMES = 5 * 5;
static constexpr int RUNNING_ANIMATION_FRAMES = 8 * 3;

// dimensions of enemy
static constexpr int ENEMY_WIDTH = 50;
static constexpr int ENEMY_HEIGHT = 50;


#endif