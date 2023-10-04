
// BOOKMFC_BUTTON_TESTDlg.h : header file
//

#pragma once



// CBOOKMFCBUTTONTESTDlg dialog
class CBOOKMFCBUTTONTESTDlg : public CDialogEx
{
// Construction
public:

	CBOOKMFCBUTTONTESTDlg(CWnd* pParent = nullptr);	// standard constructor
	void setting_IDC_STATIC_BEFORE_NUM();
	void SettingBeforeNumber();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKMFC_BUTTON_TEST_DIALOG };
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
	afx_msg void OnClickedButtonHello();
	afx_msg void OnClickedButtonExit();
	CString edit_str;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT htimer;
	//for checking ADD, SUB, MUL, DIV
	UINT m_option;

	//to new window
	CString captionStr;

	//paint
	
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEqu();
	afx_msg void OnBnClickedButtonSub();
	afx_msg void OnBnClickedButtonMul();
	afx_msg void OnBnClickedButtonDiv();
	afx_msg void OnClickedButtonAdd();
	afx_msg void OnBnClickedButtonNewWindow1();
	afx_msg void OnBnClickedButtonNewWindow2();
	afx_msg void OnBnClickedButtonNewWindow3();
	afx_msg void OnBnClickedButtonNewWindow4();
	afx_msg void OnBnClickedButtonNewWindow5();
	afx_msg void OnBnClickedButtonGotoPaint();
};
