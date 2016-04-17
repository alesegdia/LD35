#include "battlescreen.h"


#include "../ld35game.h"
#include "../assets.h"

BattleScreen::BattleScreen(LD35 *g)
	: m_game(g),
	  m_hud(m_game->m_player, m_game->m_font)
{

}

BattleScreen::~BattleScreen()
{

}

void BattleScreen::show()
{
	m_enemyHud.clear();
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 0, 0);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 0, 1);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 1, 0);
	m_enemyHud.set(EntityFactory::makeDummyEnemy(), 2, 1);
	m_battleStatus = PlayerTurn;
}

void BattleScreen::update(double delta)
{
	if( m_battleStatus == EndBattle )
	{
		m_game->setScreen(m_game->m_mapScreen);
	}
	else if( m_battleStatus == PlayerTurn )
	{
		if( m_enemyHud.numEnemies() <= 0 )
		{
			message("WIN!", EndBattle);
		}
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
				m_battleStatus = EnemyTurn;
			}
		}
	}
	else if( m_battleStatus == EnemyTurn )
	{
		std::cout << "enemyturnin" << std::endl;
		float damage = 0;
		m_enemyHud.checkAlive();

		if( m_enemyHud.enemyTurn( m_game->m_player, damage ) )
		{
			message("Wow, that hurts!", PlayerTurn);

			m_battleStatus = PlayerTurn;
			m_game->m_player->turn();
		}
		else
		{
			message("Wow, that hurts!", EnemyTurn);
		}
		m_enemyHud.checkAlive();

	}
	else if( m_battleStatus == InfoTurn )
	{
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
		{
			m_battleStatus = m_storedStatus;
		}
	}
}

void BattleScreen::render()
{
	al_clear_to_color(al_map_rgb(63,63,116));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	m_game->m_camera2.bind();

	m_hud.render();
	m_enemyHud.render();

	if( m_battleStatus == InfoTurn )
	{
		// draw base gui color
		al_draw_filled_rectangle(0, 50, 80, 80, m_game->m_player->shapeColor());
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 50, 0, m_infomsg.c_str());
	}
}

void BattleScreen::hide()
{

}

