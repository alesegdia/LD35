#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>

#include "../map/mapgen.h"
#include "../map/maprenderer.h"
#include "../map/mapentity.h"


class LD35;

class LoseScreen : public IScreen {
public:
	LoseScreen(LD35* g);

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;

private:
	LD35* m_game;


};


