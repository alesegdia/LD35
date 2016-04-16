#include "assets.h"
#include <iostream>


Assets* Assets::instance = nullptr;

Assets::Assets()
{
	// ALLOC RESOURCES!!
	pium.reset(new AudioSample("assets/power.ogg"));
	music.reset(new AudioStream("assets/ingame.ogg"));

	tileset = al_load_bitmap("assets/tileset.png");
	tilesetSheet.reset(new Spritesheet(5, 6, tileset));
	tilesetSheet8.reset(new Spritesheet(10, 12, tileset));

	player = al_load_bitmap("assets/playersheet.png");
	std::cout << player << std::endl;
	playerSheet.reset(new Spritesheet(6, 12, player));

	playerStandU.reset(new Animation(0.1f, 1));
	playerStandU->addFrame(playerSheet->getFrame(0, 5));

	playerStandD.reset(new Animation(0.1f, 1));
	playerStandD->addFrame(playerSheet->getFrame(0, 4));

	playerStandL.reset(new Animation(0.1f, 1));
	playerStandL->addFrame(playerSheet->getFrame(0, 6));

	playerStandR.reset(new Animation(0.1f, 1));
	playerStandR->addFrame(playerSheet->getFrame(0, 7));

	playerWalkU.reset(new Animation(0.1f, 2));
	playerWalkU->addFrames(playerSheet->getFrames(6, 7));

	playerWalkD.reset(new Animation(0.1f, 2));
	playerWalkD->addFrames(playerSheet->getFrames(0, 1));

	playerWalkL.reset(new Animation(0.1f, 6));
	playerWalkL->addFrames(playerSheet->getFrames(12, 17));

	playerWalkR.reset(new Animation(0.1f, 6));
	playerWalkR->addFrames(playerSheet->getFrames(18, 23));

	cursorUp = tilesetSheet->getFrame(3, 1);
	cursorDown = tilesetSheet->getFrame(3, 0);
	cursorLeft = tilesetSheet->getFrame(4, 1);
	cursorRight = tilesetSheet->getFrame(4, 0);

	enemyPlaceholder = tilesetSheet->getFrame(1, 1);
}

Assets::~Assets()
{
	// FREE RESOURCES!!
	al_destroy_bitmap(player);
	al_destroy_bitmap(tileset);
}

void Assets::Initialize()
{
	Assets::instance = new Assets();
}

void Assets::Dispose()
{
	delete instance;
}
