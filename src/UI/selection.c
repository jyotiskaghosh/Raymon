#include "selection.h"
#include "panel.h"

#include "../main.h"

extern Texture2D gTextures[];
extern Sound gSounds[];

void SelectionUpdate(Selection *select)
{
    if (IsKeyPressed(KEY_UP) && select->currentSelection != 0) {
        select->currentSelection--;
        PlaySound(gSounds[BLIP]);
    } else if (IsKeyPressed(KEY_DOWN) && select->currentSelection < select->itemsLength) {
        select->currentSelection++;
        PlaySound(gSounds[BLIP]);
    } else if (IsKeyPressed(KEY_X)) {
        select->items[select->currentSelection].onSelect();
        PlaySound(gSounds[BLIP]);
    }
}

void SelectionRender(Selection *select)
{
    PanelRender(select->bounds);
    for (int i = 0; i < select->itemsLength; i++) {
        DrawText(
            select->items[i].text, 
            select->bounds.x + 8, 
            select->bounds.y + 8 + (select->bounds.height / select->itemsLength) * i,
            16,
            WHITE    
        );
    }
    DrawTexture(
        gTextures[CURSOR], 
        select->bounds.x - 8, 
        select->bounds.y + 8 + (select->bounds.height / select->itemsLength) * select->currentSelection,
        WHITE
    );
}