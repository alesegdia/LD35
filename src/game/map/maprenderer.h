#pragma once

#include <memory>
#include <list>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <alligator/util/matrix.h>
#include <alligator/util/math.h>

class MapRenderer
{
public:
	typedef std::shared_ptr<MapRenderer> SharedPtr;

	MapRenderer(Matrix2Di::SharedPtr map, int ext = 3);

	virtual void center(float x, float y)
	{
		m_center.set(x, y);
	}

	void render();

	virtual void renderCell( int x, int y, int cell_value ) = 0 ;

	bool isVisibleTile( int x, int y )
	{
		if( x < 0 || x > m_map->cols() || y < 0 || y > m_map->rows() )
		{
			return false;
		}
		return m_visible->get(x, y) == 1;
	}

	Matrix2Di::SharedPtr getVisible()
	{
		return m_visible;
	}

protected:
	Matrix2Di::SharedPtr m_map;
	Vec2f m_center;
	Matrix2Di::SharedPtr m_visible;
	int ext;

};

class DebugMapRenderer : public MapRenderer
{
public:
	DebugMapRenderer( Matrix2Di::SharedPtr map, int ext = 3 );

	void renderCell(int x, int y, int cell_value) override;

private:
	float m_scale = 1;

};


class FilteredDebugMapRenderer : public MapRenderer
{
public:
	typedef std::shared_ptr<FilteredDebugMapRenderer> SharedPtr;
	FilteredDebugMapRenderer( Matrix2Di::SharedPtr map, Matrix2Di::SharedPtr visited, int ext = 3 );

	void renderCell(int x, int y, int cell_value) final;

	int width()
	{
		return m_map->cols() * m_scale;
	}

	int height()
	{
		return m_map->rows() * m_scale;
	}

private:
	float m_scale = 1;
	Matrix2Di::SharedPtr m_levisible;

};


class TileMapRenderer : public MapRenderer
{
public:
	TileMapRenderer( Matrix2Di::SharedPtr map, std::vector<ALLEGRO_BITMAP*> tiles);

	void renderCell(int x, int y, int cell_value) override;

private:
	std::vector<ALLEGRO_BITMAP*> m_tiles;

};
