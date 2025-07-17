#ifndef SELECTION_H
#define SELECTION_H

#include <raylib.h>

#define SELECTION_TEXT_LENGTH 16
#define SELECTION_ITEMS_LENGTH 16

typedef struct {
    char text[SELECTION_TEXT_LENGTH];
    void (*onSelect)();
} SelectionItem;

typedef struct {
    SelectionItem items[SELECTION_ITEMS_LENGTH];
    Rectangle bounds;
    int itemsLength, currentSelection;
} Selection;

void SelectionUpdate(Selection *select);
void SelectionRender(Selection *select);

#endif