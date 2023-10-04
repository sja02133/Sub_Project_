// toolbarView.cpp : implementation of the CToolbarView class
//

#include "stdafx.h"
#include "toolbar.h"

#include "toolbarDoc.h"
#include "toolbarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarView

IMPLEMENT_DYNCREATE(CToolbarView, CView)

BEGIN_MESSAGE_MAP(CToolbarView, CView)
	//{{AFX_MSG_MAP(CToolbarView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarView construction/destruction

CToolbarView::CToolbarView()
{
	// TODO: add construction code here

}

CToolbarView::~CToolbarView()
{
}

BOOL CToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarView drawing

void CToolbarView::OnDraw(CDC* pDC)
{
	CToolbarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	int IiCount = pDoc->GetLineCount();
	if (IiCount)
	{
		int IiPos;
		CLine *IptLine;

		for(IiPos = 0; IiPos < IiCount; IiPos++)
		{
			IptLine = pDoc->GetLine(IiPos);
			IptLine->Draw(pDC);

		}

	}

}

/////////////////////////////////////////////////////////////////////////////
// CToolbarView printing

BOOL CToolbarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToolbarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToolbarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarView diagnostics

#ifdef _DEBUG
void CToolbarView::AssertValid() const
{
	CView::AssertValid();
}

void CToolbarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolbarDoc* CToolbarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolbarDoc)));
	return (CToolbarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolbarView message handlers

void CToolbarView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	m_ptPrevPos = point;

	CView::OnLButtonDown(nFlags, point);
}

void CToolbarView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(GetCapture() == this)
		ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

void CToolbarView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if(GetCapture() == this)
		{
			CClientDC dc(this);
			CLine *pLine = GetDocument()->AddLine(m_ptPrevPos, point);

			pLine->Draw(&dc);
			m_ptPrevPos = point;

		}

	}

	CView::OnMouseMove(nFlags, point);
}

void CToolbarView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
	CMenu menu;
	menu.LoadMenu(IDR_CONTEXTMENU);

	CMenu* pContextMenu = menu.GetSubMenu(0);

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x,
		point.y, AfxGetMainWnd());

}
