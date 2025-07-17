#ifndef PLAYER_H
#define PLAYER_H

#include "../poke_def.h"

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 24

typedef enum {
    DOWN,
    LEFT,
    RIGHT,
    UP
} Direction;

typedef enum {
    IDLE,
    WALKING
} PlayerState;

typedef struct {
    PokemonDef pokemon;
    float x, y;
    Direction direction;
    PlayerState state;    
} Player;

void PlayerInit(float x, float y, Direction direction);
void PlayerUpdate();
void PlayerRender();

#endif