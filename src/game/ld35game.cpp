#include "ld35game.h"
#include "assets.h"

#include <allegro5/allegro_ttf.h>

#include "gameconfig.h"

LD35::LD35(int sw, int sh)
	: Game( sw, sh ), m_camera1(Vec2f(sw, sh)), m_camera2(Vec2f(sw, sh)), m_camera3(Vec2f(sw, sh))
{

}

LD35::~LD35()
{

}

void LD35::create()
{
	Assets::Initialize();

	m_camera1.scale(GameConfig::CAMERA_SCALE, GameConfig::CAMERA_SCALE);
	m_camera2.scale(GameConfig::CAMERA_SCALE, GameConfig::CAMERA_SCALE);
	m_camera3.scale(GameConfig::CAMERA_SCALE, GameConfig::CAMERA_SCALE);
	m_font = al_load_ttf_font("assets/dafont.ttf", 8, 0);
	m_fontBig = al_load_ttf_font("assets/dafont.ttf", 16, 0);

	m_player.reset(new Player());

	if( !m_font )
	{
		fprintf(stderr, "Failed to load font!\n");
	}

	m_menuScreen.reset(new MenuScreen(this));
	m_mapScreen.reset(new MapScreen(this));
	m_battleScreen.reset(new BattleScreen(this));
	m_loseScreen.reset(new LoseScreen(this));
	m_midFloorScreen.reset(new MidFloorScreen(this));
	m_minimapScreen.reset(new MiniMapScreen(this));

	Assets::instance->battle->pause();

	setScreen(m_menuScreen);
}

void LD35::dispose()
{
	Assets::Dispose();
	m_menuScreen.reset();
	m_mapScreen.reset();
	al_destroy_font(m_font);
	al_destroy_font(m_fontBig);
}

void LD35::update(double delta)
{

	if( m_player != nullptr )
	{
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_F12) )
		{
			m_player->unlockAll();
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_F1) )
	{
		setScreen(m_menuScreen);
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_F2) )
	{
		setScreen(m_mapScreen);
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_F3) )
	{
		setScreen(m_battleScreen);
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ESCAPE) )
	{
		close();
	}

	Game::update(delta);
}

