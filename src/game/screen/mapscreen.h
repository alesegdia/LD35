#pragma once

#include <allegro5/allegro5.h>

#include <alligator/game/iscreen.h>
#include <alligator/input/input.h>
#include <alligator/camera/camera.h>

#include "../map/mapgen.h"
#include "../map/maprenderer.h"
#include "../map/mapentity.h"


class LD35;

class MapScreen : public IScreen {
public:
	MapScreen(LD35* g);

	virtual ~MapScreen();

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;

	void scroll(Vec2f new_cam_pos, Camera& cam);

	void map( const Matrix2Di& map )
	{
		if( m_map == nullptr )
		{
			m_map.reset(new Matrix2Di(map));
		}
		else
		{
			*m_map = map;
		}
	}

	Matrix2Di::SharedPtr map()
	{
		return m_map;
	}

	Vec2i findFirstWalkable()
	{
		for( int x = 0; x < m_map->cols(); x++ )
		{
			for( int y = 0; y < m_map->rows(); y++ )
			{
				if( m_map->get(x, y) == 0 )
				{
					return Vec2i(x, y);
				}
			}
		}
		return Vec2i(-1, -1);
	}


private:
	LD35* m_game;
	MapRenderer::SharedPtr m_mapRenderer;
	PlayerPawnMap::SharedPtr m_player;
	Matrix2Di::SharedPtr m_map;


};


