#include "poke_def.h"
#include "main.h"

extern Pokemon gPokemons[];

void InitPokemon()
{
    gPokemons[AARDART] = (Pokemon){
        .name = "Aardart",
        .battleSpriteFront = AARDART_FRONT,
        .battleSpriteBack = AARDART_BACK,
        .baseHP = 14,
        .baseAttack = 9,
        .baseDefense = 5,
        .baseSpeed = 6
    };

    gPokemons[AGNITE] = (Pokemon){
        .name = "Agnite",
        .battleSpriteFront = AGNITE_FRONT,
        .battleSpriteBack = AGNITE_BACK,
        .baseHP = 12,
        .baseAttack = 7,
        .baseDefense = 3,
        .baseSpeed = 7
    };

    gPokemons[ANOLEAF] = (Pokemon){
        .name = "Anoleaf",
        .battleSpriteFront = ANOLEAF_FRONT,
        .battleSpriteBack = ANOLEAF_BACK,
        .baseHP = 11,
        .baseAttack = 5,
        .baseDefense = 5,
        .baseSpeed = 6
    };

    gPokemons[BAMBOON] = (Pokemon){
        .name = "Bamboom",
        .battleSpriteFront = BAMBOON_FRONT,
        .battleSpriteBack = BAMBOON_BACK,
        .baseHP = 13,
        .baseAttack = 6,
        .baseDefense = 4,
        .baseSpeed = 7
    };

    gPokemons[CARDIWING] = (Pokemon){
        .name = "Cardiwing",
        .battleSpriteFront = CARDIWING_FRONT,
        .battleSpriteBack = CARDIWING_BACK,
        .baseHP = 14,
        .baseAttack = 7,
        .baseDefense = 3,
        .baseSpeed = 7
    };
}