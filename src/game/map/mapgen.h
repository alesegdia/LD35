#pragma once

#include <alligator/util/matrix.h>
#include <alligator/util/math.h>
#include <alligator/util/rng.h>

class IMapGenerator
{
public:
	virtual Matrix2Di generate(int width, int height) = 0 ;
};



class RandomWalkerGenerator
{
public:
	RandomWalkerGenerator();

	virtual Matrix2Di generate(int width, int height, float percentage = 0.5f);

private:

	bool validate( Vec2i coord );
	Vec2i walk(Vec2i position);

	Matrix2Di m_outputMap;

};
