
// BrickBreakView.cpp: CBrickBreakView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "BrickBreak.h"
#endif

#include "BrickBreakDoc.h"
#include "BrickBreakView.h"
#include "BackImage.h"
#include <mmsystem.h> // ����
#pragma comment(lib, "winmm")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Bar.h" //�� ���
#include "Ball.h" //�� ���
#include "Brick.h"//���� ���
#include <random> //������ �����ϰ� �ϱ�����
#include <time.h> //�����۷��������� ����
#define _USE_MATH_DEFINES  //�̹� ���ǵ� PI�� atan�� ����ϱ� ����
#include <math.h>

using namespace std;

#define MOVE 1
#define COLLISION 2
#define ITEM_TIMER 3  //������ Ÿ�̸��۵��� ���� ���
#define STAGE_1 40  //STAGE_1�� �� ��ϰ���
#define STAGE_2 50   //STAGE_2�� �� ��ϰ���
#define STAGE_3 50	//STAGE_3�� �� ��ϰ���
#define STAGE_4 80	//STAGE_4�� �� ��ϰ���
#define BREAK_WIDTH 50 //����ϳ��� �ʺ�
#define BREAK_HEIGHT 30 //����ϳ��� ����
#define TOTAL_ITEM 6  //6������ ������
#define BAR_WIDTH  150 // ���� ����
#define BAR_HEIGHT 10 //���� ����
#define BAR_WIDTH_MAX 300 // ��ũ���� �ִ밪
#define BAR_WIDTH_MIN 75  // ��ũ���� �ּҰ�


// CBrickBreakView

Bar bar = Bar(); //�� ��ü����
Ball ball = Ball();// �� ��ü����
Brick brick[100];//��������
int init;


IMPLEMENT_DYNCREATE(CBrickBreakView, CView)

BEGIN_MESSAGE_MAP(CBrickBreakView, CView)
	// ǥ�� �μ� ����Դϴ�.
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

// CBrickBreakView ����/�Ҹ�

CBrickBreakView::CBrickBreakView()
	: m_WinBottom(0)
	, m_WinRight(0)
	, mouse_point(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	//�������� ����




	Space_flag = 0;

	stage = 1;
	init = 0;
	stage_total_bricks = STAGE_1;

	//������
	PlaySound(MAKEINTRESOURCE(IDR_BGSOUND), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP);
}

CBrickBreakView::~CBrickBreakView()
{
}

BOOL CBrickBreakView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	

	
	return CView::PreCreateWindow(cs);
}


// CBrickBreakView �μ�


void CBrickBreakView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBrickBreakView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CBrickBreakView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CBrickBreakView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CBrickBreakView ����

#ifdef _DEBUG
void CBrickBreakView::AssertValid() const
{
	CView::AssertValid();
}

void CBrickBreakView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBrickBreakDoc* CBrickBreakView::GetDocument() // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrickBreakDoc)));
	return (CBrickBreakDoc*)m_pDocument;
}
#endif //_DEBUG


// CBrickBreakView �޽��� ó����

//������ ������ Ȯ��
void CBrickBreakView::OnSize(UINT nType, int cx, int cy)
{
	//������ ������ Ȯ��
	
	CView::OnSize(nType, cx, cy);

	m_WinRight = cx;
	m_WinBottom = cy;


	Initialize(0);
}


// CBrickBreakView �׸���

void CBrickBreakView::OnDraw(CDC* pDC)
{

	//������ ���� �ذ��� ���� bitmap�� �׷��ش�.
	CBrickBreakDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//1. ȭ�� ũ�⿡ �ش��ϴ� Bitmap�׸��� �����Ѵ�.
	CRect rectClient;
	GetClientRect(&rectClient);
	m_BackImage.Create(pDC, rectClient.right, rectClient.bottom);

	//2. OnDraw�Լ����� ������ Bitmap�ڵ�(����̽� ���ý�Ʈ �ڵ�)�� ��´�. 
	CDC *pMemDC = m_BackImage.GetBitmapDC(pDC);
	pMemDC->BitBlt(0, 0, m_WinRight, m_WinBottom, NULL, 0, 0, WHITENESS);	//����� �Ͼ������ ĥ�Ѵ�.


	//3. pDC��� ������ Bitmap �ڵ��� ����Ͽ� ���ϴ� �׸��� �׸���(ȭ���� Bitmap���� �׸��� �׸���).

	//Bar�� ���� �׷��ֱ����� ����, �귯��
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


	//����ȭ�� 
	pMemDC->SelectObject(&blackBrush);
	pMemDC->FillRect(CRect(0, 0, m_WinRight, m_WinBottom), &blackBrush);

	//�� �׷��ֱ�
	pMemDC->FillRect(CRect(bar.x1, bar.y1, bar.x2, bar.y2), &greenBrush);

	//���� �� ǥ��
	pMemDC->SelectObject(&whitePen);
	pMemDC->MoveTo(0, 30);
	pMemDC->LineTo(m_WinRight, 30);

	//���� �׸���
	for (int i = 0; i < stage_total_bricks; i++)
	{
		if (brick[i].life == 1)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &redBrush);
		if (brick[i].life == 2)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &greenBrush);
		if (brick[i].item_flag == true)
			pMemDC->FillRect(CRect(brick[i].x1, brick[i].y1, brick[i].x2, brick[i].y2), &blueBrush);
	}



	//���׷��ֱ�(���� ��ġ���� ��ũ�⸦ ����)
	pMemDC->SelectObject(&whitePen);
	pMemDC->SelectObject(&whiteBrush);
	pMemDC->Ellipse(ball.x1, ball.y1, ball.x2, ball.y2);
	
	//��� �� ���� ǥ��
	pMemDC->SetBkMode(TRANSPARENT);        // ��� �����ϰ�
	pMemDC->SetTextColor(RGB(255, 255, 255));   // ���ڻ� ����
	pMemDC->TextOut(10, 0, m_Lifestr);
	pMemDC->TextOut(100, 0, m_Scorestr);
	pMemDC->TextOut(200, 0, m_Stage);
	pMemDC->TextOut(400, 0, m_Ball_Bar); //���� ���� ���� Ȯ���� ����


	//4. Bitmap �ڵ��� Release�� �ش�.
	m_BackImage.ReleaseBitmapDC();

	//5. Bitmap�� ������ ȭ��� ����Ѵ�.
	m_BackImage.Draw(pDC);



}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBrickBreakView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == COLLISION)
	{
		//�� �̵� step�� ũ�⸸ŭ
		ball.x1 += ball.xStep;
		ball.y1 += ball.yStep;
		ball.x2 += ball.xStep;
		ball.y2 += ball.yStep;


		//���� ����� �浹 ó��
		
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
					Score += 20; //20���� ���� ���
					m_Scorestr.Format(_T("����: %d"), Score); //���� ���� ǥ��


					if (brick[i].item_flag == true && brick[i].life == 0) //������ ����� ��
					{

						SetTimer(ITEM_TIMER, 10, NULL);   //�����ۿ� ���� Ÿ�̸� �߻�
					}

				}
			}

		}


		//���浹 ó��
		if (ball.x1 < 0 || ball.x2 > m_WinRight)
		{
			ball.x1 -= ball.xStep;
			ball.x2 -= ball.xStep;
			ball.xStep *= -1;
		}
		//���� �� �浹 ó��
		if (ball.y1 < 30) 
		{
			//�ñ��
			ball.y1 -= ball.yStep;
			ball.y2 -= ball.yStep;
			ball.yStep *= -1;

		}
		//�ٴ� �浹 ó��
		else if (ball.y2 > m_WinBottom)
		{
			//�ñ��
			ball.y1 -= ball.yStep;
			ball.y2 -= ball.yStep;
			ball.yStep *= -1;

			//������
			bar.width = BAR_WIDTH;
			//���� ������ġ(������ ��� �Ʒ����� ��������)
			ball.setPoint(m_WinRight / 2 - ball.size / 2, m_WinBottom - ball.size - bar.hight, ball.size);
			//���� ������ġ(������ ��� �Ʒ����� �ٴڰ� �پ)
			bar.MoveToCenter(m_WinRight / 2 - bar.width / 2, m_WinBottom - bar.hight, bar.width, bar.hight);
			
			
			KillTimer(MOVE);
			KillTimer(COLLISION);
			KillTimer(ITEM_TIMER);
			MessageBox(_T("���-1"), _T("����â"), MB_ICONINFORMATION);
			Life--;	// ����, ����ϳ� ����
			m_Lifestr.Format(_T("���: %d"),Life); //���� �� ��� ������Ʈ
			bar.flag = true;
			InvalidateRect(NULL);
			if (Life == 0) // ����� �������
			{
				MessageBox(_T("��������"), _T("����â"), MB_ICONINFORMATION);
				Initialize(0);
			}

			//�������� ������ ����
			for (int i = 0; i < stage_total_bricks; i++)
			{
				if (brick[i].item_flag == true && brick[i].life == 0) //�������� ��������
					brick[i].item_flag = false;
			}
		}



		// Bar�� ���� �浹 ó��
		if (((ball.x1 >= bar.x1 && ball.x1 <= bar.x2) || (ball.x2 >= bar.x1 && ball.x2 <= bar.x2)) &&
			((ball.y1 >= bar.y1 && ball.y1 <= bar.y2) || (ball.y2 >= bar.y1 && ball.y2 <= bar.y2)))
		{
			ball.yStep *= -1;
			//���� �������� �������̰� �ٵ� ���������� "�����̴� ��"�϶� x�� �ӵ� ����
			if (ball.xStep >= 0 && bar.moving == 2 && bar.flag)
			{
					if(ball.xStep+2 != 0)
						ball.xStep += 2;
					printf("�ӵ�����!");
					printf("%d",bar.moving);
			}
			// ���� �������� �����̰� �ٵ�  �������� "�����̴� ��"�϶��� x�� �ӵ� ����
			else if (ball.xStep <= 0 && bar.moving == 1 && bar.flag)
			{
					if(ball.xStep-5 != 0)
						ball.xStep -= 2;
					printf("�ӵ�����!");
					printf("%d", bar.moving);
			}
			if (ball.x2 == bar.x1 || ball.x1 == bar.x2)
			{
				ball.xStep *= -5;
				
			}
		}

		//������� ���ŵǸ� ���� ����
		for (int i = 0; i < stage_total_bricks; i++)
		{
			if (brick[i].life != 0)
				break;
			
			else if (i == stage_total_bricks - 1 && brick[stage_total_bricks - 1].life == 0) //������ ���� ��� ����� life�� 0�̶��
			{
				stage++;
				KillTimer(MOVE);
				KillTimer(COLLISION);
				KillTimer(ITEM_TIMER);
				m_Lifestr.Format(_T("���: %d"), Life); //���� �� ��� ������Ʈ
				bar.flag = true;
				if (stage == 5)
				{
					MessageBox(_T("��������!!"), _T("����â"), MB_ICONINFORMATION);
					Initialize(0);
				}
				MessageBox(_T("�������� Ŭ����!! \n ���� ���������� �Ѿ�ϴ�"), _T("����â"), MB_ICONINFORMATION);
				
				Initialize(1);
			}
		}

	}

	//���콺�� �����϶� �� �̵�
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

	//�������� �����ö� 
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
				if (brick[i].item_flag == true && brick[i].life == 0)  //������ ���� �������ΰ��
				{
					brick[i].y1 += 3;    //�������� ������
					brick[i].y2 += 3;
					//�����۰� �� �浹Ȯ��
					if (((brick[i].x1 >= bar.x1 && brick[i].x1 <= bar.x2) || (brick[i].x2 >= bar.x1 && brick[i].x2 <= bar.x2)) &&
						((brick[i].y1 >= bar.y1 && brick[i].y1 <= bar.y2) || (brick[i].y2 >= bar.y1 && brick[i].y2 <= bar.y2)))
					{
						Score += 50; //�������� ������ 50�� �߰�
						m_Scorestr.Format(_T("����: %d"), Score); //���� ���� ǥ��

						//������ ���� Ȯ��
						switch (brick[i].item)
						{
							//�� ũ�Ⱑ Ŀ���� ������
							case 1:	 
								if (ball.x2 - ball.x1 < 50) //���� ũ�� �ƽø� ����
								{
									ball.x2 += 10;
									ball.y2 += 10;
								}
										
								brick[i].item_flag = false; //������ ������ �����
								break;
							//�� ũ�Ⱑ �۾����� ������
							case 2: if (ball.x1 < (ball.x2 - ball.size / 2) && ball.y1 < (ball.y2 - ball.size / 2)) //���� ũ�� �̴ϸ� ����
									{
										ball.x2 -= ball.size/2;
										ball.y2 -= ball.size/2;
									}
									brick[i].item_flag = false; //������ ������ �����
									break;
							//�� �ӵ��� �������� ������
							case 3: if (ball.xStep > 0)
										ball.xStep += 5;
									else if (ball.xStep < 0)
										ball.xStep -= 5;
									if (ball.yStep > 0)
										ball.yStep += 5;
									else if (ball.yStep < 0)
										ball.yStep -= 5;
									brick[i].item_flag = false; //������ ������ �����
									break;
							//�� �ӵ��� �������� ������
							case 4: if (ball.xStep > 5)
										ball.xStep -= 5;
									else if (ball.xStep < -5)
										ball.xStep += 5;
									if (ball.yStep > 5)
										ball.yStep -= 5;
									else if (ball.yStep < -5)
										ball.yStep += 5;
									brick[i].item_flag = false; //������ ������ �����
									break;
							//���� ũ�Ⱑ Ŀ���� ������
							case 5: 
								if (bar.width < BAR_WIDTH_MAX)
								{
									bar.width += BAR_WIDTH;  //���� ũ�⸦ 2��� ����
								}
								brick[i].item_flag = false; //������ ������ �����
								break;
							//���� ũ�Ⱑ �۾����� ������	
							case 6: 
								if (bar.width > BAR_WIDTH_MIN)
								{
									bar.width -= BAR_WIDTH/2;  //���� ũ�⸦ 1/2�� ����
								}
								brick[i].item_flag = false; //������ ������ �����
								break;
						}
					}
				}
			}
		}
		
	}
	//���� �� Ȯ���� ����
	m_Ball_Bar.Format(_T("����ǥ (x1,y1) (x2,y2) : (%d,%d) (%d,%d) / ����ǥ  (x1,y1) (x2,y2) :  (%d,%d) (%d,%d) ")
		, ball.x1, ball.y1, ball.x2, ball.y2, bar.x1, bar.y1, bar.x2, bar.y2);


	CView::OnTimer(nIDEvent);
}


//Ű���尡 ������ �翷���� �̵�
void CBrickBreakView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nChar)
	{
	case VK_LEFT:
		bar.fast = true; //���� ���� Ű���带 ������� ���콺��ġ�� �״�� ���󰡴� ���� �� ���
		
		break;
	case VK_RIGHT:
		bar.fast = false; //������ ���� Ű���带 ������� �Ϲ������� ���⿡ ���� �����̴� �� ���
		
		break;
	case VK_UP:
		// �� ũ�� Ŀ���� ����� ����
		ball.x2 += 10;
		ball.y2 += 10;
		break;
	case VK_DOWN:
		// �� ũ�� �۰� ����� ����
		if (ball.x1 < ball.x2 && ball.y1 < ball.y2)
		{
			ball.x2 -= 10;
			ball.y2 -= 10;
		}
		break;
	case VK_SPACE:  //SPACE �ѹ� ������ �� ���� �ٽô����� ����
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
			SetTimer(ITEM_TIMER, 10, NULL);   //�����ۿ� ���� Ÿ�̸� �߻�
		}

		break;

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CBrickBreakView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// ���콺�� ���ʹ�ư�� ������ ������ �ٰ� ���ߵ��� flag�� ����
	ClipCursor(NULL);
	bar.flag = false;
	CView::OnLButtonUp(nFlags, point);
}


void CBrickBreakView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetWindowRect(&rect); //���� ����
	ClipCursor(&rect); // ���� ���� Ŀ�� �̵����ϰ� ���´�


	if (bar.flag == true) //��ó�� ���콺�� ������ ���� ���ӽ���
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
	// ���콺 ���ʹ�ư�� ������ �ٰ� �̵��ϵ��� flag�� ����
	bar.flag = true;

	CView::OnLButtonDown(nFlags, point);
}



void CBrickBreakView::OnMouseMove(UINT nFlags, CPoint point)
{
	//���콺 ���ʹ�ư�� ���� ���¿��� �����̸� ���콺�� ����Ʈ�� �޾Ƽ� �ٰ� �����̴µ� ���
	//���� �������¿��� ����Ʈ�� �����ʿ䰡 �����Ű��Ƽ� ������ �غ��ϴ�
	//if (bar.flag == true)
	//{
		mouse_point = point;
		InvalidateRect(NULL);
	//}

	CView::OnMouseMove(nFlags, point);
}


void CBrickBreakView::Initialize(int check)	// üũ��� ������ �־� ó������ ���ư��� �������, �ƴ��� Ȯ�ο�
{

	//���� ���̿� ����
	bar.width = BAR_WIDTH;
	bar.hight = BAR_HEIGHT;
	bar.flag = true;
	bar.fast = true;
	if (check == 0)	//ó������ �ǵ��ư��� �ʱ�ȭ�� ���
	{
		//������ ����� ǥ�ø� �ʱ�ȭ
		Life = 5;
		Score = 0;
	}
	m_Lifestr.Format(_T("���: %d"), Life);
	m_Scorestr.Format(_T("����: %d"), Score);
	m_Stage.Format(_T("��������: %d"), stage);

	//���� ũ��
	ball.size = 20;

	//���� ������ġ(������ ��� �Ʒ����� ��������)
	ball.setPoint(m_WinRight / 2 - ball.size / 2, m_WinBottom - ball.size - bar.hight, ball.size);

	//���� ������ġ(������ ��� �Ʒ����� �ٴڰ� �پ)
	bar.MoveToCenter(m_WinRight / 2 - bar.width / 2, m_WinBottom - bar.hight, bar.width, bar.hight); //�� �߰��� �����


	if (stage == 1)
		stage_total_bricks = STAGE_1;
	else if (stage == 2)
		stage_total_bricks = STAGE_2;
	else if (stage == 3)
		stage_total_bricks = STAGE_3;
	else if (stage == 4)
		stage_total_bricks = STAGE_4;
	

	//�������� �������ֱ� ���� ��������
	int rand_num;
	srand((unsigned)time(NULL));

	//���� �ʱ�ȭ
	for (int i = 0; i < stage_total_bricks; i++)
	{
		brick[i].height = BREAK_WIDTH;
		brick[i].width = BREAK_HEIGHT;
		/*
		rand_num = rand() % 2; // 1/2�� Ȯ���� ó���� ��� ��������
		brick[i].life = rand_num;
		*/
		rand_num = 0;
		if (stage == 1)	//�������� 1�� ����, �� ����
		{
			brick[i].life = 1;
			brick[i].x1 = (i % 20) * 58 + brick[i].height;
			brick[i].x2 = brick[i].x1 + brick[i].height;
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;

			//�������� �����ϴ� ��� ���� ����
			rand_num = rand() % (STAGE_1 / 20) + 1; //��ü��ϼ����� 20�� Ȯ��(�ϴ� ���Ƿ� ����)
		}
		if (stage == 2)	//�������� 2�� ����, �� ����
		{
			if (i % 5 == 2)
				brick[i].life = 2;		//����� ������ �þ����
			else
			{
				brick[i].life = 1;	//��� ������ 1�ΰ���
				rand_num = rand() % (STAGE_1 / 10) + 1; //��ü��ϼ����� 10�� Ȯ��(�ϴ� ���Ƿ� ����)
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
		if (stage == 3)	//�������� 3�� ���� �� ����
		{
			if ((i % 20) >= 5 && (i % 20) < 15)
				brick[i].life = 2;	//��ϻ�������
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
			if(brick[i].life == 1)	//��� ������ 1�ΰ����� ������ ����
				rand_num = rand() % (STAGE_1 / 10) + 1; //��ü��ϼ����� 10�� Ȯ��(�ϴ� ���Ƿ� ����)
		}
		if (stage == 4)	//�������� 4, ���� ��������
		{
			rand_num = rand() % 2; // 1/2�� Ȯ���� ó���� ��� ��������
			brick[i].life = rand_num;
			brick[i].x1 = (i % 20) * 58 + brick[i].height;
			brick[i].x2 = brick[i].x1 + brick[i].height;
			brick[i].y1 = i / 20 * 40 + brick[i].width + 30;
			brick[i].y2 = brick[i].y1 + brick[i].width;
			rand_num = rand() % (STAGE_1 / 10) + 1; //��ü��ϼ����� 10�� Ȯ��(�ϴ� ���Ƿ� ����)
		}
		if (rand_num == 1 && brick[i].life != 0)  //������ ���ٸ� ���������� x
		{
			brick[i].item_flag = true;  //������ ������� 
			brick[i].item = rand() % TOTAL_ITEM + 1; //�������� �� ���� ����ŭ ������ ����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return false;
}
