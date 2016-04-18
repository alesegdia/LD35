#include "winscreen.h"

#include "../ld35game.h"

MiniMapScreen::MiniMapScreen(LD35 *g)
	: m_game(g)
{

}

void MiniMapScreen::show()
{

}

void MiniMapScreen::update(double delta)
{
	m_renderer->center(m_center.x(), m_center.y());
	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) ||
		Input::IsKeyJustPressed(ALLEGRO_KEY_BACKSPACE) ||
			Input::IsKeyJustPressed(ALLEGRO_KEY_SPACE) ||
			Input::IsKeyJustPressed(ALLEGRO_KEY_TAB) ||
		Input::IsKeyJustPressed(ALLEGRO_KEY_ESCAPE) )
	{
		m_game->setScreen(m_game->m_mapScreen);
	}
}

void MiniMapScreen::render()
{

	float bx, by, rx, ry;

	//bx = 10 * 16 - m_renderer->width() / 8 * 16;
	//by = 10 * 16 - m_renderer->height() / 8 * 16;

	//bx = 10 * 16 - m_center.x() * 16;
	//bx = 10 * 16 - m_center.y() * 16;

	bx = - m_center.x() * 16;
	by = - m_center.y() * 16;

	rx = bx + 9 * 16;
	ry = by + 9 * 16;


	//rx = bx - m_center.x() * 16;
	//ry = by - m_center.y() * 16;

	m_game->m_camera3.position(rx, ry);
	m_game->m_camera3.bind();

	al_clear_to_color(al_map_rgb(69, 40, 60));
	//al_clear_to_color(al_map_rgb(255, 0, 255));
	m_renderer->render();
}

void MiniMapScreen::hide()
{

}
