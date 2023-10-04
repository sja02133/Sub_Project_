// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "toolbar.h"
#include "toolbarDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_COLORBAR, OnViewColorbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLORBAR, OnUpdateViewColorbar)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_CBWIDTH, OnSelChangeWidth)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_WIDTH,
	ID_INDICATOR_COLOR,		// Added!
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

/*

	// code start!
	// Add the color toolbar

	int iTBCtlID;
	int i;

	// Create the Color Toolbar
	if(!m_wndColorBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS |
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndColorBar.LoadToolBar(IDR_TBCOLOR)) // CBRS_TOP 쓰지 않아서(not using) 연산 오류(Runtime error) 많이 났었음
	{
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create

	}
	// Find the Black button on the toolbar

	iTBCtlID = m_wndColorBar.CommandToIndex(ID_COLOR_BLACK);
	if(iTBCtlID >= 0)
	{
		// Loop through the buttons, setting them to act as radio buttons
		for(i = iTBCtlID; i < (iTBCtlID + 8); i++)
			m_wndColorBar.SetButtonStyle(i, TBBS_CHECKGROUP);

	}

	// End!!

	
	*/



	// Add the Select Toolbar <- 여기서 툴바를 다 로드 하므로 이전껀 생략(Because toolbar is loaded here at all so pass away previous thing
	if(!CreateColorBar())
	{
		TRACE0("Failed to create color toolbar\n");
		return -1;

	}
	// End here;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	// Code Start Again!
	m_wndColorBar.EnableDocking(CBRS_ALIGN_ANY);
	// End!!

	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Code Start Here!
	// Dock the color Toolbar
	DockControlBar(&m_wndColorBar);

	// End!

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSelChangeWidth()
{
	// Get the new combo selection
	int nIndex = m_ctlWidth.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	// Get the active document
	CToolbarDoc* pDoc = (CToolbarDoc*)GetActiveDocument();

	// Do we have a valid document?
	if(pDoc)
		// Set the new drawing width
		pDoc->SetWidth(nIndex);
		
}

void CMainFrame::OnUpdateViewColorbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// Check the state of the color toolbar
	pCmdUI->SetCheck((m_wndColorBar.GetStyle() & WS_VISIBLE) !=0);
}

void CMainFrame::OnViewColorbar() 
{
	// TODO: Add your command handler code here
	
	BOOL bVisible;

	// Check the state of the color toolbar
	bVisible = ((m_wndColorBar.GetStyle() & WS_VISIBLE) != 0);

	// Toggle the color bar
	ShowControlBar(&m_wndColorBar, !bVisible, FALSE);

	// Reshuffle the frame layout
	RecalcLayout();

}

BOOL CMainFrame::CreateColorBar()
{
	int iTBCtlID;
	int i;

	if(!m_wndColorBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndColorBar.LoadToolBar(IDR_TBCOLOR))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;

	}

	iTBCtlID = m_wndColorBar.CommandToIndex(ID_COLOR_BLACK);
	if(iTBCtlID >= 0)
	{
		for (i = iTBCtlID; i < (iTBCtlID + 8); i++)
			m_wndColorBar.SetButtonStyle(i, TBBS_CHECKGROUP);

	}

	// Add the Combo
	int nWidth = 100;
	int nHeight = 125;

	// Configure the combo place holder	
	m_wndColorBar.SetButtonInfo(9, IDC_CBWIDTH, TBBS_SEPARATOR, nWidth);

	// Get the colorbar height
	CRect rect;
	m_wndColorBar.GetItemRect(9, &rect);
	rect.bottom = rect.top + nHeight;

	// Create the combo box

	m_ctlWidth.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, rect, &m_wndColorBar,
		IDC_CBWIDTH);

	// Fill the combo box
	CString szStyle;
	if (szStyle.LoadString(IDS_WIDTH_VTHIN))
		m_ctlWidth.AddString((LPCTSTR)szStyle);

	if (szStyle.LoadString(IDS_WIDTH_THIN))
		m_ctlWidth.AddString((LPCTSTR)szStyle);

	if (szStyle.LoadString(IDS_WIDTH_MEDIUM))
		m_ctlWidth.AddString((LPCTSTR)szStyle);

	if (szStyle.LoadString(IDS_WIDTH_THICK))
		m_ctlWidth.AddString((LPCTSTR)szStyle);

	if (szStyle.LoadString(IDS_WIDTH_VTHICK))
		m_ctlWidth.AddString((LPCTSTR)szStyle);


	return TRUE;
}

void CMainFrame::UpdateWidthCB(int nIndex)
{
	// Set the new seletion in the combo box
	m_ctlWidth.SetCurSel(nIndex); // <- 책대로 하니 오류 m_wndColorbar.m_ctlWidth.SetCruSel(nIndex) <- Error!!!

}
