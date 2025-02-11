
// BrickBreakView.h: CBrickBreakView 클래스의 인터페이스
//

#pragma once
#include "BackImage.h"
#include "BrickBreakDoc.h"

class CBrickBreakView : public CView
{
protected: // serialization에서만 만들어집니다.
	CBrickBreakView();
	DECLARE_DYNCREATE(CBrickBreakView)



// 특성입니다.
public:
	//CBrickBreakDoc* GetDocument() const;


// 작업입니다.
public:
	int m_WinRight;	//윈도우의 오른쪽
	int m_WinBottom; //윈도우의 바닥
	CPoint mouse_point; //바 이동을 위한 마우스 포인트
	int stage; // 스테이지를 위한 변수
	int stage_total_bricks; //스테이지에 따라 벽돌개수 변경을 위한 변수
	void Initialize(int check);
	int Life; // 목숨을 카운트 하기 위한 변수
	int Item_BNumber; // 어떤 벽돌에서 아이템이 나오는지 정할 변수
	int Score;	//점수를 세기 위한 변수
	int Space_flag; //게임중지 또는 재시작
	CString m_Lifestr; //목숨 표시를 위한 변수
	CString m_Scorestr; //점수 표시를 위한 변수
	CString m_Stage; //스테이지 표시를 위한 변수
	CString m_Ball_Bar; //공과 바의 움직임 확인을 위한 변수
	CBackImage m_BackImage;
	CBrickBreakDoc* GetDocument();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CBrickBreakView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // BrickBreakView.cpp의 디버그 버전
inline CBrickBreakDoc* CBrickBreakView::GetDocument() const
   { return reinterpret_cast<CBrickBreakDoc*>(m_pDocument); }
#endif

