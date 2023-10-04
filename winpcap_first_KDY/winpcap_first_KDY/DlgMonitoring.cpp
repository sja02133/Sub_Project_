// DlgMonitoring.cpp : implementation file
//

#include "pch.h"
#include "winpcap_first_KDY.h"
#include "DlgMonitoring.h"
#include "afxdialogex.h"

extern std::vector< pcap_t*> pcap_data;

// DlgMonitoring dialog

IMPLEMENT_DYNAMIC(DlgMonitoring, CDialogEx)

DlgMonitoring::DlgMonitoring(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINPCAP_FIRST_KDY_DIALOG_MONITORING1, pParent)
{

}

DlgMonitoring::~DlgMonitoring()
{
}

void DlgMonitoring::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PACKET, m_cListCtrl);
	//  DDX_Control(pDX, IDC_EDIT_PACKET_RAW, m_editPacketRaw);
	DDX_Control(pDX, IDC_EDIT_PACKET_RAW, m_editPacketRaw);
}


BEGIN_MESSAGE_MAP(DlgMonitoring, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PACKET_INFO, &DlgMonitoring::OnTvnSelchangedTreePacketInfo)
//	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER2, OnMaxMinInfo)
//	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER1, OnMaxMinInfo) 
ON_WM_SIZE()
ON_WM_SIZING()
ON_WM_GETMINMAXINFO()
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PACKET, &DlgMonitoring::OnItemchangedListPacket)
ON_NOTIFY(NM_CLICK, IDC_LIST_PACKET, &DlgMonitoring::OnClickListPacket)
ON_WM_CLOSE()
ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_PACKET, &DlgMonitoring::OnLvnDeleteitemListPacket)
END_MESSAGE_MAP()


// DlgMonitoring message handlers

/*
void DlgMonitoring::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	//  Get current pos of the child controls
	CRect rc1;
	CRect rc2;
	CRect rc3;

	GetDlgItem(IDC_LIST_PACKET)->GetWindowRect(rc1);
	GetDlgItem(IDC_TREE_PACKET_INFO)->GetWindowRect(rc2);
	GetDlgItem(IDC_EDIT_PACKET_RAW)->GetWindowRect(rc3);

	this->ScreenToClient(rc1);
	this->ScreenToClient(rc2);
	this->ScreenToClient(rc3);

	//  return the pos limit
	SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
	if (IDC_SPLITTER1 == pNMHDR->idFrom)
	{
		pNewMaxMinPos->lMin = rc1.top;
		pNewMaxMinPos->lMax = rc2.bottom;
	}
	else
	{
		pNewMaxMinPos->lMin = rc2.top;
		pNewMaxMinPos->lMax = rc3.bottom;
	}
}
*/

struct LIST_WIDTH {
	//비율 100%
	BYTE no = 5;
	BYTE time = 10;
	BYTE source = 17;
	BYTE destination = 17;
	BYTE protocol = 12;
	BYTE length = 12;
	BYTE info = 100 - (no + time + source + destination + protocol + length);
};

void DlgMonitoring::settingList()
{
	/*
	LONG style = ::GetWindowLong(m_cListCtrl.GetSafeHwnd(), GWL_STYLE);
	if (style & WS_HSCROLL) {
		style &= ~WS_HSCROLL;
		::SetWindowLong(m_cListCtrl.GetSafeHwnd(), GWL_STYLE, style);
	}
	*/
	
	LIST_WIDTH list_width;
	m_cListCtrl.ShowScrollBar(SB_HORZ, FALSE);

	CRect rect;
	m_cListCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	
	m_cListCtrl.GetClientRect(rect);
//	m_cListCtrl.GetWindowRect(rect);
	m_cListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	int width_size = rect.Width();
	int one_width_size = width_size / 100;

	m_cListCtrl.InsertColumn(0, L"No.", LVCFMT_LEFT, one_width_size*list_width.no);
	m_cListCtrl.InsertColumn(1, L"Time", LVCFMT_LEFT, one_width_size * list_width.time);
	m_cListCtrl.InsertColumn(2, L"Source", LVCFMT_LEFT, one_width_size * list_width.source);
	m_cListCtrl.InsertColumn(3, L"Destination", LVCFMT_LEFT, one_width_size * list_width.destination);
	m_cListCtrl.InsertColumn(4, L"Protocol", LVCFMT_LEFT, one_width_size * list_width.protocol);
	m_cListCtrl.InsertColumn(5, L"Length", LVCFMT_LEFT, one_width_size * list_width.length);
	m_cListCtrl.InsertColumn(6, L"Info", LVCFMT_LEFT, one_width_size * list_width.info);

//	Invalidate(TRUE);
}

int DlgMonitoring::gettimeofday(struct timeval* tv, struct timezone* tz)
{
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag;

	if (NULL != tv) {
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		tmpres -= DELTA_EPOCH_IN_MICROSECS;

		tmpres /= 10;

		tv->tv_sec = (tmpres / 1000000UL);
		tv->tv_usec = (tmpres % 1000000UL);
	}

	if (NULL != tz) {
		//현재 _timezone , _daylight 같은 경우가 _CRT_WARNING ~~ 문제가 생겨 후추 구현 예정
//        if (!tzflag)
//        {
//            _tzset();
//            tzflag++;
//        }
 //       tz->tz_minuteswest = _get_timezone() / 60;
 //       tz->tz_dsttime = _get_daylight();

	}
	return 0;
}

int DlgMonitoring::chronoExample(struct timeval* tv)
{
	tv->tv_usec= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	tv->tv_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return 0;
}

REAL_PACKET* getPacketQueueFront_ver2()
{
	REAL_PACKET* packet = NULL;
	std::mutex m;

	m.lock();
	while (packet == NULL) {
		if (theApp.pcap_kdy.FOR_UI.size() != 0) {
			if (theApp.pcap_kdy.FOR_UI.front()->check_ShowToUI == TRUE)
			{
				packet = theApp.pcap_kdy.FOR_UI.front();
				//packet->checkPacketReaded = TRUE;

				theApp.pcap_kdy.FOR_UI.pop();
			}
		}
		Sleep(10);
	}
	m.unlock();
	return packet;
}

RAW_PACKET* getPacketQueueFront()
{
	RAW_PACKET* packet = NULL;
	std::mutex m;

	m.lock();
	while (packet == NULL) {
		if (theApp.pcap_kdy.queue_packet.size() != 0) {
			if (theApp.pcap_kdy.queue_packet.front()->checkPacketReaded == FALSE)
			{
				packet = theApp.pcap_kdy.queue_packet.front();
				packet->checkPacketReaded = TRUE;
				theApp.pcap_kdy.queue_packet.pop();
			}
			/*
			packet = theApp.pcap_kdy.queue_packet.front();
			//			packet = theApp.pcap_kdy.queue_packet.back();
			
			theApp.pcap_kdy.queue_packet.pop();
			*/
		}
		Sleep(10);
	}
	m.unlock();
	return packet;
}



//2023.04.07
//실제 패킷의 데아터부만 가져와서 ui에 현시하는 것으로 변경되었다.
//그에 따른 ver2의 함수로서, 원본은 이 함수 아래에 전체 주석으로 되어있다.
UINT DlgMonitoring::updateEditControl(LPVOID pvParam)
{
	DlgMonitoring* this_class = (DlgMonitoring*)pvParam;
	CString str = L"";
	CString temp_str = L"";

	CString info_str = L"";

	//	std::string std_str = "";
	int count = 0;
	//	int value;
	struct timeval timeDiff;
	timeDiff.tv_sec = 0;
	timeDiff.tv_sec = 0;
	REAL_PACKET* packet = new REAL_PACKET;
	
	int pos = 0;
	BOOL check = FALSE;

	BOOL tcpCheck = FALSE;

	std::mutex m;

	// 0 -> none
	// 1 -> start or run
	// 2 -> stop or end
	// 3 -> pause

	/*
	1. dialog 시작 시에 0으로 되어 있는 것을 1로 변경.
	2. 그럼 이곳에서 0일때 무시되어 있던 게 1로 변경되고 해당 트리거에 의해 while 루프
	*/

	//2023.03.31 oid type map을 만든다.
	theApp.pcap_kdy.set_OidTypeMap(L"");

	//2023.02.14 TCP 세션 종료를 확인해야 한다.
	// 
	BOOL finAckCheck = FALSE;
	while (this_class->exitState != EXIT_STATE_STOP)
	{
		if (this_class->exitState == EXIT_STATE_PAUSE) {
			Sleep(10);
			continue;
		}


		

		packet = getPacketQueueFront_ver2();

		//추후 삭제해야함... 2023.04.17
		//Sleep(1000);
		//continue;

		PACKET_INFO info;
		try {
			if (packet->currentLength != 0) {
				info.caplen = packet->totalLength;
				info.pkt_data = (u_char*)calloc(1,info.caplen);
				memcpy(info.pkt_data, packet->realData, info.caplen);
				check = TRUE;
			}
		}
		catch (...)
		{
			check = FALSE;
		}
		if (check == TRUE) {

			if (!info_str.IsEmpty()) {
				info_str = L"";
			}
			tcpCheck = TRUE;
			theApp.pcap_kdy.postgreSQL_packet(packet, &info_str);
			CHeaderCtrl* pHeader = (CHeaderCtrl*)this_class->m_cListCtrl.GetDlgItem(0);
			//			int columnCount = pHeader->GetItemCount();
			int columnCount = 0;
			//no
			str.Format(L"%d", (this_class->m_cListCtrl.GetItemCount() + 1));
			int number = this_class->m_cListCtrl.InsertItem(this_class->m_cListCtrl.GetItemCount(), str);

			//time
			info.tv.tv_usec = 0;
			info.tv.tv_sec = 0;
			str.Format(L"%ld.%ld", info.tv.tv_sec, info.tv.tv_usec);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str.Left(7));

			//source
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, (packet->session.server_ip + L":" + packet->session.server_port));

			//destination
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, (packet->session.client_ip + L":" + packet->session.client_port));

			//protocol
			str = theApp.pcap_kdy.getProtocolText(IPV4_PROTOCOL);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str);

			//length
			str.Format(L"%d", packet->totalLength);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str);

			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, info_str);

			this_class->m_cListCtrl.SetItemData(number,
				(DWORD_PTR)packet);


			//TRACE(_T("IP = %x, Protocol = %d\n"), info.type, info.protocol);
		}
	}
	//	Sleep(10);
	return 0;
}
/*
UINT DlgMonitoring::updateEditControl(LPVOID pvParam)
{
	DlgMonitoring* this_class = (DlgMonitoring*)pvParam;
	CString str = L"";
	CString temp_str = L"";

	CString info_str = L"";

//	std::string std_str = "";
	int count = 0;
//	int value;
	struct timeval timeDiff;
	timeDiff.tv_sec = 0;
	timeDiff.tv_sec = 0;
	RAW_PACKET* packet = NULL;
	int pos = 0;
	BOOL check = FALSE;

	BOOL tcpCheck = FALSE;

	std::mutex m;

	// 0 -> none
	// 1 -> start or run
	// 2 -> stop or end
	// 3 -> pause

	
	//1. dialog 시작 시에 0으로 되어 있는 것을 1로 변경.
	//2. 그럼 이곳에서 0일때 무시되어 있던 게 1로 변경되고 해당 트리거에 의해 while 루프

	//2023.03.31 oid type map을 만든다.
	theApp.pcap_kdy.set_OidTypeMap(L"");
	
	//2023.02.14 TCP 세션 종료를 확인해야 한다.
	// 
	BOOL finAckCheck = FALSE;
	this_class->exitState = EXIT_STATE_PAUSE;//임시
	while (this_class->exitState != EXIT_STATE_STOP)
	{
		this_class->exitState = EXIT_STATE_PAUSE; //임시
		if (this_class->exitState == EXIT_STATE_PAUSE) {
			Sleep(10);
			continue;
		}
		packet = getPacketQueueFront();
		PACKET_INFO info;
		try {
			info.caplen = packet->pkt_len;
			info.pkt_data = (u_char*)malloc(info.caplen);
			memcpy(info.pkt_data, packet->pkt_data, info.caplen);
			check = TRUE;
		}
		catch (...)
		{
			check = FALSE;
		}
		if (check == TRUE) {

			if (!info_str.IsEmpty()) {
				info_str = L"";
			}

			tcpCheck = theApp.pcap_kdy.settingPacketInfo(packet->pkt_data, packet->pkt_len, &info);
			
			//2023.02.16 TEST
			//BOOL tempCheck = theApp.pcap_kdy.check_TCP_flag_FIN(info.protocol_data);
			//tempCheck = theApp.pcap_kdy.check_packet_data(info.startDataPos, info.endDataPos);
			
			//

			//2023.02.10. 서버와 관련있는 패킷인가?
			//2023.02.21. TCP Protocol 인가?
			if (theApp.pcap_kdy.check_sessionPacket(&info) && tcpCheck == TRUE &&
				theApp.pcap_kdy.check_usePacket(&info,packet->pkt_data)) {
				//이곳에 들어오면 서버와 통신하는 패킷이다.
				TRACE(L"Find!,"+ theApp.pcap_kdy.print_serverAndClientIpPort(info.sourceIP, info.sourcePort, info.destinationIP, info.destinationPort)+ L"\n");
				info_str = theApp.pcap_kdy.settingInfoString(info.protocol, info.protocol_data);
				theApp.pcap_kdy.postgreSQL_packet(&info, packet, &info_str);
				
				//2023.02.15 session을 찾고 해당 session의 시작, 도중, 종료의 과정을 체크하자.
			}
			else {
//				m.lock();
				
				//2023.02.13 void 포인터 형으로 동적 할당했기에 해제해야함.
				if (info.exist_protocol_data == TRUE && info.protocol_data != NULL)
					delete info.protocol_data;
				
				//2023.03.02 추가
				if (info.pkt_data != NULL)
					free(info.pkt_data);

				if(packet->pkt_data != NULL)
					delete packet->pkt_data;
				continue;
//				m.unlock();
			}

			if (this_class->m_cListCtrl.GetItemCount() == 0) {


				//first Packet
				this_class->chronoExample(&theApp.pcap_kdy.firstCapturePacketTime.tv);
				info.tv.tv_usec = 0;
				info.tv.tv_sec = 0;
				timeDiff.tv_sec = packet->tv.tv_sec;
				timeDiff.tv_usec = packet->tv.tv_usec;
			}
			else {
				//second packet ~~~
				info.tv.tv_usec = packet->tv.tv_usec - timeDiff.tv_usec;
				info.tv.tv_sec = packet->tv.tv_sec - timeDiff.tv_sec;

				if (info.tv.tv_usec < 0) {
					info.tv.tv_sec -= 1;
					info.tv.tv_usec += 1000000;
				}
			}
			CHeaderCtrl* pHeader = (CHeaderCtrl*)this_class->m_cListCtrl.GetDlgItem(0);
			//			int columnCount = pHeader->GetItemCount();
			int columnCount = 0;
			//no
			str.Format(L"%d", (this_class->m_cListCtrl.GetItemCount() + 1));
			int number = this_class->m_cListCtrl.InsertItem(this_class->m_cListCtrl.GetItemCount(), str);

			//time
			str.Format(L"%ld.%ld", info.tv.tv_sec, info.tv.tv_usec);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str.Left(7));

			//source
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, (info.sourceIP+L":"+info.sourcePort));

			//destination
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, (info.destinationIP + L":" + info.destinationPort));

			//protocol
			str = theApp.pcap_kdy.getProtocolText(info.protocol);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str);

			//length
			str.Format(L"%d", info.caplen);
			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, str);


			//info 부분에 postgreSQL 유저명, 데이터베이스명, 송신된 쿼리를 조회할 수 있도록 만든다.

			
			//if (info.protocol == 0x06) {
			//	UINT dataSize = info.caplen - info.protocolPos;
			//	u_char* temp = (u_char*)malloc(dataSize);
			//	u_char* data_packet = (u_char*)info.pkt_data;
			//	data_packet += info.startDataPos;
			//	memcpy(temp, data_packet, info.endDataPos - info.startDataPos);
			//	this_class->settingEditControl(data_packet, (info.endDataPos - info.startDataPos));
			//}
			

			//info
			//str.Format(L"INFO 추후에 만들어야 함");

			
//			str = L"INFO 추후에 만들어야 함";



			this_class->m_cListCtrl.SetItemText(number,
				++columnCount, info_str);

			this_class->m_cListCtrl.SetItemData(number,
				(DWORD_PTR)packet);


			TRACE(_T("IP = %x, Protocol = %d\n"), info.type, info.protocol);
		}
	}
//	Sleep(10);
	return 0;
}
*/


BOOL DlgMonitoring::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rc;
	CWnd* pWnd;

	settingList();

	pWnd = static_cast<CWnd*>(this);
	pWnd->GetWindowRect(rc);
//	pWnd->GetClientRect(rc);
	ScreenToClient(rc);

	m_DownDlgSplitterPane.Create(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | SS_HORIZ,
		this,
		GetDlgItem(IDC_TREE_PACKET_INFO),
		GetDlgItem(IDC_EDIT_PACKET_RAW),
		IDC_SPLITTER2,
		rc);

	m_MainDlgSplitterPane.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN | SS_HORIZ,
		this,		// the parent of the splitter pane
		GetDlgItem(IDC_LIST_PACKET),	// left pane
		&m_DownDlgSplitterPane,// right nested pane
		IDC_SPLITTER1,// this ID is used for saving/restoring splitter
					// position and therefore it must be unique !
		rc,	// dimensions of the splitter pane
		50,	// left constraint for splitter position
		20 // right constraint for splitter position
	);

//	HANDLE* pThread;
//	unsigned int dwThreadID;
	//HANDLE hThread = (HANDLE)_beginthreadex(NULL, NULL, updateEditControl, NULL, 0, &dwThreadID);
	
	this->exitState = 1;
	m_pThread = AfxBeginThread(updateEditControl, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DlgMonitoring::OnTvnSelchangedTreePacketInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DlgMonitoring::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (cx > 640 && cy > 320) {
		if (m_MainDlgSplitterPane.m_hWnd) {
			m_MainDlgSplitterPane.MoveWindow(0, 0, cx, cy);
		}
	}
}


void DlgMonitoring::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	LIST_WIDTH list_width;
	CRect rect;
	m_cListCtrl.GetClientRect(rect);
	int width_size = rect.Width();
	int one_width_size = width_size / 100;

	m_cListCtrl.SetColumnWidth(0, one_width_size * list_width.no);
	m_cListCtrl.SetColumnWidth(1, one_width_size * list_width.time);
	m_cListCtrl.SetColumnWidth(2, one_width_size * list_width.source);
	m_cListCtrl.SetColumnWidth(3, one_width_size * list_width.destination);
	m_cListCtrl.SetColumnWidth(4, one_width_size * list_width.protocol);
	m_cListCtrl.SetColumnWidth(5, one_width_size * list_width.length);
	m_cListCtrl.SetColumnWidth(6, one_width_size * list_width.info);
}



void DlgMonitoring::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 320;

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void DlgMonitoring::OnItemchangedListPacket(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LIST_PACKET);
	if (pListCtrl != NULL) {
		POSITION pos = pListCtrl->GetFirstSelectedItemPosition();

		if (pos == NULL)
			return;
		else {
			while (pos) {
				int nItem = pListCtrl->GetNextSelectedItem(pos);
			}
		}
	}	
	*pResult = 0;
}


void DlgMonitoring::settingEditControl(u_char* pktData,int pkt_len)
{
	//row 클릭 시..
	CString temp_str = L"";
	CString str = L"";
	//setting Edit
	for (int i = 0; i < pkt_len; i++) {
		temp_str.Format(L" %02x ", (unsigned int)*(pktData + i));
		str += temp_str;
		if ((i + 1) % 8 == 0)
			str += L" ";
		if ((i + 1) % 16 == 0 && i > 0) {
			str += L" \r\n";
		}
	}
	//Setting edit controL
	m_editPacketRaw.SetWindowTextW(L"");
	m_editPacketRaw.SetWindowTextW(str);
	//
}


void DlgMonitoring::OnClickListPacket(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;
	CListCtrl* pListCtrl;
	
	if (index < m_cListCtrl.GetItemCount() &&
		pNMItemActivate->hdr.hwndFrom == m_cListCtrl.GetSafeHwnd() &&
		index != -1) {
		pListCtrl = &m_cListCtrl;
		RAW_PACKET* rawPacket = (RAW_PACKET*)pListCtrl->GetItemData(index);
		m_editPacketRaw.SetWindowTextW(L"");
		settingEditControl(rawPacket->pkt_data, rawPacket->pkt_len);
	}
	*pResult = 0;
}

void DlgMonitoring::closeListPacket()
{

}

void DlgMonitoring::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	int answer = 0;
//	WCHAR szFilter[] = L"PCAP|*.pcap|PCAPNG|*.pcapng|All Files(*.*)|*.*||";
	WCHAR szFilter[] = L"PCAP|*.pcap|All Files(*.*)|*.*||";
	int pos = 0;
	CString file = L"";
	int listCount = m_cListCtrl.GetItemCount();
	if (listCount >= 0) {
		this->exitState = EXIT_STATE_PAUSE;
		m_pThread->SuspendThread();
		/*
		for (pcap_t* item : pcap_data) {
			pcap_breakloop(item);
		}
		*/
		if (listCount != 0)
			answer = AfxMessageBox(L"파일 저장?", MB_YESNOCANCEL);
		else
			answer = -1;
		if (IDYES == answer)
		{
			//save file | close dialog
			//HRESULT hr = BasicFileOpen_pcap(L"", FALSE);
			CFileDialog dlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST |
				OFN_OVERWRITEPROMPT, szFilter);
			
			switch (dlg.DoModal())
			{
			case IDOK:
				file = dlg.GetFileName();
				pos = file.ReverseFind(L'.');
				if (pos == -1)
				{
					file = dlg.GetPathName() + L".pcap";
					//theApp.pcap_kdy.savePcapFile(dlg.GetPathName() + L".pcap", &m_cListCtrl);
				}
				theApp.pcap_kdy.savePcapFile(file, &m_cListCtrl);
				break;
			case IDNO:
				answer = IDNO;
				break;
			}
		}
		if (IDNO == answer)
		{
			//none save file | close dialog

			//updateList exit signal
			this->exitState = EXIT_STATE_STOP;

			if (m_pThread != NULL)
			{
				m_pThread->SuspendThread();
				DWORD dwResult;
				::GetExitCodeThread(m_pThread->m_hThread, &dwResult);
				delete m_pThread;
			}
			TRACE(L"AfxThread exit success\n");

			if (m_cListCtrl.GetItemCount() > 0)
				closeListPacket();
			TRACE(L"list delete success\n");

			//theApp.pcap_kdy.stopRecvPacket();

			if (theApp.pcap_kdy.queue_packet.size() > 0) {
				theApp.pcap_kdy.stopRecvPacket();
				theApp.pcap_kdy.makeEmptyQueue(&theApp.pcap_kdy.queue_packet);
			}
			TRACE(L"queue delete success\n");

			//for(int i = 0;i< theApp.pcap_kdy.thread_PacketRecv.size();i++)
			theApp.pcap_kdy.thread_PacketRecv.clear();
		}
		else if(IDCANCEL == answer)
		{
			//resume capture
			m_pThread->ResumeThread();
			return;
		}
		if(answer == -1){
			//만약 수집된 패킷이 없더라도 recv_packet이 생성되었을 경우엔 꺼주어야 하니깐 넣는다.
			theApp.pcap_kdy.stopRecvPacket();
			theApp.pcap_kdy.thread_PacketRecv.clear();
		}
	}

	CDialogEx::OnClose();
}


void DlgMonitoring::OnLvnDeleteitemListPacket(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (pNMLV->iItem < 0)
		return;

	int k = 0;


	//2023.04.19 주석.
	/*
	RAW_PACKET* pRawPacket = (RAW_PACKET*)m_cListCtrl.GetItemData(pNMLV->iItem);
	if (pRawPacket) {
		if (pRawPacket->pkt_data != NULL) {
			try {
				TRACE(L"%x\n", pRawPacket->pkt_data);
				delete pRawPacket->pkt_data;
			}
			catch (std::exception& e) {
				TRACE(L"%s\n", e.what());
			}
		}
		delete pRawPacket;
	}
	*/
}

