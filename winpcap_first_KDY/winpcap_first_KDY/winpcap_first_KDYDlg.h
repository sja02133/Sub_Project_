
// winpcap_first_KDYDlg.h : header file
//

#pragma once
struct SORTPARAM
{
	int iSortColumn;
	bool bSortDirect;
	CListCtrl* pList;
};

// CwinpcapfirstKDYDlg dialog
class CwinpcapfirstKDYDlg : public CDialogEx
{
// Construction
public:
	CwinpcapfirstKDYDlg(CWnd* pParent = nullptr);	// standard constructor
	void sortListControl();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void settingNetworkDeviceList();
	void recvPacket();
	std::vector<pcap_if_t> vec_d;
	BOOL m_bAscending;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINPCAP_FIRST_KDY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton cButtonStart;
	afx_msg void OnClickedButtonStart();
	CListCtrl m_ListCtrl;
	afx_msg void OnEnChangeEditFilter();
};
