#pragma once


// cTestSplitter dialog

class cTestSplitter : public CDialogEx
{
	DECLARE_DYNAMIC(cTestSplitter)

public:
	cTestSplitter(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cTestSplitter();


	CSplitterWnd testSplitter;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEST_SPLITTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
};
