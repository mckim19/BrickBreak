
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
#include "BackImage.h"
#include <mmsystem.h> // 사운드
#pragma comment(lib, "winmm")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Bar.h" //바 헤더
#include "Ball.h" //공 헤더
#include "Brick.h"//벽돌 헤더
#include <random> //시작을 랜덤하게 하기위해
#include <time.h> //아이템랜점지정을 위해
#define _USE_MATH_DEFINES  //이미 정의된 PI와 atan을 사용하기 위해
#include <math.h>

using namespace std;

#define MOVE 1
#define COLLISION 2
#define ITEM_TIMER 3  //아이템 타이머작동을 위한 상수
#define STAGE_1 40  //STAGE_1의 총 블록개수
#define STAGE_2 50   //STAGE_2의 총 블록개수
#define STAGE_3 50	//STAGE_3의 총 블록개수
#define STAGE_4 80	//STAGE_4의 총 블록개수
#define BREAK_WIDTH 50 //블록하나의 너비
#define BREAK_HEIGHT 30 //블록하나의 높이
#define TOTAL_ITEM 6  //6종류의 아이템
#define BAR_WIDTH  150 // 바의 길이
#define BAR_HEIGHT 10 //바의 높이
#define BAR_WIDTH_MAX 300 // 바크기의 최대값
#define BAR_WIDTH_MIN 75  // 바크기의 최소값


// CBrickBreakView

Bar bar = Bar(); //바 객체생성
Ball ball = Ball();// 공 객체생성
Brick brick[100];//벽돌생성
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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CBrickBreakView 생성/소멸

CBrickBreakView::CBrickBreakView()
	: m_WinBottom(0)
	, m_WinRight(0)
	, mouse_point(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	//스테이지 예시




	Space_flag = 0;

	stage = 1;
	init = 0;
	stage_total_bricks = STAGE_1;

	//배경사운드
	PlaySound(MAKEINTRESOURCE(IDR_BGSOUND), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP);
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

CBrickBreakDoc* CBrickBreakView::GetDocument() // 디버그되지 않은 버전은 인라인으로 지정됩니다.
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


	Initialize(0);
}


// CBrickBreakView 그리기

void CBrickBreakView::OnDraw(CDC* pDC)
{

	//깜빡임 문제 해결을 위해 bitmap에 그려준다.
	CBrickBreakDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//1. 화면 크기에 해당하는 Bitmap그림을 생성한다.
	CRect rectClient;
	GetClientRect(&rectClient);
	m_BackImage.Create(pDC, rectClient.right, rectClient.bottom);

	//2. OnDraw함수에서 생성된 Bitmap핸들(디바이스 컨택스트 핸들)을 얻는다. 
	CDC *pMemDC = m_BackImage.GetBitmapDC(pDC);
	pMemDC->BitBlt(0, 0, m_WinRight, m_WinBottom, NULL, 0, 0, WHITENESS);	//배경을 하얀색으로 칠한다.


	//3. pDC대신 생성된 Bitmap 핸들을 사용하여 원하는 그림을 그린다(화면대신 Bitmap에다 그림을 그린다).

	//Bar나 공을 그려주기위한 색과, 브러쉬
	COLORREF colorBlack = RGB(0, 0, 0);
	COLORREF colorRed = RGB(255, 0, 0);
	COLORREF colorGreen = RGB(0, 255, 0);
	COLORREF colorBlue = RGB(0, 0, 255);
	COLORREF colorWhite = RGB(255, 255, 255);


	CPen blackPen, whitePen;
	blackPen.CreatePen(PS_SOLID, 1, colorBlack);
	whitePen.CreatePen(PS_SOLID, 1, colorWhite);


	CBrush blackBrush, whiteBrush, redBrush, greenBrush, blueBrush;
	whiteBrush.CreateSolidBrush(colorWhite);
	blackBrush.CreateSolidBrush(colorBlack);
	redBrush.CreateSolidBrush(colorRed);
	greenBrush.CreateSolidBrush(colorGreen);
	blueBrush.CreateSolidBrush(colorBlue);


	//바탕화면 
	pMemDC->SelectObject(&blackBrush);
	pMemDC->FillRect(CRect(0, 0, m_WinRight, m_WinBottom), &blackBrush);

	//바 그려주기
	pMemDC->FillRect(CRect(bar.x1, bar.y1, bar.x2, bar.y2), &greenBrush);

	//위쪽 벽 표현
	pMemDC->SelectObject(&whitePen);
	pMemDC->MoveTo(0, 30);
	pMemDC->LineTo(m_WinRight, 30);

	//벽돌 그리기
	for (int i = 0; i < stage_total_bricks; i++)
	{
		if (brick[i].life == 1)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &redBrush);
		if (brick[i].life == 2)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &greenBrush);
		if (brick[i].item_flag == true)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &blueBrush);
	}



	//공그려주기(공의 위치값에 공크기를 설정)
	pMemDC->SelectObject(&whitePen);
	pMemDC->SelectObject(&whiteBrush);
	pMemDC->Ellipse(ball.x1, ball.y1, ball.x2, ball.y2);
	
	//목숨 및 점수 표현
	pMemDC->SetBkMode(TRANSPARENT);        // 배경 투명하게
	pMemDC->SetTextColor(RGB(255, 255, 255));   // 글자색 변경
	pMemDC->TextOut(10, 0, m_Lifestr);
	pMemDC->TextOut(100, 0, m_Scorestr);
	pMemDC->TextOut(200, 0, m_Stage);
	pMemDC->TextOut(400, 0, m_Ball_Bar); //공과 볼의 에러 확인을 위해


	//4. Bitmap 핸들을 Release해 준다.
	m_BackImage.ReleaseBitmapDC();

	//5. Bitmap의 내용을 화면상에 출력한다.
	m_BackImage.Draw(pDC);



}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBrickBreakView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == COLLISION)
	{
		//공 이동 step의 크기만큼
		ball.x1 += ball.xStep;
		ball.y1 += ball.yStep;
		ball.x2 += ball.xStep;
		ball.y2 += ball.yStep;


		//공과 블록의 충돌 처리
		
		for (int i = 0; i < stage_total_bricks; i++)
		{
			if (brick[i].life != 0)
			{


				if (((ball.x1 >= brick[i].x1 && ball.x1 <= brick[i].x2) || (ball.x2 >= brick[i].x1 && ball.x2 <= brick[i].x2)) &&
					((ball.y1 >= brick[i].y1 && ball.y1 <= brick[i].y2) || (ball.y2 >= brick[i].y1 && ball.y2 <= brick[i].y2)))
				{
					ball.yStep *= -1;
					if (ball.x2 == brick[i].x1 || ball.x1 == brick[i].x2)
						ball.xStep *= -1;
					brick[i].life--;
					Score += 20; //20점씩 점수 상승
					m_Scorestr.Format(_T("점수: %d"), Score); //점수 변동 표시


					if (brick[i].item_flag == true && brick[i].life == 0) //아이템 블록일 때
					{

						SetTimer(ITEM_TIMER, 10, NULL);   //아이템에 대한 타이머 발생
					}

				}
			}

		}


		//벽충돌 처리
		if (ball.x1 < 0 || ball.x2 > m_WinRight)
		{
			ball.x1 -= ball.xStep;
			ball.x2 -= ball.xStep;
			ball.xStep *= -1;
		}
		//위쪽 벽 충돌 처리
		if (ball.y1 < 30) 
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
			bar.width = BAR_WIDTH;
			//공의 시작위치(시작을 가운데 아래에서 바위에서)
			ball.setPoint(m_WinRight / 2 - ball.size / 2, m_WinBottom - ball.size - bar.hight, ball.size);
			//바의 시작위치(시작을 가운데 아래에서 바닥과 붙어서)
			bar.MoveToCenter(m_WinRight / 2 - bar.width / 2, m_WinBottom - bar.hight, bar.width, bar.hight);
			
			
			KillTimer(MOVE);
			KillTimer(COLLISION);
			KillTimer(ITEM_TIMER);
			MessageBox(_T("목숨-1"), _T("에러창"), MB_ICONINFORMATION);
			Life--;	// 실패, 목숨하나 깍임
			m_Lifestr.Format(_T("목숨: %d"),Life); //깍인 후 목숨 업데이트
			bar.flag = true;
			InvalidateRect(NULL);
			if (Life == 0) // 목숨이 없을경우
			{
				MessageBox(_T("게임종료"), _T("에러창"), MB_ICONINFORMATION);
				Initialize(0);
			}

			//떨어지던 아이템 삭제
			for (int i = 0; i < stage_total_bricks; i++)
			{
				if (brick[i].item_flag == true && brick[i].life == 0) //아이템이 떨어질때
					brick[i].item_flag = false;
			}
		}



		// Bar와 공의 충돌 처리
		if (((ball.x1 >= bar.x1 && ball.x1 <= bar.x2) || (ball.x2 >= bar.x1 && ball.x2 <= bar.x2)) &&
			((ball.y1 >= bar.y1 && ball.y1 <= bar.y2) || (ball.y2 >= bar.y1 && ball.y2 <= bar.y2)))
		{
			ball.yStep *= -1;
			//공의 움직임이 오른쪽이고 바도 오른쪽으로 "움직이는 중"일때 x만 속도 증가
			if (ball.xStep >= 0 && bar.moving == 2 && bar.flag)
			{
					if(ball.xStep+2 != 0)
						ball.xStep += 2;
					printf("속도증가!");
					printf("%d",bar.moving);
			}
			// 공의 움직임이 왼쪽이고 바도  왼쪽으로 "움직이는 중"일때만 x만 속도 증가
			else if (ball.xStep <= 0 && bar.moving == 1 && bar.flag)
			{
					if(ball.xStep-5 != 0)
						ball.xStep -= 2;
					printf("속도증가!");
					printf("%d", bar.moving);
			}
			if (ball.x2 == bar.x1 || ball.x1 == bar.x2)
			{
				ball.xStep *= -5;
				
			}
		}

		//모든블록이 제거되면 게임 종료
		for (int i = 0; i < stage_total_bricks; i++)
		{
			if (brick[i].life != 0)
				break;
			
			else if (i == stage_total_bricks - 1 && brick[stage_total_bricks - 1].life == 0) //마지막 까지 모든 블록의 life가 0이라면
			{
				stage++;
				KillTimer(MOVE);
				KillTimer(COLLISION);
				KillTimer(ITEM_TIMER);
				m_Lifestr.Format(_T("목숨: %d"), Life); //깍인 후 목숨 업데이트
				bar.flag = true;
				if (stage == 5)
				{
					MessageBox(_T("게임종료!!"), _T("에러창"), MB_ICONINFORMATION);
					Initialize(0);
				}
				MessageBox(_T("스테이지 클리어!! \n 다음 스테이지로 넘어갑니다"), _T("에러창"), MB_ICONINFORMATION);
				
				Initialize(1);
			}
		}

	}

	//마우스로 움직일때 바 이동
	if (nIDEvent == MOVE) {
		if (bar.flag == true)
		{
			if (mouse_point.x > m_WinRight / 2)
				bar.MoveToRight(m_WinRight, mouse_point.x);
			else if (mouse_point.x < m_WinRight / 2)
				bar.MoveToLeft(mouse_point.x);
			
		}
		
		InvalidateRect(NULL);

	}

	//아이템이 내려올때 
	if (nIDEvent == ITEM_TIMER)
	{
		int item_check = 0;

		for (int i = 0; i < stage_total_bricks; i++)
		{
			if (brick[i].item_flag == true)
				item_check++;
		}

		if (item_check > 0)
		{
			for (int i = 0; i < stage_total_bricks; i++)
			{
				if (brick[i].item_flag == true && brick[i].life == 0)  //생명이 없고 아이템인경우
				{
					brick[i].y1 += 3;    //아이템이 내려옴
					brick[i].y2 += 3;
					//아이템과 바 충돌확인
					if (((brick[i].x1 >= bar.x1 && brick[i].x1 <= bar.x2) || (brick[i].x2 >= bar.x1 && brick[i].x2 <= bar.x2)) &&
						((brick[i].y1 >= bar.y1 && brick[i].y1 <= bar.y2) || (brick[i].y2 >= bar.y1 && brick[i].y2 <= bar.y2)))
					{
						Score += 50; //아이템을 먹으면 50점 추가
						m_Scorestr.Format(_T("점수: %d"), Score); //점수 변동 표시

						//아이템 종류 확인
						switch (brick[i].item)
						{
							//공 크기가 커지는 아이템
							case 1:	 
								if (ball.x2 - ball.x1 < 50) //공의 크기 맥시멈 지정
								{
									ball.x2 += 10;
									ball.y2 += 10;
								}
										
								brick[i].item_flag = false; //아이템 얻은후 사라짐
								break;
							//공 크기가 작아지는 아이템
							case 2: if (ball.x1 < (ball.x2 - ball.size / 2) && ball.y1 < (ball.y2 - ball.size / 2)) //공의 크기 미니멈 지정
									{
										ball.x2 -= ball.size/2;
										ball.y2 -= ball.size/2;
									}
									brick[i].item_flag = false; //아이템 얻은후 사라짐
									break;
							//공 속도가 빨라지는 아이템
							case 3: if (ball.xStep > 0)
										ball.xStep += 5;
									else if (ball.xStep < 0)
										ball.xStep -= 5;
									if (ball.yStep > 0)
										ball.yStep += 5;
									else if (ball.yStep < 0)
										ball.yStep -= 5;
									brick[i].item_flag = false; //아이템 얻은후 사라짐
									break;
							//공 속도가 느려지는 아이템
							case 4: if (ball.xStep > 5)
										ball.xStep -= 5;
									else if (ball.xStep < -5)
										ball.xStep += 5;
									if (ball.yStep > 5)
										ball.yStep -= 5;
									else if (ball.yStep < -5)
										ball.yStep += 5;
									brick[i].item_flag = false; //아이템 얻은후 사라짐
									break;
							//바의 크기가 커지는 아이템
							case 5: 
								if (bar.width < BAR_WIDTH_MAX)
								{
									bar.width += BAR_WIDTH;  //바의 크기를 2배로 만듬
								}
								brick[i].item_flag = false; //아이템 얻은후 사라짐
								break;
							//바의 크기가 작아지는 아이템	
							case 6: 
								if (bar.width > BAR_WIDTH_MIN)
								{
									bar.width -= BAR_WIDTH/2;  //바의 크기를 1/2로 만듬
								}
								brick[i].item_flag = false; //아이템 얻은후 사라짐
								break;
						}
					}
				}
			}
		}
		
	}
	//공과 바 확인을 위해
	m_Ball_Bar.Format(_T("공좌표 (x1,y1) (x2,y2) : (%d,%d) (%d,%d) / 바좌표  (x1,y1) (x2,y2) :  (%d,%d) (%d,%d) ")
		, ball.x1, ball.y1, ball.x2, ball.y2, bar.x1, bar.y1, bar.x2, bar.y2);


	CView::OnTimer(nIDEvent);
}


//키보드가 눌리면 양옆으로 이동
void CBrickBreakView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		bar.fast = true; //왼쪽 방향 키보드를 누를경우 마우스위치를 그대로 따라가는 빠른 바 모드
		
		break;
	case VK_RIGHT:
		bar.fast = false; //오른쪽 방향 키보드를 누를경우 일반적으로 방향에 따라 움직이는 바 모드
		
		break;
	case VK_UP:
		// 공 크기 커지게 만들기 예시
		ball.x2 += 10;
		ball.y2 += 10;
		break;
	case VK_DOWN:
		// 공 크기 작게 만들기 예시
		if (ball.x1 < ball.x2 && ball.y1 < ball.y2)
		{
			ball.x2 -= 10;
			ball.y2 -= 10;
		}
		break;
	case VK_SPACE:  //SPACE 한번 눌렀을 땐 정지 다시누르면 시작
		if (Space_flag == 0)
		{
			Space_flag = 1;
			KillTimer(MOVE);
			KillTimer(COLLISION);
			KillTimer(ITEM_TIMER);

		}
		else
		{
			Space_flag = 0;
			SetTimer(MOVE, 10, NULL);
			SetTimer(2, 10, NULL);
			SetTimer(ITEM_TIMER, 10, NULL);   //아이템에 대한 타이머 발생
		}

		break;

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CBrickBreakView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 마우스를 왼쪽버튼을 누르지 않으면 바가 멈추도록 flag를 변경
	ClipCursor(NULL);
	bar.flag = false;
	CView::OnLButtonUp(nFlags, point);
}


void CBrickBreakView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetWindowRect(&rect); //영역 얻어옴
	ClipCursor(&rect); // 영역 밖은 커서 이동못하게 막는다


	if (bar.flag == true) //맨처음 마우스를 눌렀을 때만 게임시작
	{
		std::random_device random_var;

		if (random_var() % 2 == 0)
		{
			ball.xStep = random_var() % 5 + 5;
			ball.yStep = -7;
		}
		else
		{
			ball.xStep = (random_var() % 5 + 5) * -1;
			ball.yStep = -7;
		}


		SetTimer(MOVE, 10, NULL);
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
	//굳이 눌린상태에서 포인트를 받을필요가 없을거같아서 지워서 해봅니다
	//if (bar.flag == true)
	//{
		mouse_point = point;
		InvalidateRect(NULL);
	//}

	CView::OnMouseMove(nFlags, point);
}


void CBrickBreakView::Initialize(int check)	// 체크라는 변수를 넣어 처음으로 돌아가는 경우인지, 아닌지 확인용
{

	//바의 길이와 높이
	bar.width = BAR_WIDTH;
	bar.hight = BAR_HEIGHT;
	bar.flag = true;
	bar.fast = true;
	if (check == 0)	//처음으로 되돌아가는 초기화일 경우
	{
		//점수와 목숨을 표시를 초기화
		Life = 5;
		Score = 0;
	}
	m_Lifestr.Format(_T("목숨: %d"), Life);
	m_Scorestr.Format(_T("점수: %d"), Score);
	m_Stage.Format(_T("스테이지: %d"), stage);

	//공의 크기
	ball.size = 20;

	//공의 시작위치(시작을 가운데 아래에서 바위에서)
	ball.setPoint(m_WinRight / 2 - ball.size / 2, m_WinBottom - ball.size - bar.hight, ball.size);

	//바의 시작위치(시작을 가운데 아래에서 바닥과 붙어서)
	bar.MoveToCenter(m_WinRight / 2 - bar.width / 2, m_WinBottom - bar.hight, bar.width, bar.hight); //바 중간에 만들기


	if (stage == 1)
		stage_total_bricks = STAGE_1;
	else if (stage == 2)
		stage_total_bricks = STAGE_2;
	else if (stage == 3)
		stage_total_bricks = STAGE_3;
	else if (stage == 4)
		stage_total_bricks = STAGE_4;
	

	//아이템을 지정해주기 위한 랜덤변수
	int rand_num;
	srand((unsigned)time(NULL));

	//벽돌 초기화
	for (int i = 0; i < stage_total_bricks; i++)
	{
		brick[i].height = BREAK_WIDTH;
		brick[i].width = BREAK_HEIGHT;
		/*
		rand_num = rand() % 2; // 1/2의 확률로 처음에 블록 랜덤생성
		brick[i].life = rand_num;
		*/
		rand_num = 0;
		if (stage == 1)	//스테이지 1의 구조, 및 내용
		{
			brick[i].life = 1;
			brick[i].x1 = (i % 20) * 58 + brick[i].height;
			brick[i].x2 = brick[i].x1 + brick[i].height;
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;

			//아이템을 포함하는 블록 램덤 지정
			rand_num = rand() % (STAGE_1 / 20) + 1; //전체블록수분의 20의 확률(일단 임의로 지정)
		}
		if (stage == 2)	//스테이지 2의 구조, 및 내용
		{
			if (i % 5 == 2)
				brick[i].life = 2;		//블록의 생명이 늘어나있음
			else
			{
				brick[i].life = 1;	//블록 생명이 1인곳만
				rand_num = rand() % (STAGE_1 / 10) + 1; //전체블록수분의 10의 확률(일단 임의로 지정)
			}
			if (i >= 20)
			{
				brick[i].x1 = ((i+5)%20)* 58 + brick[i].height;
				brick[i].x2 = brick[i].x1 + brick[i].height;
			}
			else 
			{
				brick[i].x1 = (i % 20) * 58 + brick[i].height;
				brick[i].x2 = brick[i].x1 + brick[i].height;
			}
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;

		}
		if (stage == 3)	//스테이지 3의 구조 및 내용
		{
			if ((i % 20) >= 5 && (i % 20) < 15)
				brick[i].life = 2;	//블록생명증가
			else
				brick[i].life = 1;
			if (i >= 40)
			{
				brick[i].life = 2;
				brick[i].x1 = ((i + 5) % 20) * 58 + brick[i].height;
				brick[i].x2 = brick[i].x1 + brick[i].height;
			}
			else
			{
				brick[i].x1 = (i % 20) * 58 + brick[i].height;
				brick[i].x2 = brick[i].x1 + brick[i].height;
			}
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;
			if(brick[i].life == 1)	//블록 생명이 1인곳에만 아이템 지정
				rand_num = rand() % (STAGE_1 / 10) + 1; //전체블록수분의 10의 확률(일단 임의로 지정)
		}
		if (stage == 4)	//스테이지 4, 랜덤 스테이지
		{
			rand_num = rand() % 2; // 1/2의 확률로 처음에 블록 랜덤생성
			brick[i].life = rand_num;
			brick[i].x1 = (i % 20) * 58 + brick[i].height;
			brick[i].x2 = brick[i].x1 + brick[i].height;
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;
			rand_num = rand() % (STAGE_1 / 10) + 1; //전체블록수분의 10의 확률(일단 임의로 지정)
		}
		if (rand_num == 1 && brick[i].life != 0)  //생명이 없다면 아이템지정 x
		{
			brick[i].item_flag = true;  //아이템 블록지정 
			brick[i].item = rand() % TOTAL_ITEM + 1; //아이템의 총 종류 수만큼 나눠서 넣음
			//brick[i].item = 4; 
		}
		else
		{
			brick[i].item_flag = false;
			brick[i].item = 0;
		}

	}
	init++;
}

BOOL CBrickBreakView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return false;
}
