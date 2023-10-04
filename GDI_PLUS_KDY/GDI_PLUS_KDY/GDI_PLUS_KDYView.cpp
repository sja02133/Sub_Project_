
// GDI_PLUS_KDYView.cpp : implementation of the CGDIPLUSKDYView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI_PLUS_KDY.h"
#endif

#include "GDI_PLUS_KDYDoc.h"
#include "GDI_PLUS_KDYView.h"

#include <corecrt_math_defines.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGDIPLUSKDYView

IMPLEMENT_DYNCREATE(CGDIPLUSKDYView, CView)

BEGIN_MESSAGE_MAP(CGDIPLUSKDYView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
//	ON_COMMAND(ID_BUTTON_ARROW_LINE, &CGDIPLUSKDYView::OnButtonArrowLine)
//	ON_COMMAND(ID_BUTTON_CIRCLE, &CGDIPLUSKDYView::OnButtonCircle)
//	ON_COMMAND(ID_BUTTON_RECTANGLE, &CGDIPLUSKDYView::OnButtonRectangle)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// CGDIPLUSKDYView construction/destruction

CGDIPLUSKDYView::CGDIPLUSKDYView() noexcept
{
	// TODO: add construction code here
	this->m_bDrawing = FALSE;
	this->figureManager = new Figure_manager();



}

CGDIPLUSKDYView::~CGDIPLUSKDYView()
{

}

BOOL CGDIPLUSKDYView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDIPLUSKDYView drawing

void CGDIPLUSKDYView::OnDraw(CDC* pDC)
{
	CGDIPLUSKDYDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	

}


// CGDIPLUSKDYView printing

BOOL CGDIPLUSKDYView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIPLUSKDYView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIPLUSKDYView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDIPLUSKDYView diagnostics

#ifdef _DEBUG
void CGDIPLUSKDYView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIPLUSKDYView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDIPLUSKDYDoc* CGDIPLUSKDYView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIPLUSKDYDoc)));
	return (CGDIPLUSKDYDoc*)m_pDocument;
}
#endif //_DEBUG


// CGDIPLUSKDYView message handlers


int CGDIPLUSKDYView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	

	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CGDIPLUSKDYView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame != nullptr)
	{
		pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	}
}

void CGDIPLUSKDYView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//test
	int k = 2;
	CDC* pDC = GetDC();
	pDC->SetDCPenColor(RGB(0, 0, 255));
	//test Done

	

	Figure_manager* manager = this->figureManager;
	// TODO: Add your message handler code here and/or call default
	this->m_bDrawing = TRUE;
	this->m_ptStart = this->m_ptEnd = point;
	if (theApp.select_figure == 1) {
		this->m_figureType = 1;
	}
	else if (theApp.select_figure == 2) {
		this->m_figureType = 2;
		manager->rect = new CRectangle(point,point);
	}
	else if (theApp.select_figure == 3) {
		this->m_figureType = 3;
		manager->circle = new CCircle(point,point);
	}
	else if (theApp.select_figure == 1000) {
		this->m_figureType = 1000;

		//가장 가까운 도형을 찾는다. 그러기 위해선 해당 좌표의 영향권이 미치는 범위를 만들자.
		// 유클리드?

	}
	CView::OnLButtonDown(nFlags, point);
}

void CGDIPLUSKDYView::OnMouseMove(UINT nFlags, CPoint point)
{
	Figure_manager* manager = this->figureManager;
	// TODO: Add your message handler code here and/or call default
	if (m_bDrawing)
	{
		// 그리기 작업을 위한 CDC 객체 생성
		CDC* pDC = GetDC();

		if (this->m_figureType == 1) {
			pDC->SetROP2(R2_COPYPEN);
			CArrow_line arrow(this->m_ptStart, this->m_ptEnd);
			arrow.Erase(pDC);
			arrow.arrow_lineResize(this->m_ptStart, point);
			arrow.Draw(pDC);
			this->m_ptEnd = point;
		}
		else if (this->m_figureType == 2) {
			//done
			manager->rect->rectangleResize(this->m_ptStart,this->m_ptEnd);
			manager->rect->Erase(pDC);
			manager->rect->rectangleResize(this->m_ptStart, point);
			manager->rect->Draw(pDC);
			this->m_ptEnd = point;
		}
		else if (this->m_figureType == 3) {
			//done
			manager->circle->circleResize(this->m_ptStart, this->m_ptEnd);
			manager->circle->Erase(pDC);
			manager->circle->circleResize(this->m_ptStart, point);
			manager->circle->Draw(pDC);
			this->m_ptEnd = point;
		}
		// CDC 객체 해제
		ReleaseDC(pDC);
	}

	CView::OnMouseMove(nFlags, point);
}

int checkCount = 0;

void CGDIPLUSKDYView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	Figure_manager* manager = this->figureManager;

	if (m_bDrawing)
	{
		// 그리기 작업을 위한 CDC 객체 생성
		CDC* pDC = GetDC();
		this->m_ptEnd = point;
		if (this->m_figureType == 1) {
			CArrow_line* arrow = new CArrow_line(this->m_ptStart, point);
			memcpy(arrow, manager->arrow, sizeof(CArrow_line));
			//manager->m_figure_list.push_back(arrow);
			manager->append_figure(arrow);
		}
		else if (this->m_figureType == 2) {
			CRectangle* rect = new CRectangle(this->m_ptStart, point);
			memcpy(rect, manager->rect, sizeof(CRectangle));
			manager->append_figure(rect);
		}
		else if (this->m_figureType == 3) {
			CCircle* circle = new CCircle(this->m_ptStart,point);
			memcpy(circle, manager->circle, sizeof(CCircle));
			manager->append_figure(circle);
		}
		ReleaseDC(pDC);
	}
	m_bDrawing = FALSE;
	this->m_ptEnd = point;
	checkCount++;
	if (checkCount == 5) {
		this->delete_memory_figureManager();
	}
	CView::OnLButtonUp(nFlags, point);
}

//커스텀 함수 모음
void CGDIPLUSKDYView::initialize_beforePoint(before_point& bPoint)
{
	bPoint.m_ptStart.x = 0;
	bPoint.m_ptStart.y = 0;
	bPoint.m_ptEnd.x = 0;
	bPoint.m_ptEnd.y = 0;
}

BOOL CGDIPLUSKDYView::check_Initialized_point(CPoint sPoint, CPoint ePoint)
{
	if (sPoint.x == 0 && sPoint.y == 0
		&& ePoint.x == 0 && ePoint.y == 0)
		return TRUE;
	else
		return FALSE;
}

void CGDIPLUSKDYView::DrawArrowTriangle(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	// 화살표 그리기
	double angle = atan2(ptEnd.y - ptStart.y, ptEnd.x - ptStart.x);
	double arrowSize = 25.0;  // 화살표 크기 조절 가능

	// 화살표 끝점 좌표 계산
	CPoint ptArrowEnd = ptEnd;

	double angle1 = angle + M_PI / 4;
	double angle2 = angle - M_PI / 4;
	int arrowSizeHalf = (arrowSize / 2);

	// 화살표 그리기
	pDC->MoveTo(ptArrowEnd);
	pDC->LineTo((ptArrowEnd.x - arrowSizeHalf * cos(angle1)), (ptArrowEnd.y - arrowSizeHalf * sin(angle1)));
	pDC->MoveTo(ptArrowEnd);
	pDC->LineTo((ptArrowEnd.x - arrowSizeHalf * cos(angle2)), (ptArrowEnd.y - arrowSizeHalf * sin(angle2)));
	
	// 삼각형
	//pDC->LineTo(static_cast<int>(ptArrowEnd.x - arrowSizeHalf * cos(angle1)), static_cast<int>(ptArrowEnd.y - arrowSizeHalf * sin(angle1)));
}

void CGDIPLUSKDYView::delete_memory_figureManager()
{
	Figure_manager* manager = this->figureManager;
	switch (this->m_figureType) {
	case 1:
		//Arrow Line
		delete(manager->arrow);
		break;
	case 2:
		//Rectangle
		delete(manager->rect);
		break;
	case 3:
		//Square
		delete(manager->square);
		break;
	case 4:
		//Circle
		delete(manager->circle);
		break;
	}
}

void CGDIPLUSKDYView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call __super::OnPaint() for painting messages
}


//void CGDIPLUSKDYView::OnButtonArrowLine()
//{
//	// TODO: Add your command handler code here
//	theApp.select_figure = 1;
//	changeButtonColor(ID_BUTTON_ARROW_LINE);
//}


//void CGDIPLUSKDYView::OnButtonCircle()
//{
//	// TODO: Add your command handler code here
//
//}


//void CGDIPLUSKDYView::OnButtonRectangle()
//{
//	// TODO: Add your command handler code here
//
//}

BOOL CGDIPLUSKDYView::changeButtonColor(int ID)
{
	//CButton* cButton = (CButton*)GetDlgItem(ID);
	//SendMessage(WM_DRAWITEM, ID, NULL);
	return TRUE;
}


void CGDIPLUSKDYView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	/*UINT state;
	CDC* dc;
	RECT rctBtn;
	
	CWnd* pWnd = GetDlgItem(nIDCtl);
	if (pWnd != nullptr) {
		dc = pWnd->GetDC();
	}

	rctBtn = lpDrawItemStruct->rcItem;*/
	
	//dc->SetBkColor(RGB(255, 255, 0));

	//dc->Detach();

	__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
