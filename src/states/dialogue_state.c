#include <raylib.h>
#include <string.h>
#include "../state_stack.h"
#include "../UI/panel.h"
#include "../main.h"
#include "dialogue_state.h"

#define PANEL (Rectangle){0, VIRTUAL_HEIGHT - 64, VIRTUAL_WIDTH, 64}

static char lines[MAX_DIALOGUE_LINES][MAX_DIALOGUE_LENGTH];
static int count;
static int current;

static void DialogueStateInit(void *data) { }

static void Next()
{
    if (current >= count)
        PopState();
    else
        current++;
}

static void DialogueStateUpdate()
{
    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_X))
        Next();
}

static void DialogueStateRender()
{
    PanelRender(PANEL);
    DrawText(lines[current], 8, VIRTUAL_HEIGHT - 64 + 8, 16, WHITE);
}

GameState dialogueState = {
    .init = DialogueStateInit,
    .update = DialogueStateUpdate,
    .render = DialogueStateRender,
};


void StartDialogueQueue(const char *l[], int lineCount) 
{
    count = lineCount;

    for (int i = 0; i < lineCount && i < MAX_DIALOGUE_LINES; i++)
        strncpy(lines[i], l[i], MAX_DIALOGUE_LENGTH);

    PushState(&dialogueState, 0);    
}
