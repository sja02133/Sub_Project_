#pragma once


// cDlgNewWindow dialog

class cDlgNewWindow : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgNewWindow)

public:
	cDlgNewWindow(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgNewWindow();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEW_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic controlNewWindow;

	CString static_string;

};
