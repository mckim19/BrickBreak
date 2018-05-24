#pragma once
#if !defined(__BACKIMAGE_H)
#define __BACKIMAGE_H

class CBackImage : public CBitmap
{
protected:
	CDC m_BitmapDC;
	CBitmap* m_pOldBitmap;
	CSize m_ImageSize;

	void DeleteBitmap();

public:
	CBackImage();
	~CBackImage();

	void Create(CDC *pDC, int nWidth, int nHeight);		//Bitmap 생성, 크기가 바뀔때 삭제후 생성함
	CDC* GetBitmapDC(CDC* pDC);				//Bitmap 핸들을 얻는다.
	void ReleaseBitmapDC();					//Bitmap 핸들을 Release한다.
	void Draw(CDC *pDC);					//Bitmap의 그림을 화면상에 그린다.
};

#endif