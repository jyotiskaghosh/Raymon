#ifndef POKE_DEF_H
#define POKE_DEF_H

#include <raylib.h>
#include "main.h"

typedef enum {
    AARDART,
    AGNITE,
    ANOLEAF,
    BAMBOON,
    CARDIWING,
    MAX_POKEMONS
} PokeID;

typedef struct {
    char name[16];
    TextureID battleSpriteFront;
    TextureID battleSpriteBack;
    int baseHP, baseAttack, baseDefense, baseSpeed;
} PokemonDef;

void InitPokemonDef();

#endif