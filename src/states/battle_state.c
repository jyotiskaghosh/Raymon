#include <raylib.h>
#include <stdio.h>
#include "../state_stack.h"
#include "../UI/panel.h"
#include "../main.h"
#include "../poke_def.h"
#include "../pokemon.h"
#include "../music.h"
#include "../transition.h"
#include "../UI/progress_bar.h"
#include "../UI/selection.h"

#define PANEL (Rectangle){0, VIRTUAL_HEIGHT - 64, VIRTUAL_WIDTH, 64}

typedef enum {
    SLIDE_IN,
    OPENING_TEXT1,
    OPENING_TEXT2,
    BATTLE_MENU,
    RUN,
} State;

static State state;
static int frameCounter;

extern Pokemon playerPokemon;
static Pokemon opponentPokemon;

static Vector2 playerSpritePos;
static Vector2 opponentSpritePos;

static float playerCircleX;
static float opponentCircleX;

static ProgressBar playerHealthBar;
static ProgressBar opponentHealthBar;

static char renderHealthBars;

static Selection menu;

extern PokemonDef gPokemons[];
extern Texture2D gTextures[];
extern Music gMusic[];

extern Transition fadeTransition;

static void onSelectFight() {}; 
static void onSelectRun() { state = RUN; };

static void BattleStateInit(void *data) 
{
    PlayMusic(gMusic[BATTLE_MUSIC]);    
    
    PokeInit(&opponentPokemon, GetRandomValue(0, MAX_POKEMONS - 1));

    playerSpritePos = (Vector2){
        .x = -64,
        .y = VIRTUAL_HEIGHT - 128
    };

    opponentSpritePos = (Vector2){
        .x = VIRTUAL_WIDTH,
        .y = 8
    };

    playerCircleX = -68;    
    opponentCircleX = VIRTUAL_WIDTH + 32;

    playerHealthBar = (ProgressBar) {
        .bounds = (Rectangle){
            .x = VIRTUAL_WIDTH - 160, 
            .y = VIRTUAL_HEIGHT - 80,
            .width = 152,
            .height = 6
        },
        .color = (Color){189, 32, 32, 255},
        .value = playerPokemon.HP,
        .max = gPokemons[playerPokemon.ID].baseHP
    };

    opponentHealthBar = (ProgressBar) {
        .bounds = (Rectangle){
            .x = 8, 
            .y = 8,
            .width = 152,
            .height = 6
        },
        .color = (Color){189, 32, 32, 255},
        .value = opponentPokemon.HP,
        .max = gPokemons[opponentPokemon.ID].baseHP
    };

    menu = (Selection){
        .items = {
                (SelectionItem){"Fight", onSelectFight},
                (SelectionItem){"Run", onSelectRun}
            },
        .bounds = {.x = VIRTUAL_WIDTH - 64, .y = VIRTUAL_HEIGHT - 64, 64, 64},
        .itemsLength = 2
    };

    state = SLIDE_IN;
    frameCounter = 0;
    renderHealthBars = 0;
}

static void callback() { PopState(); }

static void BattleStateUpdate()
{
#define SLIDE_IN_FRAMES 60
    
    if (state == SLIDE_IN && frameCounter++ > SLIDE_IN_FRAMES) {
        state = OPENING_TEXT1;
        renderHealthBars = 1;
    }

    switch (state) {
    case SLIDE_IN:
        opponentSpritePos.x -= 96.0f/SLIDE_IN_FRAMES;
        playerSpritePos.x += 96.0f/SLIDE_IN_FRAMES;
        opponentCircleX -= 102.0f/SLIDE_IN_FRAMES;
        playerCircleX += 134.0f/SLIDE_IN_FRAMES;
        break;
    case OPENING_TEXT1:
        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_X))
            state = OPENING_TEXT2;
        break;
    case OPENING_TEXT2:
        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_X))
            state = BATTLE_MENU;
        break;
    case BATTLE_MENU:
        SelectionUpdate(&menu);
        break;
    case RUN:
        StartTransition(&fadeTransition, callback);
        break;
    default:
        break;
    }
}

static void BattleStateRender()
{
    ClearBackground((Color){214, 214, 214, 255});

    DrawEllipse(opponentCircleX, 60, 72, 24, (Color){45, 184, 45, 124});
    DrawEllipse(playerCircleX, VIRTUAL_HEIGHT - 64, 72, 24, (Color){45, 184, 45, 124});

    DrawTexture(gTextures[gPokemons[opponentPokemon.ID].battleSpriteFront], opponentSpritePos.x, opponentSpritePos.y, WHITE);
    DrawTexture(gTextures[gPokemons[playerPokemon.ID].battleSpriteBack], playerSpritePos.x, playerSpritePos.y, WHITE);

    PanelRender(PANEL);

    if (renderHealthBars) {
        ProgressBarRender(playerHealthBar);
        ProgressBarRender(opponentHealthBar);
    }

    char str[32];
    
    switch (state)
    {
    case OPENING_TEXT1:
        sprintf(str, "A wild %s appeared.", gPokemons[opponentPokemon.ID].name);
        DrawText(str, 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
        break;
    case OPENING_TEXT2:
        sprintf(str, "Go %s !", gPokemons[playerPokemon.ID].name);
        DrawText(str, 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
        break;
    case BATTLE_MENU:
        SelectionRender(&menu);
        break;
    case RUN:
        DrawText("Fled successfully.", 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
        break;
    default:
        break;
    }
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