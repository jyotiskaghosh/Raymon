#ifndef TRANSITION_H
#define TRANSITION_H

#include <raylib.h>
#include "state_stack.h"

typedef struct {
    void (*init)(void (*callback)());
    void (*update)();
    void (*render)();
} Transition;

void StartTransition(Transition *trans, void (*callback)());
void UpdateTransition();
void RenderTransition();
void StopTransition();
char TransitionActive();

#endif