#include "player.h"
#include <raylib.h>
#include "../transition.h"
#include "../state_stack.h"

#define FRAMES 30

#define PLAYER_TILE_WIDTH 32
#define PLAYER_TILE_HEIGHT 48

Player player;

extern Texture2D gTextures[];

extern Transition fadeTransition;
extern GameState battleState;

static int frameCounter;

void PlayerInit(float x, float y, Direction direction)
{
    player.x = x;
    player.y = y;
    player.direction = direction;
}

void PlayerIdleStateUpdate()
{
    if (IsKeyDown(KEY_LEFT)) {
        player.direction = LEFT;
        player.state = WALKING;
    } else if (IsKeyDown(KEY_RIGHT)) {
        player.direction = RIGHT;
        player.state = WALKING;
    } else if (IsKeyDown(KEY_UP)) {
        player.direction = UP;
        player.state = WALKING;
    } else if (IsKeyDown(KEY_DOWN)) {
        player.direction = DOWN;
        player.state = WALKING;
    }
}

void PlayerIdleStateRender()
{
    switch (player.direction) {
    case DOWN:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){0, 0, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case LEFT:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){0, 1 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case RIGHT:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){0, 2 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case UP:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){0, 3 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    default:
        break;
    }
}

static void Callback() { PushState(&battleState, 0); }

static void CheckForEncounter()
{
    if (player.y >= TILE_SIZE * 10 - PLAYER_HEIGHT && GetRandomValue(0, 9) == 0)
        StartTransition(&fadeTransition, Callback);     
}

void PlayerWalkingStateUpdate()
{
    if (frameCounter++ == FRAMES) {
        CheckForEncounter();
        player.state = IDLE;
        frameCounter = 0;

        return;
    }

    float move = (float) TILE_SIZE / FRAMES;

    switch (player.direction) {
    case DOWN: 
        player.y += move;
        break;
    case LEFT: 
        player.x -= move;
        break;
    case RIGHT: 
        player.x += move;
        break;
    case UP: 
        player.y -= move;
        break;
    default:
        break;
    }
}

void PlayerWalkingStateRender()
{
    int shift = frameCounter * 4 / FRAMES;

    switch (player.direction) {
    case DOWN:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){shift * PLAYER_TILE_WIDTH, 0, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case LEFT:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){shift * PLAYER_TILE_WIDTH, 1 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case RIGHT:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){shift * PLAYER_TILE_WIDTH, 2 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    case UP:
        DrawTexturePro(gTextures[PLAYER], (Rectangle){shift * PLAYER_TILE_WIDTH, 3 * PLAYER_TILE_HEIGHT, PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT}, (Rectangle){player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        break;
    default:
        break;
    }
}

void PlayerUpdate()
{
    if (player.state == IDLE)
        PlayerIdleStateUpdate();
    else
        PlayerWalkingStateUpdate();
}

void PlayerRender()
{
    if (player.state == IDLE)
        PlayerIdleStateRender();
    else
        PlayerWalkingStateRender();
}