
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

static int ButtonList[] = { 
	ID_BUTTON_ARROW_LINE,
	ID_BUTTON_RECTANGLE,
		ID_BUTTON_CIRCLE 
};

class CMyToolbar : public CToolBar
{
protected:
	virtual void OnDraw(CDC* pDC)
	{

		Bitmap bitmap(_T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\240px-¿¹Á¦.bmp"));

		Graphics graphics(pDC->GetSafeHdc());

		int x = 0;
		int y = 0;

		graphics.DrawImage(&bitmap, x, y);

		graphics.ReleaseHDC(pDC->GetSafeHdc());
	}
};

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
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
	CBitmap m_bitMap;

	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;

	CMFCToolBar customMFCtoolbar;
	CMyToolbar customToolbar;


	void AddToolbarImages(CToolBarCtrl& toolbar);
	void LoadPngToToolBar(CToolBarCtrl& toolbar, LPCTSTR pngFilePath, int imageID);
	void testFreeImageLibrary();
	BOOL check_otherButton_SetCheck();
	void set_Button_SetCheck(int id);

	afx_msg void OnButtonArrowLine();
	afx_msg void OnButtonCircle();
	afx_msg void OnButtonRectangle();
	afx_msg void OnUpdateButtonArrowLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRectangle(CCmdUI* pCmdUI);
	afx_msg void OnButtonMouseCursor();
	afx_msg void OnUpdateButtonMouseCursor(CCmdUI* pCmdUI);
};


