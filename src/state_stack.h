#ifndef STATE_STACK_H
#define STATE_STACK_H

#define MAX_STATES 16

typedef struct {
    void (*init)(void *data);
    void (*update)();
    void (*render)();
    void (*enter)();
    void (*exit)();
} GameState;

void PushState(GameState *state, void *data);
void PopState();
void UpdateCurrentState();
void RenderCurrentState();

#endif