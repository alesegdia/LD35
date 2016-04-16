#include "maprenderer.h"

MapRenderer::MapRenderer(Matrix2Di::SharedPtr map)
	: m_map(map)
{

}

void MapRenderer::render()
{
	for( int x = 0; x < m_map->cols(); x++ )
	{
		for( int y = 0; y < m_map->rows(); y++ )
		{
			renderCell( x, y, m_map->get(x, y) );
		}
	}
}

DebugMapRenderer::DebugMapRenderer(Matrix2Di::SharedPtr map) : MapRenderer(map) {}

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

TileMapRenderer::TileMapRenderer(Matrix2Di::SharedPtr map, std::vector<ALLEGRO_BITMAP *> tiles)
	: MapRenderer(map), m_tiles(tiles) { }

void TileMapRenderer::renderCell(int x, int y, int cell_value)
{
	al_draw_bitmap(m_tiles[cell_value],
				   x * al_get_bitmap_width(m_tiles[cell_value]),
				   y * al_get_bitmap_height(m_tiles[cell_value]), 0);
}
