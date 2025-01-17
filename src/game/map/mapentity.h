#pragma once

#include <alligator/util/math.h>
#include <alligator/graphics/animation.h>
#include <alligator/input/input.h>
#include <alligator/util/matrix.h>



class MapEntity
{
public:

	MapEntity( Vec2f position, Matrix2Di::SharedPtr map, Animation::SharedPtr anim = nullptr );

	virtual void render();

	virtual void update(double delta);

	void place( float x, float y );
	void move( float ox, float oy );

	const Vec2f& position();

	bool isWalkable( int x_tile, int y_tile );

	Vec2i tile();

protected:
	void setAnim( Animation::SharedPtr anim );
	Animation::SharedPtr m_anim;


private:
	Vec2f m_position;
	AnimationData m_animData;
	Matrix2Di::SharedPtr m_map;



};

class PlayerPawnMap : public MapEntity
{
public:

	typedef std::shared_ptr<PlayerPawnMap> SharedPtr;

	PlayerPawnMap( const Vec2f& pos, Matrix2Di::SharedPtr map  );

	void update(double delta) override;

	void visualUpdate(double delta);

	void clearInteract();

	char facing()
	{
		return m_facing;
	}

private:

	void parseInput();
	void freeMovement();

	void fixedGridMovement();

	bool isNearTile(Vec2i the_tile);

	void applySpeed(double delta);

	char m_facing = 'd';
	bool m_wantToMove = false;
	Vec2f m_speed = Vec2f(0,0);

	Vec2i m_nextTile;
	Vec2i m_currentTile;

	bool m_isInteracting = false;

	Animation::SharedPtr previousAnim;

};

