#pragma once

#include <memory>
#include <list>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <alligator/util/matrix.h>

class MapRenderer
{
public:
	typedef std::shared_ptr<MapRenderer> SharedPtr;

	MapRenderer(Matrix2Di::SharedPtr map);

	void render();

	virtual void renderCell( int x, int y, int cell_value ) = 0 ;

private:
	Matrix2Di::SharedPtr m_map;

};

class DebugMapRenderer : public MapRenderer
{
public:
	DebugMapRenderer( Matrix2Di::SharedPtr map );

	void renderCell(int x, int y, int cell_value) override;

private:
	float m_scale = 1;

};


class TileMapRenderer : public MapRenderer
{
public:
	TileMapRenderer( Matrix2Di::SharedPtr map, std::vector<ALLEGRO_BITMAP*> tiles);

	void renderCell(int x, int y, int cell_value) override;

private:
	std::vector<ALLEGRO_BITMAP*> m_tiles;

};
