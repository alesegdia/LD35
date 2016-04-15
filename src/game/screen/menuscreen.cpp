#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "menuscreen.h"
#include "../ld35game.h"
#include "../assets.h"


MenuScreen::MenuScreen( LD35Game* g )
{
	m_game = g;
}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::show()
{

}

void MenuScreen::update(double delta)
{
	if( Input::IsKeyDown(ALLEGRO_KEY_ESCAPE) )
	{
		m_game->close();
	}
}

void MenuScreen::render()
{

	m_game->m_camera2.bind();

	al_clear_to_color(al_map_rgb(210,210,210));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

}

void MenuScreen::hide()
{

}

