#include <raylib.h>
#include "../state_stack.h"
#include "../transition.h"
#include "../music.h"
#include "../main.h"
#include "../poke_def.h"

extern GameState overworldState;
extern Transition fadeTransition;
extern Music gMusic[];
extern Texture2D gTextures[];
extern Pokemon gPokemons[];

static int frameCounter;
static int animationFrameCounter;

static char animationActive;

static TextureID spriteID;
static int spriteX;

static void StartStateInit(void *data) {}

static void StartStateEnter()
{
    PlayMusic(gMusic[INTRO_MUSIC]);

    spriteID = gPokemons[GetRandomValue(0, MAX_POKEMONS - 1)].battleSpriteFront;
    spriteX =  VIRTUAL_WIDTH / 2 - 32;
    
    frameCounter = 0;
    animationFrameCounter = 0;
    animationActive = 0;
}

static void Callback() { PushState(&overworldState, 0); }

static void StartStateUpdate() 
{
    if (IsKeyPressed(KEY_ENTER)) {
        StopMusic();

        StartTransition(&fadeTransition, Callback);        
        return;
    }

    if (!animationActive) 
        frameCounter++;
    else
        animationFrameCounter++;

    if (frameCounter == 180) {
        frameCounter = 0;
        animationActive = 1;
    }

    if (animationFrameCounter == 20) {
        spriteID = gPokemons[GetRandomValue(0, MAX_POKEMONS - 1)].battleSpriteFront;
        spriteX = VIRTUAL_WIDTH;
    }

    if (animationFrameCounter == 40) {
        animationFrameCounter = 0;
        animationActive = 0;
    }

    if (animationActive)
        if (animationFrameCounter < 20)
            spriteX -= (VIRTUAL_WIDTH / 2 + 32) / 20;
        else 
            spriteX -= (VIRTUAL_WIDTH / 2 + 32) / 20;
}

static void StartStateRender() 
{
    ClearBackground((Color){188, 188, 188, 255});

    DrawText("Raymon!", (VIRTUAL_WIDTH - MeasureText("Raymon!", 32)) / 2, VIRTUAL_HEIGHT / 2 - 72, 32, (Color){24, 24, 24, 255});
    DrawText("Press Enter", (VIRTUAL_WIDTH - MeasureText("Press Enter", 24)) / 2, VIRTUAL_HEIGHT / 2 + 68, 24, (Color){24, 24, 24, 255});

    DrawEllipse(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + 32, 72, 24, (Color){45, 184, 45, 124});
    DrawTexture(gTextures[spriteID], spriteX, VIRTUAL_HEIGHT / 2 - 16, WHITE);
}  

GameState startState = {
        .init = StartStateInit,
        .enter = StartStateEnter,
        .update = StartStateUpdate,
        .render = StartStateRender
    };