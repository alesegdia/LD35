
#include "losescreen.h"
#include "../ld35game.h"

#include <allegro5/allegro_font.h>

LoseScreen::LoseScreen(LD35 *g)
	: m_game(g)
{

}

void LoseScreen::show()
{

}

void LoseScreen::update(double delta)
{
	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
	{
		m_game->setScreen(m_game->m_menuScreen);
		m_game->endGame();
	}
}

void LoseScreen::render()
{
	m_game->m_camera2.bind();
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 10, 10, 0, "YOU LOST");
	al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 10, 20, 0, (std::string("Floor ") + std::to_string(m_game->m_floor)).c_str());
	al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 10, 30, 0, (std::string("Level ") + std::to_string(m_game->m_player->m_level)).c_str());
}

void LoseScreen::hide()
{

}
