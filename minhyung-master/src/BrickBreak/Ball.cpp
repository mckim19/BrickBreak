#include "stdafx.h"
#include "Ball.h"


Ball::Ball()
{
}


Ball::~Ball()
{
}


void Ball::setPoint(int x, int y, int size) //°øÀ§Ä¡
{
	x1 = x;
	x2 = x1 + size;
	y1 = y;
	y2 = y1 + size;
}
