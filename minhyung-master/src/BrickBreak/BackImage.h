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

	void Create(CDC *pDC, int nWidth, int nHeight);		//Bitmap ����, ũ�Ⱑ �ٲ� ������ ������
	CDC* GetBitmapDC(CDC* pDC);				//Bitmap �ڵ��� ��´�.
	void ReleaseBitmapDC();					//Bitmap �ڵ��� Release�Ѵ�.
	void Draw(CDC *pDC);					//Bitmap�� �׸��� ȭ��� �׸���.
};

#endif