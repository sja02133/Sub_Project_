#pragma once


// cDlgPaintDraw dialog

class cDlgPaintDraw : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgPaintDraw)

public:
	cDlgPaintDraw(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgPaintDraw();

	CString fileName;
	CString folderPath;
	CString fileFilter;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT_DRAW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void savePaintBMP();
	void savePaintJPG();
	afx_msg void OnClose();
	afx_msg void OnPaint();
};
