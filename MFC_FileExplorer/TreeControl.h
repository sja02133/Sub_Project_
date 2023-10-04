#pragma once


// cTreeControl dialog

class cTreeControl : public CDialogEx
{
	DECLARE_DYNAMIC(cTreeControl)

public:
	cTreeControl(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cTreeControl();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = fileExplorer };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
