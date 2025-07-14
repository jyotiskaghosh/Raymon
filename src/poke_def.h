#ifndef POKE_DEF_H
#define POKE_DEF_H

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
    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpeed;
} Pokemon;

void InitPokemon();

#endif