
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <memory>

#include <alligator/game/game.h>
#include <alligator/game/iscreen.h>
#include <alligator/camera/camera.h>

#include "screen/gameplayscreen.h"
#include "screen/menuscreen.h"

class LD35Game : public Game {

public:

	LD35Game ( int sw, int sh );
	virtual ~LD35Game();

	void create() override ;
	void dispose() override ;
	void update(double delta) override ;

	// all screens
	std::shared_ptr<MenuScreen> m_gameplayScreen;
	std::shared_ptr<GameplayScreen> m_mapScreen;

	ALLEGRO_FONT* m_font = nullptr;
	Camera m_camera1;
	Camera m_camera2;

};

