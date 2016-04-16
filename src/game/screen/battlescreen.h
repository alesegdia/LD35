#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>
#include <alligator/util/matrix.h>

#include "../battle/entity.h"
#include "../battle/hud.h"
#include "../assets.h"


class LD35;

enum HudState : int
{
	ChooseAbility = 0,
	ChooseEnemy,
	EnemyTurn
};


class BattleScreen : public IScreen
{
public:
	BattleScreen(LD35* g);

	virtual ~BattleScreen();

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;

private:
	LD35* m_game;
	PlayerHud m_hud;
	EnemyHud m_enemyHud;
	HudState m_hudState = ChooseAbility;

};
