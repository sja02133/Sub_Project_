
// MFC_FileExplorerDlg.h : header file
//

#pragma once

#include "MenuTab.h"
#include "DlgTab1.h"
#include "DlgTab2.h"
#include "DlgTab3.h"
#include "DlgTab4.h"

// CMFCFileExplorerDlg dialog
class CMFCFileExplorerDlg : public CDialogEx
{
// Construction
public:
	CMFCFileExplorerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_FILEEXPLORER_DIALOG };
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

private:
	//Menu
	CTabCtrl c_tab;
	cDlgTab1 homeTab;
	cDlgTab2 sharedTab;
	cDlgTab3 showTab;

	void InitMenuTabControl();
	void OnTcnSelchangeTabTest(NMHDR* pNMHDR, LRESULT* pResult);
};
