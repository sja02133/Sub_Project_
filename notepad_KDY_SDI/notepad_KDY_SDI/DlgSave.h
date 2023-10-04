#pragma once


// cDlgSave dialog

class cDlgSave : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgSave)

public:
	cDlgSave(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgSave();

	CString name;

	CnotepadKDYSDIApp* m_pDlg;
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButtonSave();
	afx_msg void OnClickedButtonNoneSave();
	afx_msg void OnClickedButtonCancel();
};
