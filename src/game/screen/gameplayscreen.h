#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>

class LD35Game;

class GameplayScreen : public IScreen {
public:
	GameplayScreen(LD35Game* g);

	virtual ~GameplayScreen();

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;


private:
	LD35Game* m_game;


};


