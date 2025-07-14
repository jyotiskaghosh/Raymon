#include <raylib.h>
#include "../transition.h"
#include "../state_stack.h"
#include "../main.h"

#define FRAMES 60

static int frameCounter;
static void (*transitionCallback)();
static float alpha;

static void FadeInit(void (*callback)())
{
    transitionCallback = callback;
    frameCounter = 0;
    alpha = 0;
}

static void FadeUpdate() 
{
    if (frameCounter++ == FRAMES) {
        StopTransition();
        return;
    }

    if (frameCounter == FRAMES / 2)
        transitionCallback();

    if (frameCounter < FRAMES / 2)
        alpha += 2.0f / FRAMES;       
    else   
        alpha -= 2.0f / FRAMES;
}

static void FadeRender() 
{
    DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, Fade(BLACK, alpha));
}

Transition fadeTransition = {
    .init = FadeInit,
    .update = FadeUpdate,
    .render = FadeRender
};