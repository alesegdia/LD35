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


private:
	Player::SharedPtr m_player;
	ALLEGRO_FONT* m_font;

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

	EnemyHud();

	void render();
	HudReturn update();

	void clear();

	void checkAlive();

	void set( Enemy::SharedPtr enemy, int x, int y );

	std::vector<Entity*> getSelecteds( Ability::SharedPtr ability );

	void setupSelection( Ability::SharedPtr ability );

	void setHover(bool hover);

private:

	void drawHealthFor( Enemy::SharedPtr enemy, int x, int y );

	EnemyLayout m_enemyLayout;
	Ability::SharedPtr m_ability;
	bool m_ignore = false;

	int m_selectedX = 0;
	int m_selectedY = 0;

	bool m_hover = false;

};

