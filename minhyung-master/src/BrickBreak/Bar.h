#pragma once
class Bar
{

public:
	Bar();
	~Bar();
	void MoveToLeft();
	void MoveToRight(int m_WinRight); 
	void MoveToCenter(int mtc_x1, int mtc_y1, int width, int hight);
	int width, hight, x1, y1, x2, y2, moving;
	bool flag;

};