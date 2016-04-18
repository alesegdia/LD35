#include "midfloorscreen.h"
#include "../assets.h"
#include <allegro5/allegro_font.h>

#include "../ld35game.h"

MidFloorScreen::MidFloorScreen(LD35 *g)
	: m_game(g)
{

}

void MidFloorScreen::show()
{
	m_game->advanceFloor();
}

void MidFloorScreen::update(double delta)
{
	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
	{
		m_game->setScreen(m_game->m_mapScreen);
	}
}

void MidFloorScreen::render()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(m_game->m_fontBig, al_map_rgb(255, 255, 255), 10, 10, 0, (std::string("Floor ") + std::to_string(m_game->m_floor)).c_str());
}

void MidFloorScreen::hide()
{

}
