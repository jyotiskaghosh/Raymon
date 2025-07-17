#include "pokemon.h"
#include "poke_def.h"

extern PokemonDef gPokemons[];

void PokeInit(Pokemon *mon, int ID)
{
    mon->ID = ID;

    PokemonDef def = gPokemons[ID];

    mon->HP = def.baseHP;
    mon->attack = def.baseAttack;
    mon->defense = def.baseDefense;
    mon->speed = def.baseSpeed;
}