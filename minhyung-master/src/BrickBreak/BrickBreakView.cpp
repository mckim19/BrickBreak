
// BrickBreakView.cpp: CBrickBreakView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "BrickBreak.h"
#endif

#include "BrickBreakDoc.h"
#include "BrickBreakView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Bar.h" //바 헤더
#include "Ball.h" //공 헤더

// CBrickBreakView

Bar bar = Bar(); //바 객체생성
Ball ball = Ball();// 공 객체생성

IMPLEMENT_DYNCREATE(CBrickBreakView, CView)

BEGIN_MESSAGE_MAP(CBrickBreakView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBrickBreakView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CBrickBreakView 생성/소멸

CBrickBreakView::CBrickBreakView()
	:m_xStep(0)
	, m_yStep(0)
	, m_WinBottom(0)
	, m_WinRight(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CBrickBreakView::~CBrickBreakView()
{
}

BOOL CBrickBreakView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CBrickBreakView 그리기

void CBrickBreakView::OnDraw(CDC* pDC)
{
	//Bar나 공을 그려주기위한 색과, 브러쉬
	COLORREF colorBlack = RGB(0, 0, 0);
	COLORREF colorRed = RGB(255, 0, 0);
	COLORREF colorGreen = RGB(0, 255, 0);
	COLORREF colorBlue = RGB(0, 0, 255);

	CBrush blackBrush, whiteBrush, redBrush, greenBrush, blueBrush;

	blackBrush.CreateSolidBrush(colorBlack);
	redBrush.CreateSolidBrush(colorRed);
	greenBrush.CreateSolidBrush(colorGreen);
	blueBrush.CreateSolidBrush(colorBlue);

	//공그려주기(공의 위치값에 공크기를 설정)
	//pDC->Ellipse(ball.x1 - ball.size + bar.hight, ball.y1 - ball.size, ball.x1 + ball.size + bar.hight, ball.y1 + ball.size);
	
	pDC->Ellipse(ball.x1, ball.y1, ball.x2, ball.y2);

	pDC->FillRect(new CRect(bar.x1, bar.y1, bar.x2, bar.y2), &greenBrush);

}


// CBrickBreakView 인쇄


void CBrickBreakView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBrickBreakView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CBrickBreakView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CBrickBreakView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CBrickBreakView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBrickBreakView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBrickBreakView 진단

#ifdef _DEBUG
void CBrickBreakView::AssertValid() const
{
	CView::AssertValid();
}

void CBrickBreakView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBrickBreakDoc* CBrickBreakView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrickBreakDoc)));
	return (CBrickBreakDoc*)m_pDocument;
}
#endif //_DEBUG


// CBrickBreakView 메시지 처리기

//윈도우 사이즈 확인
void CBrickBreakView::OnSize(UINT nType, int cx, int cy)
{
	//윈도우 사이즈 확인
	CView::OnSize(nType, cx, cy);
	m_WinRight = cx;
	m_WinBottom = cy;

	//바의 길이와 높이
	bar.width = 50;
	bar.hight = 10;

	//공의 크기
	ball.size = 20;

	//공의 시작위치(시작을 가운데 아래에서 바위에서)
	ball.setPoint ( m_WinRight / 2 - ball.size/2, m_WinBottom - ball.size - bar.hight , ball.size);

	//바의 시작위치(시작을 가운데 아래에서 바닥과 붙어서)
	bar.MoveToCenter(m_WinRight / 2 - bar.width / 2 , m_WinBottom - bar.hight, bar.width, bar.hight); //바 중간에 만들기
}


void CBrickBreakView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//공 이동 step의 크기만큼
	ball.x1 += m_xStep;
	ball.y1 += m_yStep;
	ball.x2 += m_xStep;
	ball.y2 += m_yStep;

	//벽충돌 처리
	if (ball.x1 < 0 || ball.x2 > m_WinRight)
	{
		ball.x1 -= m_xStep;
		ball.x2 -= m_xStep;
		m_xStep *= -1;
	}
	//위 충돌 처리
	if (ball.y1 < 0)
	{
		//팅기기
		ball.y1 -= m_yStep;
		ball.y2 -= m_yStep;
		m_yStep *= -1;
		
	}
	else if (ball.y2 > m_WinBottom) //바닥 충돌
	{
		//팅기기
		ball.y1 -= m_yStep;
		ball.y2 -= m_yStep;
		m_yStep *= -1;

	

		//끝내기
		MessageBox(_T("게임종료"), _T("에러창"), MB_ICONINFORMATION);
	}

	Invalidate();
	CView::OnTimer(nIDEvent);
}


void CBrickBreakView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 마우스를 누르면 공이동
	m_xStep = (point.x - ball.x1) / 50;
	m_yStep = (point.y - ball.y1) / 50;
	SetTimer(1, 30, NULL);
	CView::OnLButtonDown(nFlags, point); 
}

//키보드가 눌리면 양옆으로 이동
void CBrickBreakView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		bar.MoveToLeft();   //왼쪽방향 키보드 키를 눌렀을때 왼쪽으로 
		break;
	case VK_RIGHT:   
		bar.MoveToRight(m_WinRight);  //오른쪽방향 키보드 키를 눌렀을때 오른쪽으로
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
