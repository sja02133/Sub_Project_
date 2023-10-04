
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "notepad_KDY_SDI.h"

#include "MainFrm.h"

#include "notepad_KDY_SDIDoc.h"
#include "notepad_KDY_SDIView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_Menu_SAVE_AS, &CMainFrame::OnMenuSaveAs)
	ON_UPDATE_COMMAND_UI(ID_Menu_SAVE_AS, &CMainFrame::OnUpdateMenuSaveAs)
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
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//none create wndToolBar
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	*/
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	/*
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

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


// CMainFrame message handlers
void OnUpdatePos(CCmdUI* pCmdUI) {
	pCmdUI->Enable();
}


void CMainFrame::OnMenuSaveAs()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnUpdateMenuSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	pFrame->cView = pMainWnd->GetActiveView();
	pFrame->cDoc = pMainWnd->GetActiveDocument();

	CnotepadKDYSDIView* pView = (CnotepadKDYSDIView*)cView;
	CnotepadKDYSDIDoc* pDoc = (CnotepadKDYSDIDoc*)cDoc;

	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name
	pView->getting_TitleName();

	//*이 있을때만 들어가야한다.

	if (pDoc->titleFullName.Find(L"*") == 0) {
		// none exist initialize_title or exist save data
		str = pDoc->titleFullName;
		if (pDoc->fileInfo.filePath.IsEmpty()) {
			idNum = buttonText();
		}
		else {
			idNum = buttonTextWithFile(pDoc->fileInfo.filePath);
		}

		if (IDYES == idNum && !pDoc->fileInfo.filePath.IsEmpty()) {
			//pView->saveFile(pDoc->fileInfo.filePath);
			pView->saveFile_second(pDoc->fileInfo.filePath);
		}
		else if (IDYES == idNum && pDoc->fileInfo.filePath.IsEmpty()) {
			HRESULT ret;
			eBOM bom;
			CString file_path = L"";
			TCHAR* file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
			ret = BasicFileOpen(file, FALSE, bom);
			if (SUCCEEDED(ret)) {
				//success to save file to disk
				pView->settingFileInformation(file, bom);
				//pView->saveFile(pDoc->fileInfo.filePath);
				pView->saveFile_second(pDoc->fileInfo.filePath);

				//if save file success
				pFrame->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
			}
			free(file);
		}
	}
	CFrameWnd::OnClose();
}
