#ifndef CONSTANTS_H
#define CONSTANTS_H

// dimension of screen/window
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1050;

// dimensions of level
static constexpr int LEVEL_WIDTH = 3840;
static constexpr int LEVEL_HEIGHT = 2160;

// dimensions of the player1 sprite
static constexpr int PLAYER1_WIDTH = 128;
static constexpr int PLAYER1_HEIGHT = 70;

static constexpr int PLAYER1RUN_HEIGHT = 70;

// maximum axis velocity of player
static constexpr int PLAYER_VEL = 2;

// amount of particles
static constexpr int TOTAL_PARTICLES = 20;

// animation frames
static constexpr int PLAYER_IDLE_ANIMATION_FRAMES = 5 * 5;			// multiply to control speed of animation
static constexpr int PLAYER_RUNNING_ANIMATION_FRAMES = 4 * 3;

// dimensions of enemy
static constexpr int ENEMY_WIDTH = 60;
static constexpr int ENEMY_HEIGHT = 60;

// maximum axis velocity of enemy
static constexpr int ENEMY_VEL = 1;

// number of enemies
static constexpr int ENEMY_NUM = 3;

// bullet vel
static constexpr int BULLET_SPEED = 10;

// hp bar
static constexpr int PLAYER_HP_BAR_WIDTH = 300;
static constexpr int PLAYER_HP_BAR_HEIGHT = 5;
static constexpr int PLAYER_HP_BAR_X = 0;
static constexpr int PLAYER_HP_BAR_Y = 0;

// enemy1
static constexpr int ENEMY1_WIDTH = 192;
static constexpr int ENEMY1_HEIGHT = 88;

static constexpr int ENEMY_IDLE_ANIMATION_FRAMES = 4 * 5;

#endif