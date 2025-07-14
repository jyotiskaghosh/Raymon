#include "state_stack.h"

GameState *stateStack[MAX_STATES];
int stackTop = -1;

void PushState(GameState *state, void *data) 
{
    if (stackTop < MAX_STATES - 1) {
        stackTop++;
        stateStack[stackTop] = state;
        if (state->init) state->init(data);
        if (state->enter) state->enter();
    }
}

void PopState() 
{
    if (stackTop >= 0) {
        if (stateStack[stackTop]->exit)
            stateStack[stackTop]->exit();
        stackTop--;

        if (stackTop >= 0) {
            if (stateStack[stackTop]->enter)
                stateStack[stackTop]->enter();
        }
    }
}

void UpdateCurrentState() 
{
    if (stackTop >= 0 && stateStack[stackTop]->update)
        stateStack[stackTop]->update();
}

void RenderCurrentState() 
{
    if (stackTop >= 0 && stateStack[stackTop]->render)
        stateStack[stackTop]->render();
}
