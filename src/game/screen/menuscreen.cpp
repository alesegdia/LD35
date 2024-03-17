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
	m_state = 0;
	Assets::instance->battle->pause();
	if( !Assets::instance->music->playing() )
	{
		Assets::instance->music->rewind();
		Assets::instance->music->play();
	}
}

void MenuScreen::update(double delta)
{
	if (m_escapeState)
	{
		if (Input::IsKeyDown(ALLEGRO_KEY_Y))
		{
			m_game->close();
		}
		else if (Input::IsKeyDown(ALLEGRO_KEY_N))
		{
			m_escapeState = false;
		}
		return;
	}
	if( Input::IsKeyDown(ALLEGRO_KEY_ESCAPE) )
	{
		m_escapeState = true;
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
	{
		m_state++;
	}

	if (m_state ==4)
	{
		m_game->setScreen(m_game->m_mapScreen);
	}

	timer += delta * 10;
}

void MenuScreen::render()
{
	if (m_escapeState)
	{
		m_game->m_camera2.bind();

		al_clear_to_color(al_map_rgb(63, 63, 116));
		al_set_target_bitmap(al_get_backbuffer(m_game->display()));

		al_draw_text(m_game->m_font, al_map_rgb(203, 219, 252), 15, 11, 0, "Quit game?");
		al_draw_text(m_game->m_font, al_map_rgb(203, 219, 252), 20, 20, 0, "(Y/N)");

		return;
	}
	m_game->m_camera2.bind();

	al_clear_to_color(al_map_rgb(63,63,116));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	switch (m_state)
	{
	case 0:
		al_draw_text(m_game->m_fontBig, al_map_rgb(203, 219, 252), 15, 11, 0, "Pheee");
		al_draw_text(m_game->m_fontBig, al_map_rgb(203, 219, 252), 15, 31, 0, "Strew");
		al_draw_text(m_game->m_fontBig, al_map_rgb(203, 219, 252), 15, 51, 0, "Quest");
		break;
	case 1:
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1, 0, "Last attack type");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 1, 0, "changes your");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 2, 0, "shape.");
		al_draw_text(m_game->m_font, al_map_rgb(91, 110, 225), 4, 1 + 8 * 4, 0, "Water: +agi -def");
		al_draw_text(m_game->m_font, al_map_rgb(172, 50, 50), 4, 1 + 8 * 5, 0, "Fire: +atk -agi");
		al_draw_text(m_game->m_font, al_map_rgb(55, 148, 110), 4, 1 + 8 * 6, 0, "Gaia: -atk +def");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 8, 0, "agi: more crit");
		break;
	case 2:
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1, 0, "PHEE STATUS:");
		al_draw_text(m_game->m_font, al_map_rgb(64, 255, 64), 4, 1 + 8 * 1, 0, "regen: +hp/turn");
		al_draw_text(m_game->m_font, al_map_rgb(164, 164, 255), 4, 1 + 8 * 2, 0, "molec: agi+");
		al_draw_text(m_game->m_font, al_map_rgb(164, 255, 164), 4, 1 + 8 * 3, 0, "shell: def+");
		al_draw_text(m_game->m_font, al_map_rgb(255, 164, 164), 4, 1 + 8 * 4, 0, "fist: atk+");

		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 6, 0, "ENEMY STATUS:");
		al_draw_text(m_game->m_font, al_map_rgb(255, 0, 0), 4, 1 + 8 * 7, 0, "burn: -hp/turn");
		al_draw_text(m_game->m_font, al_map_rgb(128, 164, 128), 4, 1 + 8 * 8, 0, "stun: can't attack");

		break;
	case 3:
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1, 0, "SPACE: check skills");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 1, 0, "ENTER: interact");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 2, 0, "Interact with");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 3, 0, "stones in map");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 4, 0, "to get skills");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 5, 0, "and advance!!");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 7, 0, "Take Phee as far");
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 1 + 8 * 8, 0, "as you can!!");


		break;
	}
}

void MenuScreen::hide()
{

}

