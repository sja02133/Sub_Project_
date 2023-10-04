#pragma once

// cTextSize dialog

class cTextSize : public cDlgPaint
{
	DECLARE_DYNAMIC(cTextSize)

public:
	cTextSize(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cTextSize();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_CONFIG_TEXT_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTextSizeUp();
	afx_msg void OnBnClickedButtonTextSizeDown();
	afx_msg void OnBnClickedButtonTextSizeCollect();
};
