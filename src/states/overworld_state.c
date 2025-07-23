#include <raylib.h>
#include "../state_stack.h"
#include "../music.h"
#include "../main.h"
#include "../entities/player.h"
#include "../globals.h"

#define GRASS_TILE (Rectangle){5 * TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE}
#define TALL_GRASS_TILE (Rectangle){TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE}

static void OverworldStateInit(void *data) 
{
    PlayerInit(VIRTUAL_WIDTH / 2 - PLAYER_WIDTH, VIRTUAL_HEIGHT / 2 - PLAYER_HEIGHT, DOWN);
}

static void OverworldStateEnter()
{
    PlayMusic(gMusic[FIELD_MUSIC]);
}

static void OverworldStateUpdate()
{
    PlayerUpdate();
}

static void OverworldStateRender()
{
    for (int i = 0; i < VIRTUAL_HEIGHT / TILE_SIZE + 1; i++)
        for (int j = 0; j < VIRTUAL_WIDTH / TILE_SIZE; j++)
            DrawTextureRec(gTextures[TILES], GRASS_TILE, (Vector2){j * TILE_SIZE, i * TILE_SIZE}, WHITE);

    for (int i = 10; i < VIRTUAL_HEIGHT / TILE_SIZE + 1; i++)
        for (int j = 0; j < VIRTUAL_WIDTH / TILE_SIZE; j++)
            DrawTextureRec(gTextures[TILES], TALL_GRASS_TILE, (Vector2){j * TILE_SIZE, i * TILE_SIZE}, WHITE);

    PlayerRender();
}

GameState overworldState = {
    .init = OverworldStateInit,
    .enter = OverworldStateEnter,
    .update = OverworldStateUpdate,
    .render = OverworldStateRender,
};