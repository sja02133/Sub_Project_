
// winpcap_first_KDYDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "winpcap_first_KDY.h"
#include "winpcap_first_KDYDlg.h"
#include "afxdialogex.h"
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::vector< pcap_t*> pcap_data;
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


// CwinpcapfirstKDYDlg dialog

//#pragma pack(push, 1)
typedef struct _AAA
{
	u_char a[1];
	u_char b[2];
} AAA;
//#pragma pack(pop)


extern unsigned int get_byteToInt(unsigned char* byte);

extern int32_t shift_and_return(u_char* value, size_t bytes);
CwinpcapfirstKDYDlg::CwinpcapfirstKDYDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINPCAP_FIRST_KDY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int a = sizeof(AAA);

	/*
	u_char c = 1;
	u_short s = 1111;
	int32_t i = 84;
	int32_t i32 = shift_and_return((u_char *)&s, 2);
	i32 = get_byteToInt((u_char*)&i);
	i32 = ntohl(i32);
	int a1 = 0;
	*/
}

void CwinpcapfirstKDYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BUTTON_START, cButtonStart);
	DDX_Control(pDX, IDC_LIST_LANCARD, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CwinpcapfirstKDYDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BUTTON_START, &CwinpcapfirstKDYDlg::OnClickedButtonStart)
	ON_EN_CHANGE(IDC_EDIT_FILTER, &CwinpcapfirstKDYDlg::OnEnChangeEditFilter)
END_MESSAGE_MAP()


// CwinpcapfirstKDYDlg message handlers

BOOL CwinpcapfirstKDYDlg::OnInitDialog()
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
//	SetWindowTheme(GetDlgItem(IDC_LIST_LANCARD)->m_hWnd, L"", L"");
	
	//2023.02.06 /info 폴더, /info/server_list.txt 파일을 확인하고 없으면 추가하는 절차 추가
	if (check_existServerListTxt() == FALSE)
	{
		return FALSE;
	}

	get_IpPortList(&vector_serverList);
	/*
	PACKET_INFO a;
	a.sourceIP = L"8.8.8.8";
	a.sourcePort = L"12345";
	a.destinationIP = L"192.168.2.190";
	a.destinationPort = L"5432";
	theApp.pcap_kdy.check_sessionPacket(&a);	//2023.02.10 test 완료
	*/
	//2023.02.03 winpcap -> npcap으로 변경으로 인하여 코드 수정 -> npcap 변경하지 않아도 됨
	//vec_d = theApp.pcap_kdy.npcap_getNetworkDevice();

	if (theApp.pcap_kdy.coreMode == TRUE) {
		theApp.pcap_kdy.main_start();
	}
	else {
		vec_d = theApp.pcap_kdy.getNetworkDevice();
		settingNetworkDeviceList();
	}
	int k = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CwinpcapfirstKDYDlg::settingNetworkDeviceList()
{
	CRect rect;
	CString nameStr = L"",descriptionStr = L"";
	
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, L"No.", LVCFMT_LEFT, (int)rect.Width()*0.15);
	m_ListCtrl.InsertColumn(1, L"Name", LVCFMT_LEFT, rect.Width());
	
	for (int i = 0; i < (int)vec_d.size(); i++) {
		nameStr.Format(L"%d", i + 1);
		m_ListCtrl.InsertItem(i, nameStr);
		nameStr = CString(vec_d.at(i).name);
		nameStr += L", ";
		descriptionStr = CString(vec_d.at(i).description);
		nameStr += descriptionStr;
		m_ListCtrl.SetItemText(i,1, nameStr);
	}
	
	sortListControl();
	//critical section 사용하기
}

void CwinpcapfirstKDYDlg::sortListControl()
{
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++) {
		m_ListCtrl.SetItemData(i, i);
	}
	m_bAscending = !m_bAscending;
	SORTPARAM sortparams;
	sortparams.pList = &m_ListCtrl;
	sortparams.iSortColumn = 0;
	sortparams.bSortDirect = m_bAscending;
	m_ListCtrl.SortItems(CompareFunc, (LPARAM)&sortparams);
}

int CALLBACK CwinpcapfirstKDYDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pList = ((SORTPARAM*)lParamSort)->pList;
	int iSortColumn = ((SORTPARAM*)lParamSort)->iSortColumn;
	bool bSortDirect = ((SORTPARAM*)lParamSort)->bSortDirect;
	LVFINDINFO info1, info2;
	info1.flags = LVFI_PARAM;
	info1.lParam = lParam1;
	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;
	int irow1 = pList->FindItem(&info1, -1);
	int irow2 = pList->FindItem(&info2, -1);
	CString strItem1 = pList->GetItemText(irow1, iSortColumn);
	CString strItem2 = pList->GetItemText(irow2, iSortColumn);

	return bSortDirect ? _tcscmp(strItem1, strItem2) : -_tcscmp(strItem1, strItem2);
}

void CwinpcapfirstKDYDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CwinpcapfirstKDYDlg::OnPaint()
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
HCURSOR CwinpcapfirstKDYDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CwinpcapfirstKDYDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor) {
	case CTLCOLOR_LISTBOX:
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CwinpcapfirstKDYDlg::OnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	std::vector<CString> vec_str;
	CString str = L"";
	CString deviceName = L"";
//	POSITION pos;
	int nItem = 0;
	int position = 0;
	std::thread t;
	GetDlgItemTextW(ID_BUTTON_START, str);
	if (str.Compare(L"START") == 0) {
		//추후 스니핑 모니터링을 실시 후 TRUE일때 실행할 것.
		if (m_ListCtrl.GetSelectedCount()) {
			t = std::thread(&CwinpcapfirstKDYDlg::recvPacket, this);
		//	t.join();
		}

		DlgMonitoring dlg;
		dlg.DoModal();
		try {
			if (pcap_data.size() > 0) {
				for (pcap_t* item : pcap_data) {
					pcap_breakloop(item);
				}
			}
		}
		catch (std::exception& e)
		{
			TRACE(L"%s\n", e.what());
		}
		
		if(t.joinable())
			t.join();
	}
	else {
		SetDlgItemTextW(ID_BUTTON_START, L"START");
	}
}

void CwinpcapfirstKDYDlg::recvPacket()
{
	std::vector<CString> vec_str;
	POSITION pos;
	int nItem = 0;
	int position = 0;
	CString deviceName = L"";
	CString str = L"";
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		TRACE(L"No Item Selected");
	}
	else {
		while (pos) {
			nItem = m_ListCtrl.GetNextSelectedItem(pos);
			deviceName = m_ListCtrl.GetItemText(nItem, 1);
			position = deviceName.Find(L",", 0);
			deviceName = deviceName.Left(position);
			vec_str.push_back(deviceName);
		}
	}
	GetDlgItemText(IDC_EDIT_FILTER, str);
	theApp.pcap_kdy.gettingInfo(vec_str, str);

//	SetDlgItemTextW(ID_BUTTON_START, L"STOP");
}

void CwinpcapfirstKDYDlg::OnEnChangeEditFilter()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
