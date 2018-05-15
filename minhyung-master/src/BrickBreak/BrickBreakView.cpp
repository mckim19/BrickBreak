
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
#include "Brick.h"//벽돌 헤더
#include <random> //random을 위해
#define MOVE 1
#define COLLISION 2
#define STAGE_1 40


// CBrickBreakView

Bar bar = Bar(); //바 객체생성
Ball ball = Ball();// 공 객체생성
Brick brick[50];//벽돌생성
CRect brick3[50]; // 벽돌그리기 생성
int x, y;
int init;


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
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CBrickBreakView 생성/소멸

CBrickBreakView::CBrickBreakView()
	: m_WinBottom(0)
	, m_WinRight(0)
	, mouse_point(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	//스테이지 예시
	stage = 1;
	init = 0;
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
	


	CPen blackPen;
	blackPen.CreatePen(PS_SOLID, 1, colorBlack);


	CBrush blackBrush, whiteBrush, redBrush, greenBrush, blueBrush;

	blackBrush.CreateSolidBrush(colorBlack);
	redBrush.CreateSolidBrush(colorRed);
	greenBrush.CreateSolidBrush(colorGreen);
	blueBrush.CreateSolidBrush(colorBlue);


	//바 그려주기
	pDC->FillRect(new CRect(bar.x1, bar.y1, bar.x2, bar.y2), &greenBrush);
	

	//벽돌 그리기
	if (stage == 1) {
		if (init == 0) {
			for (int i = 0; i < STAGE_1; i++)
			{
				x = (i % 20) * 60 + 50;
				y = i / 20 * 60 + 30;
				brick3[i] = CRect(x, y, x + 50, y + 30);
				brick[i].life = 1;
			}
			init++;
		}
		for (int i = 0; i < STAGE_1; i++)
		{
			if (brick[i].life >= 1)
				pDC->FillRect(brick3[i], &redBrush);
		}
	}
	if (stage == 2) {
		if (init == 0) {
			for (int i = 0; i < 20; i++)
			{
				x = (i % 20) * 60 + 50;
				y = i / 20 * 60 + 30;
				brick3[i] = CRect(x, y, x + 50, y + 30);
				brick[i].life = 1;
			}
			init++;
		}
		for (int i = 0; i < 20; i++)
		{
			if (brick[i].life >= 1)
				pDC->FillRect(brick3[i], &redBrush);
		}

	}
	//공그려주기(공의 위치값에 공크기를 설정)
	pDC->SelectObject(&blackPen);
	pDC->SelectObject(&blackBrush);
	pDC->Ellipse(ball.x1, ball.y1, ball.x2, ball.y2);

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

//CBrickBreakDoc* CBrickBreakView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrickBreakDoc)));
//	return (CBrickBreakDoc*)m_pDocument;
//}
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
	bar.width = 150;
	bar.hight = 10;
	bar.flag = true;


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
	if (nIDEvent == COLLISION) {
		//공 이동 step의 크기만큼
		ball.x1 += ball.xStep;
		ball.y1 += ball.yStep;
		ball.x2 += ball.xStep;
		ball.y2 += ball.yStep;

		//벽충돌 처리
		if (ball.x1 < 0 || ball.x2 > m_WinRight)
		{
			ball.x1 -= ball.xStep;
			ball.x2 -= ball.xStep;
			ball.xStep *= -1;
		}
		//위쪽 벽 충돌 처리
		if (ball.y1 < 0)
		{
			//팅기기
			ball.y1 -= ball.yStep;
			ball.y2 -= ball.yStep;
			ball.yStep *= -1;

		}
		//바닥 충돌 처리
		else if (ball.y2 > m_WinBottom)
		{
			//팅기기
			ball.y1 -= ball.yStep;
			ball.y2 -= ball.yStep;
			ball.yStep *= -1;

			//끝내기
			//공의 시작위치(시작을 가운데 아래에서 바위에서)
			ball.setPoint(m_WinRight / 2 - ball.size / 2, m_WinBottom - ball.size - bar.hight, ball.size);
			//바의 시작위치(시작을 가운데 아래에서 바닥과 붙어서)
			bar.MoveToCenter(m_WinRight / 2 - bar.width / 2, m_WinBottom - bar.hight, bar.width, bar.hight);

			KillTimer(MOVE);
			KillTimer(COLLISION);
			MessageBox(_T("게임종료"), _T("에러창"), MB_ICONINFORMATION);
			bar.flag = true;
			stage = 1;
			Invalidate();
			
		}
		
		



		// Bar와 공의 충돌 처리
		if (((ball.x1 >= bar.x1 && ball.x1 <= bar.x2) || (ball.x2 >= bar.x1 && ball.x2 <= bar.x2)) &&
			((ball.y1 >= bar.y1 && ball.y1 <= bar.y2) || (ball.y2 >= bar.y1 && ball.y2 <= bar.y2)))
		{
			ball.yStep *= -1;
			if (ball.x2 == bar.x1 || ball.x1 == bar.x2)
			{
				ball.xStep *= -1;
			}
		}
		Invalidate();
	}


	//마우스로 움직일때 바 이동
	if (nIDEvent == MOVE){
		if (bar.flag == true)
		{
			if (mouse_point.x > m_WinRight / 2)
				bar.MoveToRight(m_WinRight);
			else if (mouse_point.x < m_WinRight / 2)
				bar.MoveToLeft();
		}
		Invalidate();

	}


	CView::OnTimer(nIDEvent);
}


//키보드가 눌리면 양옆으로 이동
void CBrickBreakView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		bar.MoveToLeft();   //왼쪽방향 키보드 키를 눌렀을때 왼쪽으로 
		brick[1].life--;
		break;
	case VK_RIGHT:   
		bar.MoveToRight(m_WinRight);  //오른쪽방향 키보드 키를 눌렀을때 오른쪽으로
		stage++;
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CBrickBreakView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 마우스를 왼쪽버튼을 누르지 않으면 바가 멈추도록 flag를 변경
	bar.flag = false;
	CView::OnLButtonUp(nFlags, point);
}


void CBrickBreakView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (bar.flag == true) //맨처음 마우스를 눌렀을 때만 게임시작
	{
		std::random_device random_var;

		if (random_var() % 2 == 0)
		{
			ball.xStep = (m_WinRight - m_WinRight / 2) / 80;
			ball.yStep = (0 - m_WinBottom) / 80;
		}
		else
		{
			ball.xStep = (0 - m_WinRight / 2) / 80;
			ball.yStep = (0 - m_WinBottom) / 80;
		}

		SetTimer(MOVE, 1, NULL);
		SetTimer(2, 10, NULL);
	}


	//-----------------------------------------------------------------
	// 마우스 왼쪽버튼을 누르면 바가 이동하도록 flag를 변경
	bar.flag = true;

	CView::OnLButtonDown(nFlags, point);
}



void CBrickBreakView::OnMouseMove(UINT nFlags, CPoint point)
{
	//마우스 왼쪽버튼이 눌린 상태에서 움직이면 마우스의 포인트를 받아서 바가 움직이는데 사용
	if (bar.flag == true)
	{
		mouse_point = point;
		Invalidate();
	}
	
	CView::OnMouseMove(nFlags, point);
}
