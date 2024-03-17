#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>


class LD35;

class MenuScreen : public IScreen {
public:
	MenuScreen(LD35* g);
	virtual ~MenuScreen();

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override ;
	void hide() override ;

private:
	LD35* m_game;

	float timer = 0.f;
	float param = 1.f;
	int points = 0 ;
	int m_state = 0;
	bool m_escapeState = false;

};


