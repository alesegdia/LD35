#include "assets.h"
#include <iostream>


Assets* Assets::instance = nullptr;

Assets::Assets()
{
	// ALLOC RESOURCES!!
	music.reset(new AudioStream("assets/ingame.ogg"));
	battle.reset(new AudioStream("assets/jefe.ogg"));

	tileset = al_load_bitmap("assets/tileset.png");
	tilesetSheet.reset(new Spritesheet(5, 6, tileset));
	tilesetSheet8.reset(new Spritesheet(10, 12, tileset));

	player = al_load_bitmap("assets/playersheet.png");
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

	ene1 = al_load_bitmap("assets/ene1.png");
	ene2 = al_load_bitmap("assets/ene2.png");
	ene3 = al_load_bitmap("assets/ene3.png");
	ene4 = al_load_bitmap("assets/ene4.png");
	ene5 = al_load_bitmap("assets/ene5.png");

	ene1SH.reset(new Spritesheet(5, 4, ene1));
	ene2SH.reset(new Spritesheet(4, 3, ene2));
	ene3SH.reset(new Spritesheet(6, 5, ene3));
	ene4SH.reset(new Spritesheet(4, 5, ene4));
	ene5SH.reset(new Spritesheet(4, 3, ene5));

	eneAnims[0].reset(new Animation(0.1f, 18));
	eneAnims[0]->addFrames(ene1SH->getFrames(0, 17));

	eneAnims[1].reset(new Animation(0.1f, 11));
	eneAnims[1]->addFrames(ene2SH->getFrames(0, 10));

	eneAnims[2].reset(new Animation(0.1f, 26));
	eneAnims[2]->addFrames(ene3SH->getFrames(0, 25));

	eneAnims[3].reset(new Animation(0.1f, 17));
	eneAnims[3]->addFrames(ene4SH->getFrames(0, 16));

	eneAnims[4].reset(new Animation(0.1f, 12));
	eneAnims[4]->addFrames(ene5SH->getFrames(0, 11));

	eneAnimsData[0].reset();
	eneAnimsData[1].reset();
	eneAnimsData[2].reset();
	eneAnimsData[3].reset();
	eneAnimsData[4].reset();

	eneAnims[0]->updateData(eneAnimsData[0]);
	eneAnims[1]->updateData(eneAnimsData[1]);
	eneAnims[2]->updateData(eneAnimsData[2]);
	eneAnims[3]->updateData(eneAnimsData[3]);
	eneAnims[4]->updateData(eneAnimsData[4]);

	upgradeSfx.reset(new AudioSample("assets/fxupgrade.ogg"));
	clickSfx.reset(new AudioSample("assets/fxmenu.ogg"));
	errorSfx.reset(new AudioSample("assets/fxbad.ogg"));
	levelupSfx.reset(new AudioSample("assets/fxlevelup.ogg"));

}

Assets::~Assets()
{
	// FREE RESOURCES!!
	al_destroy_bitmap(player);
	al_destroy_bitmap(tileset);

	al_destroy_bitmap(ene1);
	al_destroy_bitmap(ene2);
	al_destroy_bitmap(ene3);
	al_destroy_bitmap(ene4);
	al_destroy_bitmap(ene5);
}

void Assets::Initialize()
{
	Assets::instance = new Assets();
}

void Assets::Dispose()
{
	delete instance;
}
