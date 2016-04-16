#include "assets.h"
#include <iostream>


Assets* Assets::instance = nullptr;

Assets::Assets()
{
	// ALLOC RESOURCES!!
	pium.reset(new AudioSample("assets/power.ogg"));
	music.reset(new AudioStream("assets/ingame.ogg"));
}

Assets::~Assets()
{
	// FREE RESOURCES!!
}

void Assets::Initialize()
{
	Assets::instance = new Assets();
}

void Assets::Dispose()
{
	delete instance;
}
