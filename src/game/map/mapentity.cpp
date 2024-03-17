
#include "mapentity.h"

#include "../assets.h"

MapEntity::MapEntity(Vec2f position, Matrix2Di::SharedPtr map, Animation::SharedPtr anim)
	: m_position(position), m_map(map)
{
	setAnim(anim);
}

void MapEntity::render()
{
	if( m_anim != nullptr )
	{
		m_animData.render(round(m_position.x()), round(m_position.y()));
	}
}

void MapEntity::update(double delta)
{
	if( m_anim != nullptr )
	{
		m_animData.update(delta);
		m_anim->updateData(m_animData);
	}
}

void MapEntity::place(float x, float y)
{
	m_position.set(x, y);
}

void MapEntity::move(float ox, float oy)
{
	m_position.set(position().x() + ox, position().y() + oy);
}

const Vec2f &MapEntity::position()
{
	return m_position;
}

bool MapEntity::isWalkable(int x_tile, int y_tile)
{
	return m_map->get(x_tile, y_tile) == 0;
}

void MapEntity::setAnim(Animation::SharedPtr anim)
{
	if( anim != m_anim )
	{
		m_animData.reset();
		m_anim = anim;
		m_anim->updateData(m_animData);
	}
}

Vec2i MapEntity::tile()
{
	int tx = floor( (m_position.x() + 8.f) / 16.f );
	int ty = floor( (m_position.y() + 8.f) / 16.f );
	return Vec2i(tx, ty);
}


PlayerPawnMap::PlayerPawnMap(const Vec2f &pos, Matrix2Di::SharedPtr map)
	: MapEntity(pos, map, Assets::instance->playerWalkD)
{
	m_currentTile = tile();
	m_nextTile = tile();
}

void PlayerPawnMap::parseInput()
{
	bool u, d, l, r;

	u = Input::IsKeyDown(ALLEGRO_KEY_UP);
	d = Input::IsKeyDown(ALLEGRO_KEY_DOWN);
	l = Input::IsKeyDown(ALLEGRO_KEY_LEFT);
	r = Input::IsKeyDown(ALLEGRO_KEY_RIGHT);
	m_wantToMove = u || d || l || r;

	if( u )
	{
		m_facing = 'u';
	}

	if( d )
	{
		m_facing = 'd';
	}

	if( r )
	{
		m_facing = 'r';
	}

	if( l )
	{
		m_facing = 'l';
	}
}

void PlayerPawnMap::freeMovement()
{
	float dx, dy;
	dx = dy = 0;
	if( m_wantToMove && m_facing == 'u' )	   dy = -1;
	else if( m_wantToMove && m_facing == 'd' ) dy =  1;
	else if( m_wantToMove && m_facing == 'l' ) dx = -1;
	else if( m_wantToMove && m_facing == 'r' ) dx =  1;
	m_speed.set( dx, dy );
}

void PlayerPawnMap::fixedGridMovement()
{
	if( isNearTile(m_nextTile) )
	{
		if( !m_wantToMove )
		{
			m_speed.set(0, 0);
		}
		else
		{
			// compute next tile depending on facing
			if( m_facing == 'u' && isWalkable(tile().x(), tile().y() - 1) )
			{
				m_nextTile.set( tile().x(), tile().y() - 1 );
				m_speed.set(0, -1);
				setAnim(Assets::instance->playerWalkU);
			}
			else if( m_facing == 'd' && isWalkable(tile().x(), tile().y() + 1) )
			{
				m_nextTile.set( tile().x(), tile().y() + 1 );
				m_speed.set(0, 1);
				setAnim(Assets::instance->playerWalkD);
			}
			else if( m_facing == 'l' && isWalkable(tile().x() - 1, tile().y() ) )
			{
				m_nextTile.set( tile().x() - 1, tile().y() );
				m_speed.set(-1, 0);
				setAnim(Assets::instance->playerWalkL);
			}
			else if( m_facing == 'r' && isWalkable(tile().x() + 1, tile().y() ) )
			{
				m_nextTile.set( tile().x() + 1, tile().y() );
				m_speed.set(1, 0);
				setAnim(Assets::instance->playerWalkR);
			}
			else
			{
				m_speed.set(0,0);
			}
		}
	}

	if( m_speed.x() == 0 && m_speed.y() == 0 )
	{
		switch( m_facing )
		{
		case 'u': setAnim(Assets::instance->playerStandU); break;
		case 'd': setAnim(Assets::instance->playerStandD); break;
		case 'l': setAnim(Assets::instance->playerStandL); break;
		case 'r': setAnim(Assets::instance->playerStandR); break;
		}
	}
}

bool PlayerPawnMap::isNearTile(Vec2i the_tile)
{
	Vec2f possible_tile(the_tile.x() * 16, the_tile.y() * 16);
	float dist = Vec2f::Distance( position(), possible_tile );
	return dist < 1.f;
}

void PlayerPawnMap::applySpeed(double delta)
{
	constexpr float speed = 64.f;
	move(m_speed.x() * delta * speed, m_speed.y() * delta * speed);
}


void PlayerPawnMap::visualUpdate(double delta)
{
	MapEntity::update(delta);

	if (Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) && !m_isInteracting)
	{
		if (m_facing == 'u')
		{
			setAnim(Assets::instance->playerInteractU);
		}
		if (m_facing == 'd')
		{
			setAnim(Assets::instance->playerInteractD);
		}
		if (m_facing == 'l')
		{
			setAnim(Assets::instance->playerInteractL);
		}
		if (m_facing == 'r')
		{
			setAnim(Assets::instance->playerInteractR);
		}

		previousAnim = m_anim;

		m_isInteracting = true;
	}


}

void PlayerPawnMap::clearInteract()
{
	setAnim(previousAnim);
	m_isInteracting = false;

}

void PlayerPawnMap::update(double delta)
{

	parseInput();
	//freeMovement();
	fixedGridMovement();
	applySpeed(delta);

}
