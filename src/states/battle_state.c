#include <raylib.h>
#include <stdio.h>
#include "../state_stack.h"
#include "../UI/panel.h"
#include "../main.h"
#include "../poke_def.h"
#include "../music.h"
#include "dialogue.h"

#define PANEL (Rectangle){0, VIRTUAL_HEIGHT - 64, VIRTUAL_WIDTH, 64}

typedef enum {
    SLIDE_IN,
    OPENING_TEXT,
    BATTLE_MENU,
} State;

static State state;
static int frameCounter;

extern Pokemon playerPokemon;
static Pokemon opponentPokemon;

static Vector2 playerSpritePos;
static Vector2 opponentSpritePos;

static float playerCircleX;
static float opponentCircleX;

extern Pokemon gPokemons[];
extern Texture2D gTextures[];
extern Music gMusic[];

static void BattleStateInit(void *data) 
{
    PlayMusic(gMusic[BATTLE_MUSIC]);    
    
    opponentPokemon = gPokemons[GetRandomValue(0, MAX_POKEMONS - 1)];

    opponentSpritePos = (Vector2){
        .x = VIRTUAL_WIDTH,
        .y = 8
    };

    playerSpritePos = (Vector2){
        .x = -64,
        .y = VIRTUAL_HEIGHT - 128
    };

    opponentCircleX = VIRTUAL_WIDTH + 32;
    playerCircleX = -68;
}

static void BattleStateUpdate()
{
#define SLIDE_IN_FRAMES 60
    
    if (state == SLIDE_IN && frameCounter++ > SLIDE_IN_FRAMES)
        state = OPENING_TEXT;

    switch (state) {
    case SLIDE_IN:
        opponentSpritePos.x -= 96.0f/SLIDE_IN_FRAMES;
        playerSpritePos.x += 96.0f/SLIDE_IN_FRAMES;
        opponentCircleX -= 102.0f/SLIDE_IN_FRAMES;
        playerCircleX += 134.0f/SLIDE_IN_FRAMES;
        break;
    case OPENING_TEXT:
        char str1[32], str2[32];
        sprintf(str1, "A wild %s appeared.", opponentPokemon.name);
        sprintf(str2, "Go %s!", playerPokemon.name);

        const char *lines[] = {str1, str2};
        
        StartDialogueQueue(lines, 2);

        state = BATTLE_MENU;
    default:
        break;
    }
}

static void BattleStateRender()
{
    ClearBackground((Color){214, 214, 214, 255});

    DrawEllipse(opponentCircleX, 60, 72, 24, (Color){45, 184, 45, 124});
    DrawEllipse(playerCircleX, VIRTUAL_HEIGHT - 64, 72, 24, (Color){45, 184, 45, 124});

    DrawTexture(gTextures[opponentPokemon.battleSpriteFront], opponentSpritePos.x, opponentSpritePos.y, WHITE);
    DrawTexture(gTextures[playerPokemon.battleSpriteBack], playerSpritePos.x, playerSpritePos.y, WHITE);

    PanelRender(PANEL);
}

static void BattleStateExit()
{
    StopMusic();
}

GameState battleState = {
    .init = BattleStateInit,
    .update = BattleStateUpdate,
    .render = BattleStateRender,
    .exit = BattleStateExit
};