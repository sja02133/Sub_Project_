
// Sample_DBMS_KDYDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Sample_DBMS_KDY.h"
#include "Sample_DBMS_KDYDlg.h"
#include "afxdialogex.h"
#include "DataBaseRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDataBaseRec em;
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


// CSampleDBMSKDYDlg dialog



CSampleDBMSKDYDlg::CSampleDBMSKDYDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAMPLE_DBMS_KDY_DIALOG, pParent)
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
	, m_strAuth(_T(""))
	, m_strServerName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleDBMSKDYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVER_TYPE, m_cComboType);
	DDX_Control(pDX, IDC_COMBO_SERVER_NAME, m_cComboServerName);
	DDX_Control(pDX, IDC_COMBO_AUTH, m_cComboAuth);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_CHECK_REMEMBER, m_cCheckRemember);
	DDX_CBString(pDX, IDC_COMBO_AUTH, m_strAuth);
	DDX_CBString(pDX, IDC_COMBO_SERVER_NAME, m_strServerName);
}

BEGIN_MESSAGE_MAP(CSampleDBMSKDYDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_AUTH, &CSampleDBMSKDYDlg::OnSelchangeComboAuth)
	ON_CBN_SELENDOK(IDC_COMBO_AUTH, &CSampleDBMSKDYDlg::OnSelendokComboAuth)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSampleDBMSKDYDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CSampleDBMSKDYDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CSampleDBMSKDYDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_RADIO_CDATABASE, &CSampleDBMSKDYDlg::OnBnClickedRadioCdatabase)
	ON_BN_CLICKED(IDC_RADIO_ODBC, &CSampleDBMSKDYDlg::OnBnClickedRadioOdbc)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSampleDBMSKDYDlg message handlers

BOOL CSampleDBMSKDYDlg::OnInitDialog()
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
	//connectDB();
	settingAuthentication();

	CString str = L"";
	int check = 0;
	str = AfxGetApp()->GetProfileStringW(_T("String"), _T("rem_ip"));
	SetDlgItemText(IDC_COMBO_SERVER_NAME, str);
	str = L"";
	str = AfxGetApp()->GetProfileStringW(_T("String"), _T("rem_username"));
	SetDlgItemText(IDC_EDIT_USERNAME, str);
	str = L"";
#ifdef DEBUG
	str = AfxGetApp()->GetProfileStringW(_T("String"), _T("rem_password"));
	SetDlgItemText(IDC_EDIT_PASSWORD, str);
	str = L"";
#endif

	check = AfxGetApp()->GetProfileIntW(_T("Int"), _T("rem_auth"),0);
	m_cComboAuth.SetCurSel(check);
	if (check == 1) {
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);

	}

	check = AfxGetApp()->GetProfileIntW(_T("Int"), _T("rem_connect_type"),0);
	if (0 == check) {
		((CButton*)GetDlgItem(IDC_RADIO_CDATABASE))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_ODBC))->SetCheck(FALSE);
	}
	else {
		((CButton*)GetDlgItem(IDC_RADIO_CDATABASE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_ODBC))->SetCheck(TRUE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSampleDBMSKDYDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSampleDBMSKDYDlg::OnPaint()
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
HCURSOR CSampleDBMSKDYDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSampleDBMSKDYDlg::settingAuthentication() {
	AuthenStruct auth;
	int index = 4;
	UpdateData(FALSE);

	for (int i = 0; i < index; i++) {
		m_cComboAuth.AddString(auth.auth[i]);
	}
	m_cComboAuth.SetCurSel(0);
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CSampleDBMSKDYDlg::OnSelchangeComboAuth()
{
	// TODO: Add your control notification handler code here
	AuthenStruct auth;

	if (m_cComboAuth.GetCurSel() == 1) {
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
	}
	else {
		m_strUserName = L"";
		m_strPassword = L"";
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
	}
	UpdateData(TRUE);
}


void CSampleDBMSKDYDlg::OnSelendokComboAuth()
{
	// TODO: Add your control notification handler code here
	
}


void CSampleDBMSKDYDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	
	CString str = L"";
	int check = 0;
	GetDlgItemText(IDC_COMBO_SERVER_NAME, str);
	if (str.IsEmpty())
		MessageBox(L"IP 확인 필요!");
	else {
		AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_ip"), str);
		GetDlgItemText(IDC_EDIT_USERNAME, str);
		AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_username"), str);

		if (((CButton*)GetDlgItem(IDC_RADIO_CDATABASE))->GetCheck()) {
			check = 0;
			AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_connect_type"), check);
			theApp.odbcCheck = FALSE;
		}
		else if(((CButton*)GetDlgItem(IDC_RADIO_ODBC))->GetCheck()) {
			check = 1;
			AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_connect_type"), check);
			theApp.odbcCheck = TRUE;
		}

		check = m_cComboAuth.GetCurSel();
		AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_auth"), check);

		if (((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->GetCheck()) {
			GetDlgItemText(IDC_EDIT_PASSWORD, str);
			AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_password"), str);
		}
		UpdateData(TRUE);




		int ipIndex = 0, portIndex = 0;
		
		info.name = m_strUserName;
		info.password = m_strPassword;
		ipIndex = m_strServerName.Find(',', 1);
		info.ip = m_strServerName.Left(ipIndex);

		info.port = m_strServerName.Mid(ipIndex + 1, m_strServerName.GetLength());
		info.database = L"";
		
		connectDB(&info);

		cDlgDbQuary dlg;
		dlg.DoModal();
	}
}

BOOL CSampleDBMSKDYDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	HWND hWnd = pMsg->hwnd;
	HDC hdc = ::GetDC(hWnd);
	CDC* pDC = GetDC();
	CString str = L"";
	
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			// ENTER KEY
			GetDlgItemText(IDC_COMBO_SERVER_NAME, str);
			if (str.IsEmpty())
				MessageBox(L"IP 확인 필요!");
			else {
				SendMessage(WM_COMMAND, IDC_BUTTON_CONNECT, 0);
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSampleDBMSKDYDlg::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CSampleDBMSKDYDlg::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
	MessageBox(L"도움말 페이지입니다. (현재 미구현)");
}

void CSampleDBMSKDYDlg::OnBnClickedRadioCdatabase()
{
	// TODO: Add your control notification handler code here
	CButton* button = (CButton*)GetDlgItem(IDC_RADIO_ODBC);
	if (button->GetCheck())
		button->SetCheck(FALSE);
	theApp.odbcCheck = FALSE;
}


void CSampleDBMSKDYDlg::OnBnClickedRadioOdbc()
{
	// TODO: Add your control notification handler code here
	CButton* button = (CButton*)GetDlgItem(IDC_RADIO_CDATABASE);
	if (button->GetCheck())
		button->SetCheck(FALSE);

	theApp.odbcCheck = TRUE;
}

void CSampleDBMSKDYDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CString str = L"";
	int check = 0;
	GetDlgItemText(IDC_COMBO_SERVER_NAME, str);
	AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_ip"), str);
	GetDlgItemText(IDC_EDIT_USERNAME, str);
	AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_username"), str);

	if (((CButton*)GetDlgItem(IDC_RADIO_CDATABASE))->GetCheck()) {
		check = 0;
		AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_connect_type"), check);
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_ODBC))->GetCheck()) {
		check = 1;
		AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_connect_type"), check);
	}

	check = m_cComboAuth.GetCurSel();
	AfxGetApp()->WriteProfileInt(_T("Int"), _T("rem_auth"), check);

	if (((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->GetCheck()) {
		GetDlgItemText(IDC_EDIT_PASSWORD, str);
		AfxGetApp()->WriteProfileStringW(_T("String"), _T("rem_password"), str);
	}



	CDialogEx::OnClose();
}
