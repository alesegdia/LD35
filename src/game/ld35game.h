
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <memory>

#include <alligator/game/game.h>
#include <alligator/game/iscreen.h>
#include <alligator/camera/camera.h>

#include "screen/mapscreen.h"
#include "screen/menuscreen.h"
#include "screen/battlescreen.h"
#include "screen/losescreen.h"
#include "screen/midfloorscreen.h"

#include "battle/entity.h"

class LD35 : public Game {

public:

	LD35 ( int sw, int sh );
	virtual ~LD35();

	void create() override ;
	void dispose() override ;
	void update(double delta) override ;

	void endGame()
	{
		resetPlayer();
		m_floor = 1;
		setScreen(m_menuScreen);
		m_battleScreen->setPlayer(m_player);
		m_mapScreen->nextLevel(m_floor);
	}

	void advanceFloor()
	{
		m_floor++;
		m_mapScreen->nextLevel(m_floor);
	}

	void resetPlayer()
	{
		m_player.reset(new Player);
	}

	// all screens
	std::shared_ptr<MenuScreen> m_menuScreen;
	std::shared_ptr<MapScreen> m_mapScreen;
	std::shared_ptr<BattleScreen> m_battleScreen;
	std::shared_ptr<LoseScreen> m_loseScreen;
	std::shared_ptr<MidFloorScreen> m_midFloorScreen;

	Player::SharedPtr m_player;

	ALLEGRO_FONT* m_font = nullptr;
	ALLEGRO_FONT* m_fontBig = nullptr;
	Camera m_camera1;
	Camera m_camera2;

	int m_floor = 1;

};

