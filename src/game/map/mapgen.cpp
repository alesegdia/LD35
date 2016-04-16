
#include "mapgen.h"

RandomWalkerGenerator::RandomWalkerGenerator() : m_outputMap(0,0) {}

Matrix2Di RandomWalkerGenerator::generate(int width, int height, float percentage)
{
	m_outputMap.resize(width, height, 1);
	//Vec2i prev_position(RNG::rng->nextInt(width - 1), RNG::rng->nextInt(height - 1));
	Vec2i prev_position(width / 2, height / 2);
	for( int i = 0; i < width * height * percentage; i++ )
	{
		Vec2i new_position;
		do
		{
			new_position = walk(prev_position);
		}
		while(!validate(new_position));
		prev_position = new_position;
		m_outputMap.set(prev_position.x(), prev_position.y(), 0);
	}

	Matrix2Di output_map(m_outputMap.cols() + 2, m_outputMap.rows() + 2, 1);
	subcopy( m_outputMap, output_map, 1, 1 );
	return output_map;
}

bool RandomWalkerGenerator::validate(Vec2i coord)
{
	bool valid_coords = coord.x() >= 0					&& coord.y() >= 0 &&
			coord.x() < m_outputMap.cols()	&& coord.y() < m_outputMap.rows();
	bool is_solid_tile = valid_coords; //&& m_outputMap.get(coord.x(), coord.y()) == 1;
	return is_solid_tile;
}

Vec2i RandomWalkerGenerator::walk(Vec2i position)
{
	char orientation = RNG::rng->nextBool() ? 'h' : 'v';
	int side = RNG::rng->nextInt(1);
	if( orientation == 'h' )
	{
		if( side == 1 ) position.x(position.x() + 1);
		else position.x(position.x() - 1);
	}
	else
	{
		if( side == 1 ) position.y(position.y() + 1);
		else position.y(position.y() - 1);
	}
	return position;
}
