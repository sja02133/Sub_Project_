
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "Multiple_Notepad_KDY.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CString titleStr = L"";
	BOOL checkCancel = TRUE;

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos) {
		CDocTemplate* pdoct = AfxGetApp()->GetNextDocTemplate(pos);
		if (pdoct) {
			POSITION docPos = pdoct->GetFirstDocPosition();
			while (docPos != NULL) {
				CDocument* pDoc = pdoct->GetNextDoc(docPos);
				if (pDoc->IsModified()) {
					POSITION viewPos = pDoc->GetFirstViewPosition();
					while (viewPos) {
						CView* pView = pDoc->GetNextView(viewPos);
						CFrameWnd* pFrame = pView->GetParentFrame();
						CMDIChildWnd* cFrame = (CMDIChildWnd*)pView->GetParentFrame();
//						theApp.pDoc = pDoc;
//						theApp.pView = pView;
						if (pView->GetDocument()->SaveModified()) {
							pFrame->CloseWindow();
							checkCancel = TRUE;
						}
						else {
							checkCancel = FALSE;
						}
//						theApp.pDoc = 0;
//						theApp.pView = 0;
					}
				}
			}
		}
	}

	if (checkCancel != FALSE) {
		AfxGetApp()->m_pDocManager->CloseAllDocuments(FALSE);
		CMDIFrameWnd::OnClose();
	}
}
