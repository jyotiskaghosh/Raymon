#ifndef POKEMON_H
#define POKEMON_H

#include "poke_def.h"

typedef struct {
    PokeID ID;
    int HP, attack, defense, speed;
} Pokemon;

void PokeInit(Pokemon *pokemon, int ID);

#endif