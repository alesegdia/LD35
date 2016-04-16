#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <alligator/util/rng.h>

#include <iostream>

#include "menuscreen.h"
#include "../ld35game.h"
#include "../assets.h"


MenuScreen::MenuScreen( LD35* g )
{
	m_game = g;
}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::show()
{
	std::cout << "playin!" << std::endl;
	Assets::instance->music->play();
}

void MenuScreen::update(double delta)
{
	if( Input::IsKeyDown(ALLEGRO_KEY_ESCAPE) )
	{
		m_game->close();
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_SPACE) )
	{
		Assets::instance->pium->play();
		param += delta;
		points++;
	}

	timer += delta * 10;

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_P) )
	{
		if( Assets::instance->music->playing() )
		{
			m_game->setScreen(m_game->m_mapScreen);
			Assets::instance->music->pause();
		}
		else
		{
			Assets::instance->music->resume();
		}
	}


}

void MenuScreen::render()
{

	m_game->m_camera2.bind();

	al_clear_to_color(al_map_rgb(210,210,210));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	float sc = 5 + param * 4 * sin(timer * param);

	m_game->m_camera2.scale(sc, sc);

	al_draw_text(m_game->m_font, al_map_rgb(RNG::rng->nextInt(256), RNG::rng->nextInt(256), RNG::rng->nextInt(256)), 3, 2, 0, "LD35");

	std::string pointstr;
	pointstr += points;

	m_game->m_camera1.bind();
	al_draw_text(m_game->m_font, al_map_rgb(RNG::rng->nextInt(256), RNG::rng->nextInt(256), RNG::rng->nextInt(256)), 3, 54, 0, std::to_string(points).c_str());

}

void MenuScreen::hide()
{

}

