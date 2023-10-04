
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)



// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CRichEditCtrl m_RichEdit;

	/*
	CMainFrame* pFrame;
	CnotepadKDYSDIApp* pApp;
	CnotepadKDYSDIDoc* pDoc;
	CnotepadKDYSDIView* pView;
	*/
	CView* cView;
	CDocument* cDoc;
// Implementation
public:
	virtual ~CMainFrame();

	void OnUpdatePos(CCmdUI* pCmdUI);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuSaveAs();
	afx_msg void OnUpdateMenuSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnClose();
};


