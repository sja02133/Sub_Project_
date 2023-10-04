
// Sample_DBMS_KDYDlg.h : header file
//
#include "DlgDbQuary.h"

#pragma once

// CSampleDBMSKDYDlg dialog
class CSampleDBMSKDYDlg : public CDialogEx
{
// Construction
public:
	CSampleDBMSKDYDlg(CWnd* pParent = nullptr);	// standard constructor
	void settingAuthentication();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLE_DBMS_KDY_DIALOG };
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
	DbConnInfo info;
	CComboBox m_cComboType;
	CComboBox m_cComboServerName;
	CComboBox m_cComboAuth;
	CString m_strUserName;
	CString m_strPassword;
	CButton m_cCheckRemember;
	afx_msg void OnSelchangeComboAuth();
	CString m_strAuth;
	afx_msg void OnSelendokComboAuth();
	afx_msg void OnBnClickedButtonConnect();
	CString m_strServerName;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadioCdatabase();
	afx_msg void OnBnClickedRadioOdbc();
	afx_msg void OnClose();
};
