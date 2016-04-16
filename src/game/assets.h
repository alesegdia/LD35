#pragma once

#include <memory>

#include <allegro5/allegro.h>
#include <alligator/graphics/spritesheet.h>
#include <alligator/graphics/animation.h>
#include <alligator/audio/audiosample.h>
#include <alligator/audio/audiostream.h>

class Assets
{
public:

	// singleton interface
	static Assets* instance;
	static void Initialize();
	static void Dispose();

	Assets();
	~Assets();

	AudioSample::SharedPtr pium;
	AudioStream::SharedPtr music;

	ALLEGRO_BITMAP* tileset;
	ALLEGRO_BITMAP* player;

	Spritesheet::SharedPtr tilesetSheet;
	Spritesheet::SharedPtr tilesetSheet8;
	Spritesheet::SharedPtr playerSheet;

	Animation::SharedPtr playerStandU;
	Animation::SharedPtr playerStandD;
	Animation::SharedPtr playerStandL;
	Animation::SharedPtr playerStandR;

	Animation::SharedPtr playerWalkU;
	Animation::SharedPtr playerWalkD;
	Animation::SharedPtr playerWalkL;
	Animation::SharedPtr playerWalkR;

	ALLEGRO_BITMAP* cursorUp;
	ALLEGRO_BITMAP* cursorDown;
	ALLEGRO_BITMAP* cursorLeft;
	ALLEGRO_BITMAP* cursorRight;

	ALLEGRO_BITMAP* enemyPlaceholder;


};
