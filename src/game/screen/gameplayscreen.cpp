#include "gameplayscreen.h"
#include "../ld35game.h"
#include "../assets.h"

GameplayScreen::GameplayScreen(LD35Game *g)
	: m_game(g)
{

}

GameplayScreen::~GameplayScreen()
{

}

void GameplayScreen::show()
{

}

void GameplayScreen::update(double delta)
{

}

void GameplayScreen::render()
{
	al_clear_to_color(al_map_rgb(210,210,210));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));
}

void GameplayScreen::hide()
{

}
