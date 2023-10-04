#pragma once



// cButtonColor dialog
class cButtonColor : public CDialogEx
{
	DECLARE_DYNAMIC(cButtonColor);

public:
	cButtonColor(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cButtonColor();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedButtonColorRed();
	afx_msg void OnBnClickedButtonColorBlack();
	afx_msg void OnBnClickedButtonColorYellow();
	afx_msg void OnBnClickedButtonColorGreen();
	afx_msg void OnBnClickedButtonColorOrange();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};

