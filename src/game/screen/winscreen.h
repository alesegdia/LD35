#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>

#include "../map/mapgen.h"
#include "../map/maprenderer.h"
#include "../map/mapentity.h"


class LD35;

class MiniMapScreen : public IScreen {
public:
	MiniMapScreen(LD35* g);

	void config( Matrix2Di::SharedPtr visible, Matrix2Di::SharedPtr lemap, Vec2i center )
	{
		m_visible = visible;
		m_renderer.reset(new FilteredDebugMapRenderer(lemap, visible, 32));
		m_renderer->center(center.x(), center.y());
		m_center = center;
	}

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;

private:
	LD35* m_game;
	Matrix2Di::SharedPtr m_visible;
	FilteredDebugMapRenderer::SharedPtr m_renderer;
	Vec2i m_center;

};


