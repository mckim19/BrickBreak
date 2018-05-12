#include "stdafx.h"
#include "Bar.h"

Bar::Bar()
{

}

Bar::~Bar()
{
}

void Bar::MoveToLeft()
{
	if (x1 > 0)
	{
		x1 -= 10;
		x2 = x1 + width;
	}
}

void Bar::MoveToRight(int m_WinRight)
{
	if (x2 < m_WinRight)
	{
		x1 += 10;
		x2 = x1 + width;
	}
}

void Bar::MoveToCenter(int mtc_x1, int mtc_y1, int width, int hight)
{
	x1 = mtc_x1;
	y1 = mtc_y1;
	x2 = x1 + width;
	y2 = y1 + hight;
}