#pragma once
#pragma once
class Ball
{
public:
	Ball();
	~Ball();
	void setPoint(int x, int y, int size);
	int size;
	int  x1, x2, y1, y2;
	int xStep, yStep;
};
