#include "battle_sprite.h"
#include "../globals.h"

void BattleSpriteRender(BattleSprite sprite) {
    if (sprite.blink) {
        BeginShaderMode(gShaders[BLINK]);
    }
    
        DrawTexture(gTextures[sprite.ID], (int)sprite.x, (int)sprite.y, (Color){255, 255, 255, sprite.opacity});

    if (sprite.blink) {
        EndShaderMode();
    }
}