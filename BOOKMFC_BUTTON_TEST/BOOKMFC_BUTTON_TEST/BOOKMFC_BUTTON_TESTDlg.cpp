
// BOOKMFC_BUTTON_TESTDlg.cpp : implementation file
//

#include "pch.h"

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
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBOOKMFCBUTTONTESTDlg dialog



CBOOKMFCBUTTONTESTDlg::CBOOKMFCBUTTONTESTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOOKMFC_BUTTON_TEST_DIALOG, pParent)
	, edit_str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBOOKMFCBUTTONTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, edit_str);
}

BEGIN_MESSAGE_MAP(CBOOKMFCBUTTONTESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_HELLO, &CBOOKMFCBUTTONTESTDlg::OnClickedButtonHello)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CBOOKMFCBUTTONTESTDlg::OnClickedButtonExit)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EQU, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonEqu)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonSub)
	ON_BN_CLICKED(IDC_BUTTON_MUL, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonMul)
	ON_BN_CLICKED(IDC_BUTTON_DIV, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonDiv)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CBOOKMFCBUTTONTESTDlg::OnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_NEW_WINDOW_1, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow1)
	ON_BN_CLICKED(IDC_BUTTON_NEW_WINDOW_2, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow2)
	ON_BN_CLICKED(IDC_BUTTON_NEW_WINDOW_3, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow3)
	ON_BN_CLICKED(IDC_BUTTON_NEW_WINDOW_4, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow4)
	ON_BN_CLICKED(IDC_BUTTON_NEW_WINDOW_5, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow5)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_PAINT, &CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonGotoPaint)
END_MESSAGE_MAP()


// CBOOKMFCBUTTONTESTDlg message handlers

BOOL CBOOKMFCBUTTONTESTDlg::OnInitDialog()
{
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
	
	htimer = SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBOOKMFCBUTTONTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBOOKMFCBUTTONTESTDlg::OnPaint()
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
HCURSOR CBOOKMFCBUTTONTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBOOKMFCBUTTONTESTDlg::OnClickedButtonHello()
{
	// TODO: Add your control notification handler code here
	//MessageBox(_T("hello"));
	switch (AfxMessageBox(_T("HELLO"), MB_ABORTRETRYIGNORE | MB_ICONQUESTION)) {
	case IDABORT:
		AfxMessageBox(_T("abort"));
		edit_str = _T("abort");
		break;
	case IDRETRY:
		AfxMessageBox(_T("retry"));
		edit_str = _T("retry");
		break;
	case IDIGNORE:
		AfxMessageBox(_T("ignore"));
		edit_str = _T("ignore");
		break;
	default:
		AfxMessageBox(_T("none or error"));
		edit_str = _T("none or error");
		break;
	}
	UpdateData(FALSE);

}


void CBOOKMFCBUTTONTESTDlg::OnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
	//MessageBox(_T("exit"));
	OnOK();
}

static const TCHAR* dayOfWeek_kr[] = {L"월", L"화", L"수", L"목", L"금", L"토", L"일"};

void CBOOKMFCBUTTONTESTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CTime gct = CTime::GetCurrentTime();
	
	CString currentDate = L"";
	CString currentTime = L"";
	int time = 0;
	CString currentTotalTime =L"";
	CString gubun;

	currentDate.Format(_T("%d-%02d-%02d %s요일 "), gct.GetYear(), gct.GetMonth(), gct.GetDay(), dayOfWeek_kr[gct.GetDayOfWeek()]);
	if (gct.GetHour() > 12) {
		gubun = "오후";
		time = gct.GetHour() - 12;
	}
	else {
		gubun = "오전";
		time = gct.GetHour();
	}
	currentTime.Format(_T(" %02d:%02d:%02d"), time, gct.GetMinute(), gct.GetSecond());
		
	currentTotalTime = currentDate + gubun + currentTime;
	GetDlgItem(IDC_STATIC_CUR_TIME)->SetWindowTextW(currentTotalTime);
	Invalidate();

	CDialogEx::OnTimer(nIDEvent);
}


void CBOOKMFCBUTTONTESTDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(htimer);
	CDialogEx::OnClose();
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
}

void CBOOKMFCBUTTONTESTDlg::setting_IDC_STATIC_BEFORE_NUM() {
	CString str = L"";
	GetDlgItem(IDC_EDIT_NUM)->GetWindowTextW(str);
	SetDlgItemText(IDC_STATIC_BEFORE_NUM, str);
}

void CBOOKMFCBUTTONTESTDlg::OnClickedButtonAdd()
{
	CBOOKMFCBUTTONTESTDlg::m_option = 1;
	SettingBeforeNumber();
	// TODO: Add your control notification handler code here
}

void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonSub()
{
	// TODO: Add your control notification handler code here
	CBOOKMFCBUTTONTESTDlg::m_option = 2;
	SettingBeforeNumber();
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonMul()
{
	// TODO: Add your control notification handler code here
	CBOOKMFCBUTTONTESTDlg::m_option = 3;
	SettingBeforeNumber();
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonDiv()
{
	// TODO: Add your control notification handler code here
	CBOOKMFCBUTTONTESTDlg::m_option = 4;
	SettingBeforeNumber();
}

void CBOOKMFCBUTTONTESTDlg::SettingBeforeNumber() {
	CString beforeNum_str = L"";
	CString currentNum_str = L"";
	int beforeNum = 0, currentNum = 0;
	GetDlgItemText(IDC_STATIC_BEFORE_NUM, beforeNum_str);
	GetDlgItemText(IDC_EDIT_NUM, currentNum_str);
	if (beforeNum_str.Compare(L"") == 0) {
		//공백
		SetDlgItemText(IDC_STATIC_BEFORE_NUM, currentNum_str);
	}
	else {
		beforeNum = _ttoi(beforeNum_str);
		currentNum = _ttoi(currentNum_str);
		switch (m_option) {
		case 1:
			beforeNum += currentNum;
			break;
		case 2:
			beforeNum -= currentNum;
			break;
		case 3:
			beforeNum *= currentNum;
			break;
		case 4:
			beforeNum /= currentNum;
			break;
		default:
			TRACE(L"error!");
			break;
		}
		beforeNum_str.Format(L"%d", beforeNum);
		SetDlgItemText(IDC_STATIC_BEFORE_NUM, beforeNum_str);
	}
	SetDlgItemText(IDC_EDIT_NUM, L"");
}

void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonEqu()
{
	CString equ_num = L"";
	CString edit_num = L"";

	GetDlgItemText(IDC_EDIT_NUM, edit_num);
	if (edit_num.Compare(L"") == 0) {
		GetDlgItemText(IDC_STATIC_BEFORE_NUM, equ_num);
		SetDlgItemText(IDC_STATIC_EQUAL, equ_num);
	}
	else {
		SetDlgItemText(IDC_EDIT_NUM, L"");
	}
	// TODO: Add your control notification handler code here
}

void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow1()
{
	GetDlgItemText(IDC_BUTTON_NEW_WINDOW_1, captionStr);
	cDlgNewWindow dlgNewWindow;
	dlgNewWindow.static_string = captionStr;
	dlgNewWindow.DoModal();
	// TODO: Add your control notification handler code here
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow2()
{
	GetDlgItemText(IDC_BUTTON_NEW_WINDOW_2, captionStr);
	
	cDlgNewWindow dlgNewWindow;
	dlgNewWindow.static_string = captionStr;
	dlgNewWindow.DoModal();
	// TODO: Add your control notification handler code here
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow3()
{
	GetDlgItemText(IDC_BUTTON_NEW_WINDOW_3, captionStr);
	cDlgNewWindow dlgNewWindow;
	dlgNewWindow.static_string = captionStr;
	dlgNewWindow.DoModal();
	// TODO: Add your control notification handler code here
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow4()
{
	GetDlgItemText(IDC_BUTTON_NEW_WINDOW_4, captionStr);
	cDlgNewWindow dlgNewWindow;
	dlgNewWindow.static_string = captionStr;
	dlgNewWindow.DoModal();
	// TODO: Add your control notification handler code here
}


void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonNewWindow5()
{
	GetDlgItemText(IDC_BUTTON_NEW_WINDOW_5, captionStr);
	cDlgNewWindow dlgNewWindow;
	dlgNewWindow.static_string = captionStr;
	dlgNewWindow.DoModal();
	// TODO: Add your control notification handler code here
}

void CBOOKMFCBUTTONTESTDlg::OnBnClickedButtonGotoPaint()
{
	// TODO: Add your control notification handler code here
	cDlgPaint dlgPaint;
	dlgPaint.DoModal();
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
