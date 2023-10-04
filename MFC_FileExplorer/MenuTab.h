#pragma once

#include "DlgTab1.h"
#include "DlgTab2.h"
#include "DlgTab3.h"
#include "DlgTab4.h"

// cMenuTab dialog

class cMenuTab : public CDialogEx
{
	DECLARE_DYNAMIC(cMenuTab)

public:
	cMenuTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cMenuTab();
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = fileExplorer };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton fixedButton;
};
