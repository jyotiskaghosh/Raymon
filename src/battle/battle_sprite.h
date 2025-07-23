#ifndef BATTLE_SPRITE_H
#define BATTLE_SPRITE_H

#include <raylib.h>
#include "../main.h"

typedef struct {
    TextureID ID;
    float x, y;
    
    // effects
    char blink;
    char opacity;
} BattleSprite;

void BattleSpriteRender(BattleSprite sprite);

#endif
