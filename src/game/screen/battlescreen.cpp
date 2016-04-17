#include "battlescreen.h"


#include "../ld35game.h"
#include "../assets.h"

BattleScreen::BattleScreen(LD35 *g)
	: m_game(g),
	  m_hud(m_game->m_player, m_game->m_font),
	  m_enemyHud(m_game->m_player.get())
{

}

BattleScreen::~BattleScreen()
{

}

void BattleScreen::show()
{
	Assets::instance->music->pause();
	Assets::instance->battle->rewind();
	Assets::instance->battle->play();


	m_enemyHud.clear();

	//makeDummyParty();
	makePartyForFloor(m_game->m_floor);


	/*
	e = EntityFactory::makeDummyEnemy();
	partyExperience += e->computeExp();
	m_enemyHud.set(e, 0, 1);

	e = EntityFactory::makeDummyEnemy();
	partyExperience += e->computeExp();
	m_enemyHud.set(e, 2, 1);

	e = EntityFactory::makeDummyEnemy();
	partyExperience += e->computeExp();
	m_enemyHud.set(e, 1, 1);
	*/

	m_battleStatus = PlayerTurn;
}

void BattleScreen::update(double delta)
{
	Assets::instance->Update(delta);
	if( m_battleStatus == InfoTurn )
	{
		if( m_infoForNotif )
		{
			notifs.pop();
			m_infoForNotif = false;
		}
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
		{
			m_battleStatus = m_storedStatus;
		}
	}
	else if( !notifs.empty() )
	{
		m_storedStatus = m_battleStatus;
		message(m_storedStatus, notifs.top());
		m_infoForNotif = true;
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
		{
			std::cout << "MEH!";
			notifs.pop();
		}
	}
	else if( m_battleStatus == EndBattle )
	{
		m_game->setScreen(m_game->m_mapScreen);
	}
	else if( m_battleStatus == PlayerTurn )
	{
		if( m_enemyHud.numEnemies() <= 0 )
		{
			int exp = consumeExperience();
			m_game->m_player->addExp(exp);
			std::string str = "WIN! +";
			str += std::to_string(exp);
			str += " exp";
			message(EndBattle, str);
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
				m_game->m_player->stepStatusEffects(notifs);

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

		if( m_enemyHud.enemyTurn( m_game->m_player, damage, notifs ) )
		{
			message(PlayerTurn, "Wow, that hurts!");
			m_battleStatus = PlayerTurn;
			m_game->m_player->turn();
		}
		else
		{
			message(EnemyTurn, "Wow, that hurts!");
		}
		m_enemyHud.checkAlive();

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
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, 50, 0, m_infomsg1.c_str());
	}
}

void BattleScreen::hide()
{

}

