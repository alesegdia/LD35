#pragma once

#include <list>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>
#include <alligator/util/matrix.h>

#include "../battle/entity.h"
#include "../assets.h"


enum HudReturn : int
{
	HudSuccess = 0,
	HudFailure,
	HudNoOp,
	HudBack
};


class GenericHud
{
public:
	virtual ~GenericHud();

	virtual void render() = 0 ;
	virtual HudReturn update() = 0 ;
};

class EnemyHud;

class PlayerHud : public GenericHud
{
public:
	PlayerHud( Player::SharedPtr player, ALLEGRO_FONT* font );

	void render() override ;

	HudReturn update() override ;

	bool tryCursorUp();

	int numAbilities();

	bool tryCursorDown();

	Ability::SharedPtr getSelected();

	void setPlayer( Player::SharedPtr player )
	{
		m_player = player;
	}

	void SetEnemyHud(EnemyHud* enemyHud)
	{
		m_enemyHud = enemyHud;
	}


private:
	Player::SharedPtr m_player;
	ALLEGRO_FONT* m_font;
	EnemyHud* m_enemyHud;

	int m_top = 0;
	int m_bot = 3;
	int m_selected = 0;

};

typedef Matrix2D<Enemy::SharedPtr> EnemyLayout;

class EnemyHud
{
public:

	static constexpr int LayoutWidth = 3;
	static constexpr int LayoutHeight = 2;

	EnemyHud(Entity* player);

	void render();
	HudReturn update();

	void clear();

	void checkAlive();

	bool hasEnemyAt(int x, int y)
	{
		return m_enemyLayout.get(x, y) != nullptr;
	}

	void set( Enemy::SharedPtr enemy, int x, int y );

	std::vector<Entity*> getSelecteds( Ability::SharedPtr ability );

	void setupSelection( Ability::SharedPtr ability );

	void setHover(bool hover);

	bool enemyTurn( Player::SharedPtr player, float& damage, std::stack<std::string>& notifstack, int& attackresult )
	{
		attackresult = 0;
		for( m_nextEnemyTurn; m_nextEnemyTurn < LayoutWidth * LayoutHeight; m_nextEnemyTurn++ )
		{
			Enemy::SharedPtr e = m_enemyLayout.data()[m_nextEnemyTurn];
			if( e != nullptr )
			{
				e->stepStatusEffects(notifstack);

				bool dobreak = false;
				if( !e->isStunned() )
				{
					std::vector<Entity*> playerlist = { player.get() };
					attackresult = e->abilities()[0]->apply((Entity*)e.get(), playerlist);
					m_nextEnemyTurn++;
					dobreak = true;
				}


				if( dobreak )
				{
					break;
				}
			}
		}

		if( m_nextEnemyTurn >= LayoutWidth * LayoutHeight )
		{
			m_nextEnemyTurn = 0;
			return true;
		}
		return false;
	}

	void add( Enemy::SharedPtr e )
	{

		if( m_enemyLayout.get(1, 0) == nullptr )	  set(e, 1, 0);
		else if( m_enemyLayout.get(1, 1) == nullptr ) set(e, 1, 1);
		else if( m_enemyLayout.get(0, 0) == nullptr ) set(e, 0, 0);
		else if( m_enemyLayout.get(2, 0) == nullptr ) set(e, 2, 0);
		else if( m_enemyLayout.get(0, 1) == nullptr ) set(e, 0, 1);
		else if( m_enemyLayout.get(2, 1) == nullptr ) set(e, 2, 1);

		/*
		bool dobreak = false;

		for( int x = 0; x < m_enemyLayout.cols(); x++ )
		{
			for( int y = 0; y < m_enemyLayout.rows(); y++ )
			{
				if( m_enemyLayout.get(x, y) == nullptr )
				{
					set(e, x, y);
					dobreak = true;
					break;
				}
			}
			if( dobreak )
			{
				break;
			}
		}
		*/
	}

	int numEnemies()
	{
		int i = 0;
		for( int x = 0; x < m_enemyLayout.cols(); x++ )
		{
			for( int y = 0; y < m_enemyLayout.rows(); y++ )
			{
				if( m_enemyLayout.get(x, y) != nullptr )
				{
					i++;
				}
			}
		}
		return i;
	}

private:

	void drawHealthFor( Enemy::SharedPtr enemy, int x, int y );

	EnemyLayout m_enemyLayout;
	Ability::SharedPtr m_ability;
	bool m_ignore = false;

	int m_selectedX = 0;
	int m_selectedY = 0;

	int m_nextEnemyTurn = 0;

	bool m_hover = false;

	Entity* m_player = nullptr;

};

