// toolbarDoc.cpp : implementation of the CToolbarDoc class
//

#include "stdafx.h"
#include "toolbar.h"
#include "MainFrm.h"
#include "toolbarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarDoc

IMPLEMENT_DYNCREATE(CToolbarDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolbarDoc, CDocument)
	ON_COMMAND_RANGE(ID_COLOR_BLACK, ID_COLOR_WHITE, OnColorCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLOR_BLACK, ID_COLOR_WHITE, OnUpdateColorUI)
	ON_COMMAND_RANGE(ID_WIDTH_VTHIN, ID_WIDTH_VTHICK, OnWidthCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_WIDTH_VTHIN, ID_WIDTH_VTHICK, OnUpdateWidthUI)

//	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COLOR, OnUpdateIndicatorColor)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_WIDTH, OnUpdateIndicatorWidth)


	//{{AFX_MSG_MAP(CToolbarDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

const COLORREF CToolbarDoc::m_crColors[8] = {
	RGB(   0,   0,   0),
	RGB(   0,   0, 255),
	RGB(   0, 255,   0),
	RGB(   0, 255, 255),
	RGB( 255,   0,   0),
	RGB( 255,   0, 255),
	RGB( 255, 255,   0),
	RGB( 255, 255, 255)

};





/////////////////////////////////////////////////////////////////////////////
// CToolbarDoc construction/destruction

CToolbarDoc::CToolbarDoc()
{
	// TODO: add one-time construction code here

}

CToolbarDoc::~CToolbarDoc()
{
}

BOOL CToolbarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

		m_nColor = 0;
		m_nWidth = 0;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToolbarDoc serialization

void CToolbarDoc::Serialize(CArchive& ar)
{
	m_oaLines.Serialize(ar);

}

/////////////////////////////////////////////////////////////////////////////
// CToolbarDoc diagnostics

#ifdef _DEBUG
void CToolbarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolbarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolbarDoc commands

CLine* CToolbarDoc::AddLine(CPoint ptFrom, CPoint ptTo)
{
	static UINT nWidths[5] = { 1, 8, 16, 24, 32 };

	CLine* pLine = new CLine(ptFrom, ptTo, m_crColors[m_nColor], nWidths[m_nWidth]);
	try
	{
		m_oaLines.Add(pLine);
		SetModifiedFlag();

	}

	catch(CMemoryException* perr)
	{
		AfxMessageBox("Out of memory", MB_ICONSTOP | MB_OK);

		if(pLine)
		{
			delete pLine;
			pLine = NULL;

		}

		perr->Delete();

	}

	return pLine;

}

int CToolbarDoc::GetLineCount()
{
	return m_oaLines.GetSize();

}

CLine* CToolbarDoc::GetLine(int nIndex)
{
	return (CLine*)m_oaLines[nIndex];

}

void CToolbarDoc::DeleteContents()
{
	int IiCount = m_oaLines.GetSize();
	int IiPos;

	if (IiCount)
	{
		for (IiPos = 0; IiPos < IiCount; IiPos++)
			delete m_oaLines[IiPos];

		m_oaLines.RemoveAll();

	}

	CDocument::DeleteContents();

}

UINT CToolbarDoc::GetColor()
{
	return ID_COLOR_BLACK + m_nColor;

}

void CToolbarDoc::OnColorCommand(UINT nID)
{
	m_nColor = nID - ID_COLOR_BLACK;

}

void CToolbarDoc::OnUpdateColorUI(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetColor() == pCmdUI->m_nID ? 1 : 0);

}

UINT CToolbarDoc::GetWidth()
{
	return ID_WIDTH_VTHIN + m_nWidth;

}

void CToolbarDoc::OnWidthCommand(UINT nID)
{
	m_nWidth = nID - ID_WIDTH_VTHIN;

}

void CToolbarDoc::OnUpdateWidthUI(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetWidth() == pCmdUI->m_nID ? 1 : 0);

}


void CToolbarDoc::SetWidth(UINT nID)
{
	m_nWidth = nID;
	UpdateColorbar(nID);

}

void CToolbarDoc::UpdateColorbar(int nIndex)
{
	// Get the position of the first view
	POSITION pos = GetFirstViewPosition();

	// Did we get a valid position?
	if (pos != NULL)
	{
		// Get a pointer to the view in that position
		CView* pView = GetNextView(pos);

		// Do we have a valid pointer to the view?

		if(pView)
		{
			// Get a pointer to the frame through the view
			CMainFrame* pFrame = (CMainFrame*) pView->GetTopLevelFrame();
			// include "MainFrm.h" 포함(including!)!

			// did we get a pointer to the frame?
			if(pFrame)
				// Update the combo box on the color toolbar
				// through the frame

				pFrame->UpdateWidthCB(nIndex);

		}

	}

}


void CToolbarDoc::OnUpdateIndicatorColor(CCmdUI *pCmdUI)
{
	CString strColor;

	// What is the current color?

	switch(m_nColor)
	{
	case 0:	// black
		strColor = "검정색";
		break;

	case 1: // blue
		strColor = "파란색";
		break;

	case 2: // green
		strColor = "녹색";
		break;

	case 3: // cyan
		strColor = "남색";
		break;

	case 4: // red
		strColor = "빨간색";
		break;

	case 5: // magenta
		strColor = "자주색";
		break;

	case 6: // yellow
		strColor = "노란색";
		break;

	case 7: // white
		strColor = "흰색";
		break;

	}

	// enable the status bar pane
	pCmdUI->Enable(TRUE);
	
	// set the text of the status bar pane
	// to the current color
	pCmdUI->SetText(strColor);

}


void CToolbarDoc::OnUpdateIndicatorWidth(CCmdUI *pCmdUI)
{
	CString strWidth;
	// What is the current width?
	switch(m_nWidth)
	{
	case 0: // very thin
		strWidth = "아주 가늘게";
		break;

	case 1: // thin
		strWidth = "가늘게";
		break;

	case 2: // medium
		strWidth = "보통";
		break;

	case 3: // thick
		strWidth = "두껍게";
		break;

	case 4: // very thick
		strWidth = "아주 두껍게";
		break;

	}

	// Enable the status bar pane
	pCmdUI->Enable(TRUE);

	// Set the text of the status bar pane
	// to the current width
	pCmdUI->SetText(strWidth);



}
