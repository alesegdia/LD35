#include "maprenderer.h"

MapRenderer::MapRenderer(Matrix2Di::SharedPtr map, int ext)
	: m_map(map), m_visible(new Matrix2Di(map->rows(), map->cols(), 0)), ext(ext)
{

}

void MapRenderer::render()
{
	for( int x = m_center.x() - ext; x < m_center.x() + ext + 1; x++ )
	{
		for( int y = m_center.y() - ext; y < m_center.y() + ext + 1; y++ )
		{
			if( x >= 0 && x < m_map->cols() &&
				y >= 0 && y < m_map->rows())
			{
				if( std::abs(x - m_center.x()) < 3 &&
					std::abs(y - m_center.y()) < 3 )
				{
					m_visible->set(x, y, 1);
				}
				renderCell( x, y, m_map->get(x, y) );
			}
		}
	}
}

DebugMapRenderer::DebugMapRenderer(Matrix2Di::SharedPtr map, int ext) : MapRenderer(map, ext) {}
FilteredDebugMapRenderer::FilteredDebugMapRenderer(Matrix2Di::SharedPtr map, Matrix2Di::SharedPtr visited, int ext) : MapRenderer(map, ext), m_levisible(visited) {}

void DebugMapRenderer::renderCell(int x, int y, int cell_value)
{
	float x1, y1, x2, y2;
	x1 =	 x * m_scale;
	y1 =	 y * m_scale;
	x2 = (x+1) * m_scale;
	y2 = (y+1) * m_scale;
	if( cell_value == 0 )
	{
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 0, 0));
	}
	else
	{
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 255, 0));
	}
}

void FilteredDebugMapRenderer::renderCell(int x, int y, int cell_value)
{
	m_scale = 4;
	float x1, y1, x2, y2;
	x1 =	 x * m_scale;
	y1 =	 y * m_scale;
	x2 = (x+1) * m_scale;
	y2 = (y+1) * m_scale;

	ALLEGRO_COLOR c;

	if( ((int)m_center.x()) == x && ((int)m_center.y() == y) )
	{
		c = al_map_rgb(255, 255, 50);
	}
	else if( m_levisible->get(x, y) == 1 )
	{
		std::cout << "YES!" << std::endl;
		if( cell_value == 0 )
		{
			c = al_map_rgb(63, 63, 116);
		}
		else if( cell_value == 1 )
		{
			c = al_map_rgb(69, 40, 60);
		}
		else if( cell_value == 2 )
		{
			c = al_map_rgb(238, 195, 154);
		}
		else if( cell_value == 3 )
		{
			c = al_map_rgb(172, 50, 50);
		}
		else if( cell_value == 4 )
		{
			c = al_map_rgb(106, 190, 48);
		}
		else if( cell_value == 5 )
		{
			c = al_map_rgb(95, 205, 228);
		}
	}
	else // 63, 63, 116
	{
		c = al_map_rgb(69, 40, 60);
	}

	al_draw_filled_rectangle(x1, y1, x2, y2, c);

}


TileMapRenderer::TileMapRenderer(Matrix2Di::SharedPtr map, std::vector<ALLEGRO_BITMAP *> tiles)
	: MapRenderer(map), m_tiles(tiles) { }

void TileMapRenderer::renderCell(int x, int y, int cell_value)
{
	al_draw_bitmap(m_tiles[cell_value],
				   x * al_get_bitmap_width(m_tiles[cell_value]),
				   y * al_get_bitmap_height(m_tiles[cell_value]), 0);
}
