#include "stdafx.h"
#include "BackImage.h"

CBackImage::CBackImage()
{
	m_ImageSize = CSize(0, 0);
}

CBackImage::~CBackImage()
{
	DeleteBitmap();
}

void CBackImage::DeleteBitmap()
{
	if (operator HBITMAP())
		DeleteObject();
}

void CBackImage::Create(CDC *pDC, int nWidth, int nHeight)
{
	nWidth = max(nWidth, 1);
	nHeight = max(nHeight, 1);
	if (m_ImageSize.cx != nWidth || m_ImageSize.cy != nHeight) {
		m_ImageSize.cx = nWidth;
		m_ImageSize.cy = nHeight;
		DeleteBitmap();
		CreateCompatibleBitmap(pDC, nWidth, nHeight);
	}
}

CDC* CBackImage::GetBitmapDC(CDC* pDC)
{
	m_BitmapDC.CreateCompatibleDC(pDC);
	m_pOldBitmap = m_BitmapDC.SelectObject(this);
	return &m_BitmapDC;
}

void CBackImage::ReleaseBitmapDC()
{
	//	if(m_pOldBitmap){
	m_BitmapDC.SelectObject(m_pOldBitmap);
	m_BitmapDC.DeleteDC();
	//		m_pOldBitmap = NULL;
	//	}
}

void CBackImage::Draw(CDC *pDC)
{
	CDC* pBitmapDC;
	BITMAP bm;

	pBitmapDC = GetBitmapDC(pDC);

	GetBitmap(&bm);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, pBitmapDC, 0, 0, SRCCOPY);

	ReleaseBitmapDC();
}

