#include "transition.h"

static Transition *transition;
static char active;

void StartTransition(Transition *trans, void (*callback)())
{
    transition = trans;
    active = 1;

    if(transition->init) transition->init(callback);
}

void UpdateTransition()
{
    if (active && transition->update) transition->update();
}

void RenderTransition()
{
    if (active && transition->render) transition->render();
}

void StopTransition() 
{
    active = 0;
}

char TransitionActive() 
{ 
    return active;
}