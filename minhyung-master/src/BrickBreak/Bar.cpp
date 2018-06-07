#include "stdafx.h"
#include "Bar.h"

Bar::Bar()
{

}

Bar::~Bar()
{
}

void Bar::MoveToLeft(int x)
{
	if (fast)
	{
		if (x1 > 0 )
		{
			x1 = x;  
			x2 = x1 + width;
			moving = 1; //바가 왼쪽으로 이동중이다
		}
	}
	else
	{
		if (x1 > 0)
		{
			
			x1 -= 10;
			x2 = x1 + width;
			moving = 1; //바가 왼쪽으로 이동중이다
		}
	}
}

void Bar::MoveToRight(int m_WinRight, int x)
{
	if (fast)
	{
		if (x2 < m_WinRight)
		{
			x1 = x;
			x2 = x1 + width;
			moving = 2; //바가 오른쪽으로 이동중이다
		}
	}
	else
	{
		if (x2 < m_WinRight)
		{
			x1 += 10;
			x2 = x1 + width;
			moving = 2; //바가 오른쪽으로 이동중이다
		}
	}
}

void Bar::MoveToCenter(int mtc_x1, int mtc_y1, int width, int hight)
{
	x1 = mtc_x1;
	y1 = mtc_y1;
	x2 = x1 + width;
	y2 = y1 + hight;
}