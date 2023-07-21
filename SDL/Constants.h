#ifndef CONSTANTS_H
#define CONSTANTS_H

// dimension of screen/window
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1050;

// dimensions of level
static constexpr int LEVEL_WIDTH = 3840;
static constexpr int LEVEL_HEIGHT = 2160;

// dimensions of the player sprite
static constexpr int PLAYER_WIDTH = 50;
static constexpr int PLAYER_HEIGHT = 50;

// dimensions of the player2 sprite
static constexpr int PLAYER2_WIDTH = 127;
static constexpr int PLAYER2_HEIGHT = 70;

static constexpr int PLAYER2RUN_HEIGHT = 90;

// maximum axis velocity of player
static constexpr int DOT_VEL = 1;

// amount of particles
static constexpr int TOTAL_PARTICLES = 20;

// animation frames
static constexpr int IDLE_ANIMATION_FRAMES = 5 * 5;
static constexpr int RUNNING_ANIMATION_FRAMES = 8 * 3;

#endif
