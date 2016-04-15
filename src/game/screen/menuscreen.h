#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>


class LD35Game;

class MenuScreen : public IScreen {
public:
	MenuScreen(LD35Game* g);
	virtual ~MenuScreen();

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override ;
	void hide() override ;

private:
	LD35Game* m_game;

	float timer = 0.f;
	float param = 1.f;
	int points = 0 ;

};


