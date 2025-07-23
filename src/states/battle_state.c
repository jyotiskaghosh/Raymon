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
#include "../battle/battle_sprite.h"
#include "../globals.h"
#include "../transitions/transitions.h"

#define PANEL (Rectangle){0, VIRTUAL_HEIGHT - 64, VIRTUAL_WIDTH, 64}

typedef enum {
    SLIDE_IN,
    OPENING_TEXT1,
    OPENING_TEXT2,
    BATTLE_MENU,
    RUN,
    PLAYER_MOVE,
    OPPONENT_MOVE,
    VICTORY,
    DEFEAT
} State;

typedef enum {
    ATTACK_START,
    ATTACK_ANIM,
    ATTACK_DAMAGE,
    ATTACK_DONE
} AttackState;

static State state;
static AttackState attackState;

static int frameCounter;

extern Pokemon playerPokemon;
static Pokemon opponentPokemon;

static BattleSprite playerPokemonSprite;
static BattleSprite opponentPokemonSprite;

static float playerCircleX;
static float opponentCircleX;

static ProgressBar playerHealthBar;
static ProgressBar opponentHealthBar;

static char renderHealthBars;

static Selection menu;

static int damage;
static int moveCounter;

static void onSelectFight() { 
    Pokemon *attacker, *defender;
    if (playerPokemon.speed > opponentPokemon.speed) {
        state = PLAYER_MOVE;
    } else if (opponentPokemon.speed > playerPokemon.speed) {
        state = OPPONENT_MOVE;
    } else {
        if (GetRandomValue(0, 1) == 0) state = PLAYER_MOVE;
        else state = OPPONENT_MOVE;
    }
}; 

static void onSelectRun() { state = RUN; };

static void BattleStateInit(void *data) 
{
    PlayMusic(gMusic[BATTLE_MUSIC]);    
    
    PokeInit(&opponentPokemon, GetRandomValue(0, MAX_POKEMONS - 1));

    playerPokemonSprite = (BattleSprite){
        .ID = gPokemons[playerPokemon.ID].battleSpriteBack,
        .x = -64,
        .y = VIRTUAL_HEIGHT - 128,
        .opacity = 255
    };

    opponentPokemonSprite = (BattleSprite){
        .ID = gPokemons[opponentPokemon.ID].battleSpriteFront,
        .x = VIRTUAL_WIDTH,
        .y = 8,
        .opacity = 255
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
        .value = &playerPokemon.HP,
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
        .value = &opponentPokemon.HP,
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
    attackState = ATTACK_START;
    frameCounter = 0;
    renderHealthBars = 0;
    moveCounter = 0;
}

static int Damage(Pokemon attacker, Pokemon defender) {
    damage = (attacker.attack - defender.defense) > 1 ? (attacker.attack - defender.defense) : 1;
}

static void callback() { PopState(); }

static void BattleStateUpdate()
{
#define SLIDE_IN_FRAMES 60
    switch (state) {
    case SLIDE_IN:
        if (frameCounter++ > SLIDE_IN_FRAMES) {
            state = OPENING_TEXT1;
            renderHealthBars = 1;
            frameCounter = 0;
        }

        opponentPokemonSprite.x -= 96.0f/SLIDE_IN_FRAMES;
        playerPokemonSprite.x += 96.0f/SLIDE_IN_FRAMES;
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
        if (frameCounter++ == 0) PlaySound(gSounds[RUN_SOUND]);
        
        StartTransition(&fadeTransition, callback);
        
        break;
    case PLAYER_MOVE:
        switch (attackState) {
        case ATTACK_START:
            if (frameCounter == 0) {
                damage = Damage(playerPokemon, opponentPokemon);
            }

            if (frameCounter == 60) {
                attackState = ATTACK_ANIM;
                frameCounter = 0;
            }

            frameCounter++;
            break;
        case ATTACK_ANIM:
            if (frameCounter == 0) PlaySound(gSounds[POWERUP_SOUND]);

            if (frameCounter <= 60 && frameCounter % 10 == 0)
                playerPokemonSprite.blink = !playerPokemonSprite.blink;

            if (frameCounter == 60) PlaySound(gSounds[HIT_SOUND]);

            if (frameCounter > 60 && frameCounter % 10 == 0)
                opponentPokemonSprite.opacity = opponentPokemonSprite.opacity == 64 ? 255 : 64;

            if (frameCounter == 120) {
                attackState = ATTACK_DAMAGE;
                frameCounter = 0;
                break;
            }

            frameCounter++;
            break;
        case ATTACK_DAMAGE:
            if (frameCounter++ == damage) {
                attackState = ATTACK_DONE;
                frameCounter = 0;
                break;
            }
            
            if (--opponentPokemon.HP == 0) {
                state = VICTORY; 
                frameCounter = 0; 
            }

            break;
        case ATTACK_DONE:
            if (++moveCounter == 2) {
                state = BATTLE_MENU;
                attackState = ATTACK_START;    
                moveCounter = 0;
                
                break;
            }

            state = OPPONENT_MOVE;
            attackState = ATTACK_START;
            break;
        default:
            break;
        }

        break;
    case OPPONENT_MOVE:
       switch (attackState) {
        case ATTACK_START:
            if (frameCounter == 0) {
                damage = Damage(opponentPokemon, playerPokemon);
            }

            if (frameCounter == 60) {
                attackState = ATTACK_ANIM;
                frameCounter = 0;
            }

            frameCounter++;
            break;
        case ATTACK_ANIM:
            if (frameCounter == 0) PlaySound(gSounds[POWERUP_SOUND]);

            if (frameCounter <= 60 && frameCounter % 10 == 0)
                opponentPokemonSprite.blink = !opponentPokemonSprite.blink;

            if (frameCounter == 60) PlaySound(gSounds[HIT_SOUND]);

            if (frameCounter > 60 && frameCounter % 10 == 0)
                playerPokemonSprite.opacity = playerPokemonSprite.opacity == 64 ? 255 : 64;

            if (frameCounter == 120) {
                attackState = ATTACK_DAMAGE;
                frameCounter = 0;
                break;
            }

            frameCounter++;
            break;
        case ATTACK_DAMAGE:
            if (frameCounter++ == damage) {
                attackState = ATTACK_DONE;
                frameCounter = 0;
                break;
            }
            
            if (--playerPokemon.HP == 0) {
                state = DEFEAT; 
                frameCounter = 0; 
            }

            break;
        case ATTACK_DONE:
            if (++moveCounter == 2) {
                state = BATTLE_MENU;
                attackState = ATTACK_START;    
                moveCounter = 0;
                
                break;
            }

            state = PLAYER_MOVE;
            attackState = ATTACK_START;
            break;
        default:
            break;
        }

        break;
    case VICTORY:
        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_X))
            StartTransition(&fadeTransition, callback);

        if (frameCounter == 0) {
            StopMusic();
            PlayMusic(gMusic[VICTORY_MUSIC]);
        }

        if (frameCounter == 20)
            break;
        
        opponentPokemonSprite.y += (VIRTUAL_HEIGHT - 8) / 20;

        frameCounter++;
        break;
    case DEFEAT:
        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_X))
            StartTransition(&fadeTransition, callback);

        if (frameCounter == 0)
            StopMusic();

        if (frameCounter == 20)
            break;
        
        playerPokemonSprite.y += 128 / 20;

        frameCounter++;
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

    BattleSpriteRender(playerPokemonSprite);
    BattleSpriteRender(opponentPokemonSprite);
    
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
    case PLAYER_MOVE:
        switch (attackState) {
        case ATTACK_START:
            sprintf(str, "%s attacks %s!", gPokemons[playerPokemon.ID].name, gPokemons[opponentPokemon.ID].name);
            DrawText(str, 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
            break;
        case ATTACK_ANIM:

            break;
        case ATTACK_DAMAGE:
            
            break;
        case ATTACK_DONE:
            
            break;
        default:
            break;
        }

        break;
    case OPPONENT_MOVE:
        switch (attackState) {
        case ATTACK_START:
            sprintf(str, "%s attacks %s!", gPokemons[opponentPokemon.ID].name, gPokemons[playerPokemon.ID].name);
            DrawText(str, 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
            break;
        case ATTACK_ANIM:

            break;
        case ATTACK_DAMAGE:
            
            break;
        case ATTACK_DONE:
            
            break;
        default:
            break;
        }

        break;
    case VICTORY:
        DrawText("Victory!", 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
        
        break;
    case DEFEAT:
        DrawText("You fainted!", 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
        
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