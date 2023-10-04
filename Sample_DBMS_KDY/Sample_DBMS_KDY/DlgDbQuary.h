#pragma once
#include "Sample_DBMS_KDY.h"

// cDlgDbQuary dialog
#include "odbc_kdy.h"

struct SORTPARAM {
	int iSortColumn;
	bool bSortDirect;
	CListCtrl* pList;
};

class cDlgDbQuary : public CDialogEx,public COdbc_kdy
{
	DECLARE_DYNAMIC(cDlgDbQuary)
public:
	cDlgDbQuary(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgDbQuary();
	BOOL sendSelectQuery(CString query);
	void sendOtherQuery(CString query);
	int checkQuery(CString query);
	void settingResultMessage(CString str,BOOL isQuery);
	void settingListControl(COdbc_kdy* odbc);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QUARY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_clResult;
	virtual BOOL OnInitDialog();
	CTabCtrl m_tabQuary;

	CString m_strEditResult;

};