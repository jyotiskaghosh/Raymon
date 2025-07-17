#include <raylib.h>
#include "main.h"
#include "state_stack.h"
#include "transition.h"
#include "poke_def.h"
#include "pokemon.h"

Music gMusic[MAX_MUSIC];
Sound gSounds[MAX_SOUNDS];
Texture2D gTextures[MAX_TEXTURES];
PokemonDef gPokemons[MAX_POKEMONS];

Pokemon playerPokemon;
Music currentMusic;

extern GameState startState;

static void InitGame()
{
	InitPokemonDef();

	PokeInit(&playerPokemon, GetRandomValue(0, MAX_POKEMONS - 1));	

	// Music
	gMusic[INTRO_MUSIC] = LoadMusicStream("resources/audio/intro.mp3");
	gMusic[FIELD_MUSIC] = LoadMusicStream("resources/audio/field_music.wav");
	gMusic[BATTLE_MUSIC] = LoadMusicStream("resources/audio/battle_music.mp3");

	gSounds[BLIP] = LoadSound("resources/audio/blip.wav");

	// Textures
	gTextures[TILES] = LoadTexture("resources/graphics/sheet.png");
	gTextures[PLAYER] = LoadTexture("resources/graphics/player.png");
	gTextures[CURSOR] = LoadTexture("resources/graphics/cursor.png");

	gTextures[AARDART_FRONT] = LoadTexture("resources/graphics/pokemon/aardart-front.png");
	gTextures[AARDART_BACK] = LoadTexture("resources/graphics/pokemon/aardart-back.png");

	gTextures[AGNITE_FRONT] = LoadTexture("resources/graphics/pokemon/agnite-front.png");
	gTextures[AGNITE_BACK] = LoadTexture("resources/graphics/pokemon/agnite-back.png");

	gTextures[ANOLEAF_FRONT] = LoadTexture("resources/graphics/pokemon/anoleaf-front.png");
	gTextures[ANOLEAF_BACK] = LoadTexture("resources/graphics/pokemon/anoleaf-back.png");

	gTextures[BAMBOON_FRONT] = LoadTexture("resources/graphics/pokemon/bamboon-front.png");
	gTextures[BAMBOON_BACK] = LoadTexture("resources/graphics/pokemon/bamboon-back.png");

	gTextures[CARDIWING_FRONT] = LoadTexture("resources/graphics/pokemon/cardiwing-front.png");
	gTextures[CARDIWING_BACK] = LoadTexture("resources/graphics/pokemon/cardiwing-back.png");
}

static void UnloadGame()
{
	for (int i = 0; i < MAX_MUSIC; i++)
		UnloadMusicStream(gMusic[i]);

	for (int i = 0; i < MAX_SOUNDS; i++)
		UnloadSound(gSounds[i]);

	for (int i = 0; i < MAX_TEXTURES; i++)
		UnloadTexture(gTextures[i]);
}

int main()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "pokemon clone");

	InitAudioDevice();

	InitGame();

	RenderTexture2D virtualScreen = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

	SetTargetFPS(60);

 	PushState(&startState, 0);

	while (!WindowShouldClose()) {
		UpdateMusicStream(currentMusic);
		
		if (!TransitionActive())
			UpdateCurrentState();
		UpdateTransition();

		BeginTextureMode(virtualScreen);
			RenderCurrentState();
			RenderTransition();
		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);

			float scaleX = (float) WINDOW_WIDTH / VIRTUAL_WIDTH;
			float scaleY = (float) WINDOW_HEIGHT / VIRTUAL_HEIGHT;
			DrawTexturePro(
				virtualScreen.texture,
				(Rectangle){0, 0, VIRTUAL_WIDTH, -VIRTUAL_HEIGHT},
				(Rectangle){0, 0, VIRTUAL_WIDTH * scaleX, VIRTUAL_HEIGHT * scaleY},
				(Vector2){0, 0},
				0,
				WHITE	
			);
		EndDrawing();
	}

	UnloadGame();

	CloseAudioDevice();

	CloseWindow();
}
