#include "ld35game.h"
#include "assets.h"

#include <allegro5/allegro_ttf.h>

#include "gameconfig.h"

LD35Game::LD35Game(int sw, int sh)
	: Game( sw, sh ), m_camera1(Vec2f(sw, sh)), m_camera2(Vec2f(sw, sh))
{

}

LD35Game::~LD35Game()
{

}

void LD35Game::create()
{
	Assets::Initialize();

	m_camera1.scale(GameConfig::CAMERA_SCALE, GameConfig::CAMERA_SCALE);
	m_camera2.scale(GameConfig::CAMERA_SCALE, GameConfig::CAMERA_SCALE);
	m_font = al_load_ttf_font("assets/dafont.ttf", 8, 0);

	if( !m_font )
	{
		fprintf(stderr, "Failed to load font!\n");
	}

	m_menuScreen.reset(new MenuScreen(this));
	m_gameplayScreen.reset(new GameplayScreen(this));

	setScreen(m_menuScreen);
}

void LD35Game::dispose()
{
	Assets::Dispose();
	m_menuScreen.reset();
	m_gameplayScreen.reset();
	al_destroy_font(m_font);
}

void LD35Game::update(double delta)
{
	if( Input::IsKeyDown(ALLEGRO_KEY_ESCAPE) )
	{
		close();
	}

	Game::update(delta);
}

