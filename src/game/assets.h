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

	AudioSample::SharedPtr upgradeSfx;
	AudioSample::SharedPtr clickSfx;
	AudioSample::SharedPtr errorSfx;
	AudioSample::SharedPtr levelupSfx;

	AudioStream::SharedPtr music;
	AudioStream::SharedPtr battle;

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

	ALLEGRO_BITMAP* ene1;
	ALLEGRO_BITMAP* ene2;
	ALLEGRO_BITMAP* ene3;
	ALLEGRO_BITMAP* ene4;
	ALLEGRO_BITMAP* ene5;

	Spritesheet::SharedPtr ene1SH;
	Spritesheet::SharedPtr ene2SH;
	Spritesheet::SharedPtr ene3SH;
	Spritesheet::SharedPtr ene4SH;
	Spritesheet::SharedPtr ene5SH;

	Animation::SharedPtr eneAnims[5];

	AnimationData eneAnimsData[5];


	void Update(double delta)
	{
		for( int i = 0; i < 5; i++ )
		{
			eneAnimsData[i].timer += delta;
			eneAnims[i]->updateData(eneAnimsData[i]);
		}
	}



};
