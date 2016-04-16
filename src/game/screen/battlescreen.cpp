#include "battlescreen.h"


#include "../ld35game.h"
#include "../assets.h"

BattleScreen::BattleScreen(LD35 *g)
	: m_game(g),
	  m_hud(m_game->m_player, m_game->m_font)
{
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 0, 0);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 0, 1);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 1, 0);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 2, 1);
}

BattleScreen::~BattleScreen()
{

}

void BattleScreen::show()
{

}

void BattleScreen::update(double delta)
{
	m_enemyHud.setHover(false);
	if( m_hudState == ChooseAbility )
	{
		HudReturn opret = m_hud.update();
		if( opret == HudSuccess )
		{

			m_enemyHud.setupSelection(m_hud.getSelected());
			m_hudState = ChooseEnemy;
		}
	}
	else if( m_hudState == ChooseEnemy )
	{
		m_enemyHud.setHover(true);
		HudReturn opret = m_enemyHud.update();

		if( opret == HudBack )
		{
			m_hudState = ChooseAbility;
		}
		else if( opret == HudSuccess )
		{
			m_hudState = ChooseAbility;
			Entity* ent = m_game->m_player.get();
			m_hud.getSelected()->apply(ent, m_enemyHud.getSelecteds(m_hud.getSelected()));
			m_enemyHud.checkAlive();
		}
	}
}

void BattleScreen::render()
{
	al_clear_to_color(al_map_rgb(63,63,116));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	m_game->m_camera1.bind();

	m_hud.render();
	m_enemyHud.render();
}

void BattleScreen::hide()
{

}

