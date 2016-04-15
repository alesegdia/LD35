#pragma once

#include <memory>
#include <allegro5/allegro.h>
#include <alligator/graphics/spritesheet.h>
#include <alligator/graphics/animation.h>

class Assets
{
public:

	// singleton interface
	static Assets* instance;
	static void Initialize();
	static void Dispose();

	Assets();
	~Assets();

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

};
