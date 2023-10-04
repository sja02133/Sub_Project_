
// MFC_FileExplorerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_FileExplorer.h"
#include "MFC_FileExplorerDlg.h"
#include "afxdialogex.h"
#include "MenuTab.h"
#include "TreeControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCFileExplorerDlg dialog



CMFCFileExplorerDlg::CMFCFileExplorerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_FILEEXPLORER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCFileExplorerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCFileExplorerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCFileExplorerDlg message handlers
BOOL CMFCFileExplorerDlg::OnInitDialog()
{
	//초기 창 크기값 세팅
	//MoveWindow(0, 0, 1024, 768);
	CDialogEx::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//Control Start
	InitMenuTabControl();
	//Control End




	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCFileExplorerDlg::InitMenuTabControl() {
	CRect rect;
	/*
	if (c_tab.m_hWnd == NULL)
	{
		c_tab.SubclassDlgItem(IDC_TAB_TEST,this);
		c_tab.GetWindowRect(rect);
		ScreenToClient(rect);

		c_tab.InsertItem(0, _T("탭1"));
		c_tab.InsertItem(1, _T("탭2"));
	}
	*/

	this->GetClientRect(&rect);

	//GetWindowRect(&rect);

	c_tab.Create(WS_VISIBLE | WS_CHILD, CRect(10, 10, 320, rect.bottom - 50), this, 400);

	c_tab.InsertItem(0, _T("홈"));
	c_tab.InsertItem(1, _T("공유"));
	c_tab.InsertItem(2, _T("보기"));

	{
		//HOME TAB
		homeTab.Create(IDD_DIALOG_TAB1, &c_tab);
		//homeTab.MoveWindow(2, 26, rect.Width() - 6, rect.Height() - 28);
		homeTab.ShowWindow(SW_SHOW);
	}

	{
		//SHARED TAB
		sharedTab.Create(IDD_DIALOG_TAB2, &c_tab);
		//sharedTab.MoveWindow(2, 26, rect.Width() - 6, rect.Height() - 28);
		sharedTab.ShowWindow(SW_HIDE);
	}

	{
		//SHARED TAB
		showTab.Create(IDD_DIALOG_TAB3, &c_tab);
		//showTab.MoveWindow(2, 26, rect.Width() - 6, rect.Height() - 28);
		showTab.ShowWindow(SW_HIDE);
	}

	//c_tab.SetCurSel(0);
	//dlgTab1.ShowWindow(SW_SHOW);




}

void CMFCFileExplorerDlg::OnTcnSelchangeTabTest(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	int nCurSel = c_tab.GetCurSel();

	switch (nCurSel) {
	case 0:
		homeTab.ShowWindow(SW_SHOW);
		sharedTab.ShowWindow(SW_HIDE);
		showTab.ShowWindow(SW_HIDE);
		break;
	case 1:
		homeTab.ShowWindow(SW_HIDE);
		sharedTab.ShowWindow(SW_SHOW);
		showTab.ShowWindow(SW_HIDE);
		break;
	case 2:
		homeTab.ShowWindow(SW_HIDE);
		sharedTab.ShowWindow(SW_HIDE);
		showTab.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void CMFCFileExplorerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCFileExplorerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCFileExplorerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


