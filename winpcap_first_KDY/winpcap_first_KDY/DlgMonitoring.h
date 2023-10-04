#pragma once
// DlgMonitoring dialog

#include "SplitterControl.h"
#include "SSplitter.h"

#include "timeAnythingKDY.h"

#include <sstream>

#include <chrono>

#include "FileDialogCustom.h"

#define EXIT_STATE_NONE 0
#define EXIT_STATE_START 1
#define EXIT_STATE_STOP 2
#define EXIT_STATE_PAUSE 3

class DlgMonitoring : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMonitoring)

public:
	DlgMonitoring(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DlgMonitoring();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINPCAP_FIRST_KDY_DIALOG_MONITORING1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	void settingList();
	CSSplitter m_MainDlgSplitterPane;
	CSSplitter m_DownDlgSplitterPane;

	afx_msg void OnTvnSelchangedTreePacketInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

public:
	CEdit m_editPacketRaw;
	static UINT updateEditControl(LPVOID pvParam);
	CListCtrl m_cListCtrl;

	CString interface_id;
	CString interfaceDescription;
	CString interfaceName;


	UINT exitState = 0;
	// 0 -> none
	// 1 -> start or run
	// 2 -> stop or end
	// 3 -> pause

	CWinThread* m_pThread;

	void settingEditControl(u_char* pktData, int pkt_len);
	int gettimeofday(struct timeval* tv, struct timezone* tz);
	int chronoExample(struct timeval* tv);
	void closeListPacket();

	afx_msg void OnItemchangedListPacket(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPacket(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnLvnDeleteitemListPacket(NMHDR* pNMHDR, LRESULT* pResult);
};

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
