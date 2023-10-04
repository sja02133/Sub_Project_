#include "pch.h"
#include "cWinPcapKDY.h"
#include "fileInputOutput.h"

#include <mutex>
#include "byteControl.h"	//2023.02.23 byte control을 위한 모음



//#include <mutex>
//#include <fstream>
extern std::vector< pcap_t*> pcap_data;
std::vector<struct_ipPort> vector_serverList;	//2023.02.10, fileInputOutput.h의 server 정보들이 담기는 vector을 쓰기 위해 선언

void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);
void packet_handler_for_pcap_file(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);

CString settingTcpInfoString(void* protocol_data);


//core 모드일 시에 이 함수 호출한다.
void cWinPcapKDY::main_start() {
	//std::thread(&cWinPcapKDY::check_stateOfProgram, this);
	coreMode = TRUE;
	std::vector<pcap_if_t> deviceVector;
	deviceVector = getNetworkDevice();

	//for (int i = 0; i < (int)vec_d.size(); i++) {
	//	nameStr.Format(L"%d", i + 1);
	//	m_ListCtrl.InsertItem(i, nameStr);
	//	nameStr = CString(vec_d.at(i).name);
	//	nameStr += L", ";
	//	descriptionStr = CString(vec_d.at(i).description);
	//	nameStr += descriptionStr;
	//	m_ListCtrl.SetItemText(i, 1, nameStr);
	//}


	int k = 0;

}


BOOL cWinPcapKDY::set_OidTypeMap(CString filePath) {
	std::wfstream io_file;
	io_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\pgsql_oid_type_ver_11.txt");
	if (oid_type_filePath.GetLength() == 0) {
		if (!check_existPostgresOidTypeVer11Txt(L"pgsql_oid_type_ver_11")) {
			TRACE(L"pgsql_oid_type_ver_11.txt file create or check failed...\n\n");
			return FALSE;
		}
	}
	//io_file.open(oid_type_filePath.GetBuffer());
	//oid_type_filePath.ReleaseBuffer();

	if (io_file.fail()) {
		TRACE(L"pgsql_oid_type_ver_11.txt file create failed...\n");
		return FALSE;
	}
	wchar_t line[256] = { 0, };
	BOOL findServerIpPortPos = FALSE;
	CString s = L"";
	UINT seekCount = 0;
	int exitCount = 0;
	CString oid_num = L"";
	CString typlen = L"";
	//while (io_file.getline(line, 256)) {
	while (io_file.getline(line, 1024)) {
		struct oid_struct oid_info;
		oid_num = L"";
		for (int i = 0, k = 0; i < 1024; i++) {
			if (line[i] == ' ') {
				k++;
			}
			else if (k == 0 && line[i] != ' ') {
				//oid
				oid_num += line[i];
				oid_info.oid += line[i];
			}
			else if (k == 0 && line[i] == ' ') {
				k++;
			}
			else if (k == 1 && line[i] != ' ') {
				//typcategory
				oid_info.typcategory += line[i];
			}
			else if (k == 1 && line[i] == ' ') {
				k++;
			}
			else if (k == 2 && line[i] != ' ') {
				oid_info.typarray += line[i];
			}
			else if (k == 3 && line[i] == ' ') {
				k++;
			}
			else if (k == 3 && line[i] != ' ') {
				typlen += line[i];
			}
			else if (k == 4) {
				if(line[i] == '/')
					i+=2;

				if (line[i] != NULL) {
					oid_info.name += line[i];
				}
				else {
					oid_info.typlen = _ttoi(typlen);
					typlen = "";
					oid_map.insert({ _ttoi(oid_info.oid), oid_info });
					i = 1024;
				}
			}
		}
	}
	io_file.close();

	return TRUE;
}

CString cWinPcapKDY::make_mapSessionKey(CString s_ip, CString s_port, CString c_ip, CString c_port)
{
	//Key 규칙
	// "Server IP"+":"+"Server Port"+"|"+"Client IP"+":"+"Client Port"
	CString str = L"";

	if (s_ip.IsEmpty() || s_port.IsEmpty() || c_ip.IsEmpty() || c_port.IsEmpty()) {
		TRACE(L"need to check Server, Client 's IP or Port\n");
		return str;
	}
	str = s_ip + L":" + s_port + L"|" + c_ip + L":" + c_port;
	return str;
}

CString cWinPcapKDY::make_mapSessionKey(struct_session session)
{
	//Key 규칙
	// "Server IP"+":"+"Server Port"+"|"+"Client IP"+":"+"Client Port"
	CString str = L"";
	if (session.server_ip.IsEmpty() || session.server_port.IsEmpty() ||
		session.client_ip.IsEmpty() || session.client_port.IsEmpty()) {
		//TRACE(L"need to check Server, Client 's IP or Port\n");
		return str;
	}
	str.AppendFormat(session.server_ip + L":" + session.server_port + L"|" + session.client_ip +
		L":" + session.client_port);
	return str;
}

struct_session cWinPcapKDY::get_mapSessionValue(CString key)
{	
	struct_session session;
	session.server_ip = L"";
	session.server_port = L"";
	session.client_ip = L"";
	session.client_port = L"";
	for (auto iter : map_sessionInfo) {
		if (iter.first == key)
			return iter.second;
	}
	TRACE(L"Can't find key - value\n");
	return session;
}

BOOL cWinPcapKDY::add_mapSessionValue(CString key, struct_session session)
{
	auto ret = map_sessionInfo.insert({ key,session });
	return TRUE;
}

BOOL cWinPcapKDY::check_mapSessionValue(CString key)
{
	if (map_sessionInfo.count(key))
		return TRUE;

	if (map_sessionInfo.find(key) != map_sessionInfo.end())
		return TRUE;

	return FALSE;
}

BOOL cWinPcapKDY::delete_mapSessionKeyValue(CString key)
{
	map_sessionInfo.erase(key);
	return TRUE;
}


int getAbit(u_char x, int n)
{
	return (x & (1 << n)) >> n;
}

int getAbit(unsigned short x, int n)
{
	return (x & (1 << n)) >> n;
}

int getAbit(unsigned short* x, int n)
{
	//8의 배수로
	//		  <-        <-
	// 0000 0000 0000 0000
	return (*x & (1 << n)) >> n;
}

UINT get_TCP_sessionState(CString infoString)
{
	if (infoString.Find(L"[SYN]"))
	{
		//find
		return 1;
	}
	else if (infoString.Find(L"[FIN, ACK]")) {
		return 3;
	}
	else if (infoString.Find(L"[RST, ACK]"))
	{
		return 3;
	}
	return 2;
}

BOOL cWinPcapKDY::set_sessionState(u_short protocol, void* protocol_data, CString ServerIp, CString ServerPort, CString ClientIp, CString ClientPort)
{
	//session의 상태(state)를 설정한다.
	// 규칙은 다음과 같다.
	// 0이면 return FALSE;
	// 1이면 SYN,ACK가 들어온 상태. 즉 시작.
	// 2이면 진행 중이라는 상태. return FALSE
	// 3이면 종료 준비 절차, 즉 RST혹은 FIN,ACK가 들어왔다.
	// 4는 session의 상태(state)가 3이면 4로 바꿔준다. 그런 뒤 해당 session은 vector에서 삭제한다.
	
	CString str = L"";
	UINT state = 0;

	switch (protocol) {
	case TCP_PROTOCOL:
		//tcp
		//str = settingTcpInfoString(protocol_data); //2023.04.14 주석처리

		break;
	case UDP_PROTOCOL:
		//udp

		break;
	}
	
	/*
	for (auto iter2 = vector_sessionInfo.begin(); iter2 != vector_sessionInfo.end(); ++iter2)
	{
		struct_session session = *iter2;
		if ((session.server_ip == ServerIp && session.server_port == ServerPort) &&
			(session.client_ip == ClientIp && session.client_port == ClientPort))
		{
			
			switch (protocol) {
			case TCP_PROTOCOL:
				//tcp
				str = settingTcpInfoString(protocol_data);
				state = get_TCP_sessionState(str);
				break;
			case UDP_PROTOCOL:
				//udp

				break;
			}



			switch (state) {
			case 1:
				iter2->state = 2;
				break;
			case 2:
				return FALSE;
				break;
			case 3:
				iter2->state = 4;
				break;
			case 4:
				//iter2 = vector_sessionInfo.erase(iter2);
				break;
			}
		}
		TRACE(L"can't find...");
	}
	*/
	return TRUE;
}

CString settingTcpInfoString(void* protocol_data)
{
	TCP_HEADER* tcpHeader = new TCP_HEADER;
	memcpy(tcpHeader, protocol_data, sizeof(TCP_HEADER));
	CString str = L"";
	int bitCheck = 0;
	for (int i = 8; i < 16; i++) {
		// i = 8 -> Fin
		// i = 9 -> Syn
		// i = 10 -> Reset
		// i = 11 -> push
		// i = 12 -> ack
		// i = 13 -> urgent
		// i = 14 -> ENC-Echo
		// i = 15 -> Congestion Window Reduced
		// i = 0 -> Accurate ECN
		// i = 1 ~ 3 -> Reserved
		// i = 4 ~ 7 -> Header length
		bitCheck = getAbit(tcpHeader->flag, i);
		CString tempStr = L"";
		if(bitCheck == 1)
		switch (i) {
		case 8:
		{
			//Fin
			tempStr = L"FIN";
		}
			break;
		case 9:
		{
			//Syn
			tempStr = L"SYN";
		}
		break;
		case 10:
		{
			//Reset
			tempStr = L"RESET";
		}
		break;
		case 11:
		{
			//push
			tempStr = L"PUSH";
		}
		break;
		case 12:
		{
			//ack
			tempStr = L"ACK";
		}
		break;
		case 13:
		{
			//urgent
			tempStr = L"URGENT";
		}
		break;
		case 14:
		{
			//ENC-Echo
			tempStr = L"ENC-ECHO";
		}
		break;
		case 15:
		{
			//Congestion Window Reduced
			tempStr = L"CONGESTION WINDOW REDUCED";
		}
		break;
		default:
			break;
		}

		if (!tempStr.IsEmpty()) {
			if (!str.IsEmpty())
				str += L", ";
			str += tempStr;
		}
	}
	
	str = L"[" + str;
	str = str + L"]";
	
	delete tcpHeader;

	return str;
}

CString cWinPcapKDY::settingInfoString(u_short protocol, void* protocol_data)
{
	CString str;

	switch (protocol) {
	case TCP_PROTOCOL:
		//tcp
		
		// 2023.04.16 주석
		//str = settingTcpInfoString(protocol_data);
		break;
	case UDP_PROTOCOL:
		//udp

		break;
	}

	//2023.02.22 PostgreSQL 관련 메시지 출력을 위한..



	return str;
}

BOOL loadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len) {
		fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0) {
		fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
		return FALSE;
	}
	return TRUE;
}

std::vector<pcap_if_t> cWinPcapKDY::npcap_getNetworkDevice()
{
	std::vector<pcap_if_t> vec_d;
	pcap_if_t* alldevs;
	pcap_if_t* d;
	pcap_if_t temp;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (!loadNpcapDlls()) {
		TRACE(L"Couldn't load Npcap\n");
		exit(1);
	}

	PTSTR AdapterName = 0;

	ULONG AdapterLength;
	AdapterLength = sizeof(AdapterName);
	//if (PacketGetAdapterNames(AdapterName, &AdapterLength)) {

	//}
	//pcap_t *pcap = pcap_fopen_offline(tshark_out, errbuf);

	/*
	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		TRACE(L"Error in pcap_findalldevs : %s\n", errbuf);
		exit(1);
	}

	for (d = alldevs; d; d = d->next) {
		temp.name = d->name;
		temp.description = d->description;
		temp.flags = d->flags;
		temp.addresses = d->addresses;
		temp.next = d->next;
		vec_d.push_back(temp);
	}
	*/
	return vec_d;
}

std::vector<pcap_if_t> cWinPcapKDY::getNetworkDevice() {
	std::vector<pcap_if_t> vec_d;
	pcap_if_t* alldevs;
	pcap_if_t* d;
	pcap_if_t temp;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		TRACE(L"Error in pcap_findalldevs : %s\n", errbuf);
		exit(1);
	}

	for (d = alldevs; d; d = d->next) {
		temp.name = d->name;
		temp.description = d->description;
		temp.flags = d->flags;
		temp.addresses = d->addresses;
		temp.next = d->next;
		vec_d.push_back(temp);
	}
	return vec_d;
}

CString cWinPcapKDY::settingFilter(CString* filter)
{
	//filter rule
	//host ~~~ 


	CString returnFilterString = L"";
	int type = 0;

	filter->MakeLower();

	type = checkProtocolType(filter);

	switch (type)
	{
	case TCP_PROTOCOL:
		//tcp
		break;
	case UDP_PROTOCOL:
		//udp
		break;
	}

	return returnFilterString;
}

int cWinPcapKDY::checkProtocolType(CString* filter)
{
	int type = 0;

	if (filter->Find(L"tcp"))
		return TCP_PROTOCOL;
	else if (filter->Find(L"udp"))
		return UDP_PROTOCOL;
	else
		return -1;
}

//void get_packet(CString deviceName, CString filter, void* pcap_kdy);
//void get_packet(CString deviceName,struct bpf_program fcode) {
void cWinPcapKDY::get_packet(CString deviceName,CString filter) {
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	int length = deviceName.GetLength();
//	char* st = new char[length];
	if ((adhandle = pcap_open_live((LPSTR)deviceName.GetBuffer(0), 65536, 1, 22, errbuf)) == NULL) {
		return;
	}
	
	if (!filter.IsEmpty()) {
		struct bpf_program fcode;

		//테스트 용 추후 삭제필요
		//filter = L"host 192.168.137.138 and port 5432";

		if (pcap_compile(adhandle, &fcode, (CStringA)filter, 1, NULL) < 0) {
			TRACE(L"%s\n",(CString)pcap_geterr(adhandle));
			return;
		}
		if (pcap_setfilter(adhandle, &fcode) < 0) {
			TRACE(L"%s\n", (CString)pcap_geterr(adhandle));
			return;
		}
	}
	
	


	pcap_data.push_back(adhandle);
	int k = pcap_loop(adhandle, 0, packet_handler, (u_char*)this);
	if (k == -2)
		TRACE(L"pcap_loop is end because pcap_breakloop is called, pcap_loop return value is %d\n", k);
	else if (k == -1)
		TRACE(L"pcap_loop happen error, error : %s\n", pcap_geterr(adhandle));
	pcap_data.clear();
}

BOOL cWinPcapKDY::check_stateOfProgram() {
	while (true) {
		if (checkRecvPacket == RECV_CHECK_STOP) {
			for (auto iter = thread_PacketRecv.begin(); iter != thread_PacketRecv.end(); ++iter) {
				SuspendThread((*iter).native_handle());
			}
			
			if (type_list_map.size() > 0) {
				TRACE(L"here");
			}

			if (realPacketMap.size() > 0) {
				TRACE(L"here");
			}

			if (FOR_UI.size() > 0){
				TRACE(L"here");
			}

			if (queue_packet.size() > 0) {
				TRACE(L"here");
			}

			if (multi_map.size()) {
				TRACE(L"here");
			}
		}
		Sleep(100);
	}
}

void cWinPcapKDY::gettingInfo(std::vector<CString> deviceName,CString filter)
{
	pcap_if_t* alldevs;
	pcap_if_t* d;

	pcap_t* adhandle;
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	CString str = L"";
//	std::thread thread_PacketRecv; //2023.01.10 -> 타 함수에서 쓰레드 종료를 위하여 헤더파일에 선언.
	//여기서부터 멀티스레드로 만들어야 한다.
	
	std::thread t;
	
	if (checkRecvPacket == RECV_CHECK_END)
		checkRecvPacket = RECV_CHECK_START;

	//check queue is empty state when before of setting thread
	if (queue_packet.size() != 0)
	{
		TRACE(L"queue size is not 0\n");
		makeEmptyQueue(&queue_packet);
	}
	
	for (int i = 0; i < deviceName.size(); i++) {
//		t = std::thread(&cWinPcapKDY::get_packet,this, deviceName.at(i), filter);
		if (i == 0 && thread_PacketRecv.size() != 0) {
			TRACE(L"thread is not 0?\n");
			for (int j = 0; j < thread_PacketRecv.size(); j++) {
				SuspendThread(thread_PacketRecv.at(i).native_handle());
			}
		}
		//2023.04.19 추가...
		thread_PacketRecv.push_back(std::thread(&cWinPcapKDY::get_packet, this, deviceName.at(i), filter));
		thread_PacketRecv.at(i).detach();
	}
	TRACE(L"thread detach complete\n");
}

struct packet_data {
	int no;
	long sec;
	long usec;
	CString source;
	CString destination;
	CString protocol;
	int length;
	CString info;
};

//callback
/* 패킷이 캡처 됬을때, 호출되는 콜백 함수 */
// 인자1 : 파라미터로 넘겨받은 값
// 인자2 : 패킷 정보
// 인자3 : 실제 캡처된 패킷 데이터
// 캡처한 패킷에 대한 모든 일은 이 함수 에서 
int count = 0;



//int packet_seek = 0;

/*
BOOL settingEtherFrame(const u_char* pkt_data, ether_header* ether_header)
{

	u_char* pos_pkt = (u_char*)pkt_data;
	packet_seek += 14;
	memcpy(ether_header, pkt_data, 14);
//	memmove(pos_pkt, pkt_data, packet_seek);
	pos_pkt += 14;
	
	switch (ntohs(ether_header->type))
	{
	case IPV4_PROTOCOL:
		
		break;
	case ARP_PROTOCOL:

		break;
	case IPV6_PROTOCOL:

		break;
	default:

		break;
	}

	ip_header temp_ip_header;
	settingIpHeader(pkt_data, &temp_ip_header);

	memcpy(&ether_header->FCS, pos_pkt, sizeof(ether_header->FCS));
	packet_seek += sizeof(ether_header->FCS);
	return TRUE;
}
*/

//CString cWinPcapKDY::getProtocolText(u_char protocol)
CString cWinPcapKDY::getProtocolText(u_short protocol)
{
	CString str = L"";
	switch (protocol) {
	case HOPOPT_PROTOCOL:
		str.Format(L"HOPOPT");
		break;
	case ICMP_PROTOCOL:
		str.Format(L"ICMP");
		break;
	case IGMP_PROTOCOL:
		str.Format(L"IGMP");
		break;
	case GGP_PROTOCOL:
		str.Format(L"GGP");
		break;
	case IPV4_PROTOCOL:
		str.Format(L"IPV4");
		break;
	case ST_PROTOCOL:
		str.Format(L"ST");
		break;
	case TCP_PROTOCOL:
		str.Format(L"TCP");
		break;
	case CBT_PROTOCOL:
		str.Format(L"CBT");
		break;
	case EGP_PROTOCOL:
		str.Format(L"EGP");
		break;
	case IGP_PROTOCOL:
		str.Format(L"IGP");
		break;
	case BBN__RCC__MON_PROTOCOL:
		str.Format(L"BBN-RCC-MON");
		break;
	case NVP__II_PROTOCOL:
		str.Format(L"NVP-II");
		break;
	case PUP_PROTOCOL:
		str.Format(L"PUP");
		break;
	case ARGUS_PROTOCOL:
		str.Format(L"ARGUS");
		break;
	case EMCON_PROTOCOL:
		str.Format(L"EMCON");
		break;
	case XNET_PROTOCOL:
		str.Format(L"XNET");
		break;
	case CHAOS_PROTOCOL:
		str.Format(L"CHAOS");
		break;
	case UDP_PROTOCOL:
		str.Format(L"UDP");
		break;
	case MUX_PROTOCOL:
		str.Format(L"MUX");
		break;
	case DCN__MEAS_PROTOCOL:
		str.Format(L"DCN-MEAS");
		break;
	case HMP_PROTOCOL:
		str.Format(L"HMP");
		break;
	case PRM_PROTOCOL:
		str.Format(L"PRM");
		break;
	case XNS__IDP_PROTOCOL:
		str.Format(L"XNS-IDP");
		break;
	default:
		str.Format(L"");
		break;
	}
	return str;
}

UINT shortToDec(u_short a)
{
	std::stringstream sstream;
//	std::wstringstream cstream;
	sstream << std::hex << a;
	std::string str = sstream.str();
	CString cstr = L"";
	cstr = str.c_str();
	CString temp1 = L"", temp2 = L"";
	temp1 = cstr.Mid(0, 2);
	temp2 = cstr.Mid(2, 3);
	cstr = temp2 + temp1;
	int hexCount = 3;
	int totalValue = 0;
	for (int i = 0; i < cstr.GetLength(); i++) {
		int num = 0;
		switch (cstr.GetAt(i)) {
		case 'A':
			num = 10;
			break;
		case 'B':
			num = 11;
			break;
		case 'C':
			num = 12;
			break;
		case 'D':
			num = 13;
			break;
		case 'E':
			num = 14;
			break;
		case 'F':
			num = 15;
			break;
		default: {
			const wchar_t* p = new wchar_t(cstr.GetAt(i));
			num = _wtoi(p);
			delete p;
			break;
		}
		}
		hexCount != 0 ? num = (std::pow(16, hexCount)) * num : num = num * 1;
		hexCount--;
		totalValue += num;
	}
	return totalValue;
}

BOOL cWinPcapKDY::check_TCP_flag_FIN(void* protocol_data)
{
	TCP_FLAG flag;
	checkTcpFlag((u_char*)protocol_data, flag);
	return flag.FIN;

	/*
	CString str = L"";

	str = settingTcpInfoString(protocol_data);
	int value = 0;
	value = str.Find(L"FIN");
	value = str.Find(L"fin");

	if (str.Find(L"FIN") >= 0 || str.Find(L"fin") >= 0)
	{
		return TRUE;
	}
	return FALSE;
	*/
}

BOOL cWinPcapKDY::check_packet_data(int startDataPos, int endDataPos)
{
	if (startDataPos < endDataPos)
		return TRUE;
	else
		return FALSE;
}

BOOL set_packetSeekOfType(int& seek, u_short type)
{
	switch (type)
	{
	case VLAN_TYPE:
		seek += 18;
		break;
	default:
		seek += 14;
		break;
	}

	if(seek == 0)
		return FALSE;
	else
		return TRUE;
}



//2023.02.21 업데이트
// IPV4, ARP, IPV6 한정 + 무조건 ether header는 14byte 연산 -> VLAN 추가하여 각 type마다 연산
// -> TCP 환경만 볼것이고 나머지는 return FALSE한다.
BOOL cWinPcapKDY::settingPacketInfo(const u_char* pkt_data,int pkt_length, PACKET_INFO* info)
{
	TRACE(L"begin settingPacketInfo()\n");

	BOOL recvPacketCheck = TRUE;	//우리는 현재 IPV4,TCP 패킷만 수집하여 분석할 것 이다.

//	packet_seek = 0;
	u_char* pos_pkt = (u_char*)pkt_data;
	int packetTotalLength = info->caplen;
	int pos = 0;
	u_char* tempPkt = (u_char*)calloc(1, info->caplen);
	try {
		//std::copy(&pkt_data[0], &pkt_data[info->caplen], tempPkt);
		memcpy(tempPkt, &pkt_data[pos], info->caplen);
	}
	catch (std::exception& e) {
		TRACE(L"%s\n", e.what());
	}
	int headerTotalLength = 0;	//2023.02.21 IP header length + Protocol header length

	//setting ether header
	ETHER_HEADER* etherHeader = new ETHER_HEADER;
	memcpy(etherHeader, &tempPkt[pos], sizeof(ETHER_HEADER));
	info->etherHeaderPos = 0;
	info->type = ntohs(etherHeader->type);
	delete etherHeader;

	set_packetSeekOfType(pos, info->type);
	//set_packetSeekOfType(packet_seek, info->type);
	info->ipHeaderPos = pos;

	if (info->type == IPV4_TYPE) {
		IPV4_HEADER* ipV4Header = new IPV4_HEADER;
		settingIpv4Header(&tempPkt[0], ipV4Header, pos);
		info->protocol = ipV4Header->proto;
		info->sourceIP.Format(L"%d.%d.%d.%d", ipV4Header->saddr.byte1,
			ipV4Header->saddr.byte2, ipV4Header->saddr.byte3, ipV4Header->saddr.byte4);
		info->destinationIP.Format(L"%d.%d.%d.%d", ipV4Header->daddr.byte1,
			ipV4Header->daddr.byte2, ipV4Header->daddr.byte3, ipV4Header->daddr.byte4);
		//2023.02.22 IP length + Protocol length = info->ipTotalLength
		//info->ipTotalLength = ipV4Header->tlen;
		info->ipTotalLength = ntohs(ipV4Header->tlen);
		if (ipV4Header->op_pad == TRUE) {
			free(ipV4Header->option);
		}
		else
			delete ipV4Header;
	}
	/*
	else if (info->type == ARP_TYPE) {
		ARP_HEADER arpHeader;
		settingArpHeader(pkt_data, &arpHeader, pos);
		info->protocol = arpHeader.protoType;
		info->sourceIP.Format(L"%d.%d.%d.%d", arpHeader.saddr.byte1,
			arpHeader.saddr.byte2, arpHeader.saddr.byte3, arpHeader.saddr.byte4);
		info->destinationIP.Format(L"%d.%d.%d.%d", arpHeader.taddr.byte1,
			arpHeader.taddr.byte2, arpHeader.taddr.byte3, arpHeader.taddr.byte4);
	}
	else if (info->type == IPV6_TYPE) {
		IPV6_HEADER ipV6Header;
		settingIpv6Header(pkt_data, &ipV6Header, pos);
		info->protocol = ipV6Header.nextHeader;
		info->sourceIP.Format(L"%X:%X:%X:%X:%X:%X",
			ipV6Header.saddr.byte1, ipV6Header.saddr.byte2,
			ipV6Header.saddr.byte3, ipV6Header.saddr.byte4,
			ipV6Header.saddr.byte5, ipV6Header.saddr.byte6);
		info->destinationIP.Format(L"%X:%X:%X:%X:%X:%X",
			ipV6Header.daddr.byte1, ipV6Header.daddr.byte2,
			ipV6Header.daddr.byte3, ipV6Header.daddr.byte4,
			ipV6Header.daddr.byte5, ipV6Header.daddr.byte6);
	}
	*/
	else {
		recvPacketCheck = FALSE;
	}
	info->protocolPos = pos;

	if (info->type != NULL) {
		if (info->protocol == TCP_PROTOCOL) {
			TCP_HEADER* tcpHeader = new TCP_HEADER;
			settingTcpPacket(pkt_data, tcpHeader, pos);
			info->sourcePort.Format(L"%d", ntohs(tcpHeader->sport));
			info->destinationPort.Format(L"%d", ntohs(tcpHeader->dport));
			info->protocol_data = tcpHeader;	//2023.02.13 TCP 데이터를 DlgMonitoring.cpp -> updateEditControl()에서 SYN, ACK 등을 구분하기 위하여..
			info->exist_protocol_data = TRUE;
			//2023.04.11 
		}
		/*
		else if(info->protocol == UDP_PROTOCOL){
			UDP_HEADER udpHeader;
			settingUdpPacket(pkt_data, &udpHeader, pos);
		}
		else if (info->protocol == HOPOPT_PROTOCOL) {
			//IPv6의 프로토콜
		}
		*/
		else {
			//현재 해석 X
			recvPacketCheck = FALSE;
		}
	}

	if (recvPacketCheck == FALSE) {
		free(tempPkt);

		if (info->caplen > 0)
			free(info->pkt_data);
		
		if (info->exist_protocol_data == TRUE)
			free(info->protocol_data);

		return FALSE;
	}

	info->startDataPos = pos;		//data 시작

	//2023.02.22 Data 영역이 padding인지 데이터인지 구분하기 위하여
	// 2023.04.11 수정,
	//2023.02.22
	//ipTotalLength - (ip header length + protocol header length) != 0
	// -> data
	//ipTotalLength - (ip header length + protocol header length) == 0
	// -> padding
	//2023.04.11
	//PACKET_INFO 구조체에 PACKET_HEADER_LENGTH 구조체 추가.
	info->headerLength.ether = info->ipHeaderPos;
	info->headerLength.ip = info->protocolPos - info->ipHeaderPos;
	info->headerLength.protocol = info->startDataPos - info->protocolPos;

	info->dataLength = info->ipTotalLength - (info->headerLength.ip + info->headerLength.protocol);

	//trailer => {if data length is 0} 0 > capture length - (ether header length + ip header length + protocol header length)
	//padding => 0 > capture length - (ether header length + ip header length + protocol header length + data length)
	//if (info->caplen > info->dataLength) {
	//	//padding
	//	info->padding_size = info->caplen - info->dataLength;
	//}
	if (info->caplen > (info->ipTotalLength + info->headerLength.ether)) {
		int length = info->caplen - (info->ipTotalLength + info->headerLength.ether);
		if (length == 2) {
			//trailer
			info->trailer_size = length;
		}
		else {
			//padding
			info->padding_size = length;
		}
	}

	info->endDataPos = info->startDataPos + info->dataLength;
	return TRUE;
}



BOOL cWinPcapKDY::set_structSession(PACKET_INFO* info, struct_session* session)
{
	for (auto iter = vector_serverList.begin(); iter != vector_serverList.end(); ++iter)
	{
		struct_ipPort ipPort = *iter;
		//2023.02.16 현재 문제, 지금은 destination 즉, 수신부에서만 server ip, port 경우에만 추가한다.
		// 반대로 중간에 캡쳐하는 과정이라 치면, source 즉, 송신부가 서버가 될수 있으므로 교차 검증해야한다.
		//struct_session session;
		if (ipPort.ip == info->destinationIP && ipPort.s_port == info->destinationPort)
		{
			//서버가 des 수신 부..
			session->server_ip = ipPort.ip;
			session->server_port = ipPort.s_port;
			session->client_ip = info->sourceIP;
			session->client_port = info->sourcePort;
		}
		else if (ipPort.ip == info->sourceIP && ipPort.s_port == info->sourcePort)
		{
			//서버가 source 송신 부..
			session->server_ip = info->sourceIP;
			session->server_port = info->sourcePort;
			session->client_ip = info->destinationIP;
			session->client_port = info->destinationPort;
		}
	}
	if (!session->server_ip.IsEmpty() && !session->server_port.IsEmpty() &&
		!session->client_ip.IsEmpty() && !session->client_port.IsEmpty())
		return TRUE;
	else
		return FALSE;
}

// session 정보가 기입되어 있는 vector(server_file)의 데이터들과 
// 대조하여 특정 서버 IP:PORT와 대조되는 자료인지 판단하여
// session 정보가 기입되어 있는 vector(vector_sessionInfo)
// 에 추가한 뒤 TRUE를 반환한다. 없으면 FALSE
//BOOL cWinPcapKDY::check_sessionPacket(PACKET_INFO* info)
BOOL cWinPcapKDY::check_sessionPacket(PACKET_INFO* info_data)
{	
	// return 값 : true => 해당 패킷을 보겠다. | false => 해당 패킷은 스킵한다.
	
	// 2023.02.16 추가 문제, 만약 FIN일때 session을 제거한다고 했을때 그 다음에 오는 FIN, ACK 혹은 RST등의 부분은 읽고 map에 추가한다.
	// 이 문제는 해결하기 위해서는 FIN을 만났을때 이후의 부분도 안보는것으로 처리해야하는데.

	PACKET_INFO* info = copy_PACKET_INFO_data(info_data);

	BOOL check = TRUE;
	BOOL sessionInfo_check = TRUE;

	for (auto iter = vector_serverList.begin(); iter != vector_serverList.end(); ++iter)
	{
		struct_ipPort ipPort = *iter;
		//2023.02.16 현재 문제, 지금은 destination 즉, 수신부에서만 server ip, port 경우에만 추가한다.
		// 반대로 중간에 캡쳐하는 과정이라 치면, source 즉, 송신부가 서버가 될수 있으므로 교차 검증해야한다.
		struct_session session;
		if (ipPort.ip == info->destinationIP && ipPort.s_port == info->destinationPort)
		{
			//서버가 des 수신 부..
			session.server_ip = ipPort.ip;
			session.server_port = ipPort.s_port;
			session.client_ip = info->sourceIP;
			session.client_port = info->sourcePort;
		}
		else if (ipPort.ip == info->sourceIP && ipPort.s_port == info->sourcePort)
		{
			//서버가 source 송신 부..
			session.server_ip = info->sourceIP;
			session.server_port = info->sourcePort;
			session.client_ip = info->destinationIP;
			session.client_port = info->destinationPort;
		}
		else
			return FALSE;

		//key에 해당하는 value가 없다.
		check = check_mapSessionValue(make_mapSessionKey(session));

		//struct_session session; 의 ack, seq 넘버를 입력한다.
		int pos = 0;
		UINT protocolLen = info->startDataPos - info->protocolPos;
		u_char* pkt_protocol = (u_char*)malloc(protocolLen);
		memcpy(pkt_protocol, info->protocol_data, protocolLen);
		//pos += protocolLen;
		pos += 4;	//pass to source, destination port position..
		//session.seqNum = pkt_protocol[pos];
		memcpy(&session.seqNum, &pkt_protocol[pos], sizeof(UINT));
		//session.seqNum = get_byteToInt(&pkt_protocol[pos]);
		pos += 4;
		memcpy(&session.ackNum, &pkt_protocol[pos], sizeof(UINT));
		//session.ackNum = get_byteToInt(&pkt_protocol[pos]);
		struct TCP_FLAG flag_info;
		checkTcpFlag((u_char*)info->protocol_data, flag_info);

		free(pkt_protocol);

		if (check == FALSE)
		{
			//none exist in map
			//data가 존재하면 세션에 추가해야함.
			if (check_packet_data(info->startDataPos, info->endDataPos))
			{
				if (add_mapSessionValue(make_mapSessionKey(session), session))
				{
					std::map<CString, struct_session>::iterator iter;
					for (iter = map_sessionInfo.begin(); iter != map_sessionInfo.end(); ++iter) {
						if ((*iter).first == make_mapSessionKey(session)) {
							(*iter).second.session_queue.push(info);
						}
					}
				}
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else
		{
			//exist in map
			TRACE(L"exist in map..\n");
			//flag가 FIN이 있다면 버려야 하는 값 혹은 세션에서 지워줘야 함..

			sessionInfo_check = check_TCP_flag_FIN(info->protocol_data);
			if (sessionInfo_check == TRUE) {
				//FIN flag가 왔다..
				delete_mapSessionKeyValue(make_mapSessionKey(session));
				TRACE(L"delete session.. info : " + print_serverAndClientIpPort(session.server_ip,
					session.server_port, session.client_ip, session.client_port));
				return FALSE;
			}
			else {
				std::map<CString, struct_session>::iterator iter;
				
				for (iter = map_sessionInfo.begin(); iter != map_sessionInfo.end();++iter) {
					if ((*iter).first == make_mapSessionKey(session)) {
						(*iter).second.session_queue.push(info);
					}
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

PACKET_INFO* cWinPcapKDY::copy_PACKET_INFO_data(PACKET_INFO* info)
{
	PACKET_INFO* copy_info = new PACKET_INFO;

	//non memory alloc part copy..
	copy_info->tv = info->tv;
	copy_info->headerLength = info->headerLength;
	copy_info->dataLength = info->dataLength;
	copy_info->startDataPos = info->startDataPos;
	copy_info->endDataPos = info->endDataPos;
	copy_info->etherHeaderPos = info->etherHeaderPos;
	copy_info->ipHeaderPos = info->ipHeaderPos;
	copy_info->protocolPos = info->protocolPos;
	copy_info->caplen = info->caplen;
	copy_info->protocol = info->protocol;
	copy_info->type = info->type;
	copy_info->sourceIP = info->sourceIP;
	copy_info->destinationIP = info->destinationIP;
	copy_info->info_string = info->info_string;
	copy_info->sourcePort = info->sourcePort;
	copy_info->destinationPort = info->destinationPort;
	copy_info->exist_protocol_data = info->exist_protocol_data;
	copy_info->ipTotalLength = info->ipTotalLength;
	copy_info->trailer_size = info->trailer_size;
	copy_info->padding_size = info->padding_size;

	//memory alloc part copy..
	if (copy_info->caplen > 0) {
		copy_info->pkt_data = (u_char*)malloc(copy_info->caplen);
		memcpy(copy_info->pkt_data, info->pkt_data,copy_info->caplen);
	}

	if (copy_info->exist_protocol_data == TRUE) {
		copy_info->protocol_data = (u_char*)malloc(copy_info->startDataPos - copy_info->protocolPos);
		memcpy(copy_info->protocol_data, &info->pkt_data[info->protocolPos], copy_info->startDataPos - copy_info->protocolPos);
	}

	return copy_info;
}

BOOL cWinPcapKDY::checkTcpFlag(u_char* tcp,struct TCP_FLAG& flag)
{
	int pos = 0;
	pos += 2;	//pass to source port
	pos += 2;	//pass to destination port
	pos += 4;	//pass to sequence number
	pos += 4;	//pass to acknowledge number

	u_short flag_value = 0;
	memcpy(&flag_value, &tcp[pos], sizeof(u_short));
	if (flag_value == NULL)
		return FALSE;

	endswap(&flag_value);
	
	flag.URG = flag_value & (1 << 5) ? TRUE : FALSE;
	flag.ACK = flag_value & (1 << 4) ? TRUE : FALSE;
	flag.PSH = flag_value & (1 << 3) ? TRUE : FALSE;
	flag.RST = flag_value & (1 << 2) ? TRUE : FALSE;
	flag.SYN = flag_value & (1 << 1) ? TRUE : FALSE;
	flag.FIN = flag_value & (1 << 0) ? TRUE : FALSE;

	return TRUE;
}

CString cWinPcapKDY::print_serverAndClientIpPort(CString s_ip, CString s_port, CString c_ip, CString c_port)
{
	CString str = L"S_IP("
		+ s_ip
		+ L"), S_PORT("
		+ s_port
		+ L") | C_IP("
		+ c_ip
		+ L"), C_PORT("
		+ c_port
		+ L")";
	return str;
}

//2023.02.03 추가
// TCP 패킷을 ether, ip, data 부로 나누어 trace로 보기 위한 함수
void cWinPcapKDY::trace_tcp_packet(PACKET_INFO* info, const u_char* pkt_data)
{
	//u_char* temp_packet = (u_char*)malloc(info->caplen);
	//memcpy(temp_packet, pkt_data, info->caplen);
	CString temp_str = L"", str = L"";
	//ETHER
	/*
	temp_str += "ETEHR : ";
	for (int i = 0; i < info->ipHeaderPos; i++,temp_packet++) {
		//temp_str.Format(L" %02x ", (unsigned int)*(pkt_data + i));
		str.Format(L" %02x ", (unsigned int)*(pkt_data + i));
		temp_str += str;
	}
	temp_str += " || ";

	//IP
	temp_str += "IP : ";
	for (int i = info->ipHeaderPos; i < info->startDataPos; i++) {
		str.Format(L" %02x ", (unsigned int)*(pkt_data + i));
		temp_str += str;
	}
	temp_str += " || ";
	*/
	//data
	temp_str += "DATA : ";

#if 0
	for (int i = info->startDataPos; i < info->caplen; i++) {
//		str.Format(L" %02x ", (unsigned int)*(pkt_data + i));
		str.Format(L"%c", (unsigned char)*(pkt_data + i));
		if (((unsigned char)*(pkt_data + i)) == 0) {
			int k = 0;
		}
		temp_str += str;
	}
#endif

	typedef struct AAA {
		CString name, value;
	} AAA;

	std::vector<AAA> vec;
	
	AAA temp;
	int begin = info->startDataPos + 8;
	if (pkt_data[info->startDataPos] == 0) {
		for (int i = info->startDataPos + 8, j = 0; i < info->caplen; i++) {

			if (pkt_data[i] == 0)
			{
				if (j % 2 == 0)
					temp.name = (char*)&pkt_data[begin];
				else {
					temp.value = (char*)&pkt_data[begin];
					vec.push_back(temp);
				}
				j++;
				begin = i + 1;
			}
		}
	}

	temp_str += " || ";
//	TRACE(temp_str + L"\n");
}

BOOL cWinPcapKDY::set_startUpMessage(CString* info_str, u_char* data,int dataSize)
{
	// length + protocol major version + protocol minor version + parameter name + parameter value + parameter name + .....
	// 4		2                        2                        가변 + 00         가변 + 00
	int totalLength = 0;
	short majorVer = 0, minorVer = 0;
	CString parameterName = L"", parameterValue = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);
	int pos = 0;

	memcpy(tempPkt, data, dataSize);

	pos++;

	totalLength = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	if (totalLength <= 0) {
		free(tempPkt);
		return FALSE;
	}

	majorVer = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);

	minorVer = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);

	//checkLast에서 마지막 1을 한번더 빼는 이유는 마지막이 00으로 끝나기 때문이다.
	//int size = (sizeof(int) + sizeof(short) + sizeof(short));
	int checkLast = (totalLength - pos - 1);

	//로그인 실패
	// -> FIN,ACK 가 들어와야 처리
	//TIMEZONE, SERVTER_ENCODING , SERVER_VERSION

	BOOL checkParameterTurn = TRUE;		//TRUE => name, FALSE => value
	CString tempStr = L"";
	for (int i = 0; i < checkLast + 1; i++)
	{
		if (i == 106)
			int abdbdb = 0;
		if (tempPkt[pos + i] == NULL) {
			int a = 0;
		}
		if (tempPkt[pos+i] == 0)
		{
			switch (checkParameterTurn)
			{
			case 1:
//				*info_str += L" Parameter Name = ";
				tempStr += parameterName;
				tempStr += L" = ";
				checkParameterTurn = FALSE;
				break;
			case 0:
//				tempStr += L" Parameter Value = ";
				tempStr += parameterValue;
				if ((i < checkLast))
					tempStr += L", ";

				checkParameterTurn = TRUE;
				break;
			}
		}
		else {
			if (checkParameterTurn == TRUE) {
				//name
				if (!parameterValue.IsEmpty())
					parameterValue = L"";
				parameterName += tempPkt[pos+i];
			}
			else {
				//value
				if (!parameterName.IsEmpty())
					parameterName = L"";
				parameterValue += tempPkt[pos+i];
			}
		}
	}
	free(tempPkt);
	*info_str = tempStr;
	return TRUE;
}

BOOL cWinPcapKDY::set_SimpleQueryMessage(CString* info_str, u_char* data, int dataSize)
{
	// type + length + Query
	// 1	  4        가변 + 00
	
	// + length는 length 4byte + Query XX byte이다.

	int totalLength = 0;
	CString parameterName = L"", parameterValue = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);

	int pos = 0;
	memcpy(tempPkt, data, dataSize);

	pos++;
	totalLength = get_byteToInt(&tempPkt[pos]);

	pos += sizeof(int);

	//2023.03.03 현재 MultiPacket 처리가 안되어 있어서 임시로 만든다.
	//checkLast에서 마지막 1을 한번더 빼는 이유는 마지막이 00으로 끝나기 때문이다.
	int size = (sizeof(u_char) + sizeof(int));
	int checkLast = (totalLength - size - 1);
	for (int i = 0; i < checkLast; i++)
	{
		if (pos + i > dataSize)
			return TRUE;

		if (tempPkt[pos+i] == 0xa0)
			*info_str += L"\n";
		else
			*info_str += tempPkt[pos+i];
	}
	free(tempPkt);
	return TRUE;
}


#include <afxconv.h>


BOOL cWinPcapKDY::set_ErrorMessage(CString* info_str, u_char* data, int dataSize)
{
	
	// Name		byte
	// Type		1
	// Length	4
	
	// -------------
	// 제일 앞의 오는 char 1 byte 값(※대문자)에 따라서 Name이 구분된다.
	// S => Severity
	// V => Text
	// C => Code
	// M => Message
	// F => File
	// L => Line
	// R => Routine
	// 끝에 00으로 끝을 구분한다.

	

	int totalLength = 0;
	CString parameterName = L"", parameterValue = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);
	int pos = 0;

	memcpy(tempPkt, data, dataSize);

	pos++;	//Pass to type 1 byte

	totalLength = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	

	//checkLast에서 마지막 1을 한번더 빼는 이유는 마지막이 00으로 끝나기 때문이다.
	int checkLast = (totalLength - (pos - sizeof(int)) - 1);

	//Error code 검출 및 수정
	BOOL checkErrorCode = FALSE;

	for (int i = pos,count = 0; i < checkLast; i++)
	{
		if (i != pos) {
			*info_str += L", ";
		}
		if (tempPkt[i] == 'S')
		{
			//Severity
			*info_str += L"Severity = ";
		}
		else if (tempPkt[i] == 'V')
		{
			*info_str += L"Text = ";
		}
		else if (tempPkt[i] == 'C')
		{
			*info_str += L"Code = ";
			checkErrorCode = TRUE;
		}
		else if (tempPkt[i] == 'M')
		{
			*info_str += L"Message = ";
		}
		else if (tempPkt[i] == 'F')
		{
			*info_str += L"File = ";
		}
		else if (tempPkt[i] == 'L')
		{
			*info_str += L"Line = ";
		}
		else if (tempPkt[i] == 'R')
		{
			*info_str += L"Routine = ";
		}
		else if (tempPkt[i] == 'P')
		{
			*info_str += L"Position = ";
		}
		else
		{
			*info_str += L"??? = ";
		}
		i++;	//Pass to First character
		int checkCount = 0;
		while (tempPkt[i+checkCount] != 0)
		{
			checkCount++;
			/*
			CString temp_String = L"";
			USES_CONVERSION;
			if (tempPkt[i] > 128) {
				u_char tempChar[2] = { 0, };
				tempChar[0] = *&tempPkt[i];
				tempChar[1] = *&tempPkt[i+1];

				temp_String += A2W((char*)tempChar);
				temp_String = temp_String.Left(1);
				i += 2;
				*info_str += temp_String;
			}
			else {
				*info_str += tempPkt[i];
				i++;
			}
			if (i > checkLast)
				break;
				*/
		}
		int tempPktPos = i;
		CString temp_String = L"";
		u_char ansiChar[2] = { 0, };
		char* tempAnsiChar = 0;

		ULONG checkCharSet = CharSet((char*)&tempPkt[i], checkCount);
		switch (checkCharSet)
		{
		case STRING_ENG:
		{
			while (tempPktPos < (i + checkCount)) {
				*info_str += tempPkt[tempPktPos];
				tempPktPos++;
			}
		}
			break;
		case STRING_ANSI:
		{
			USES_CONVERSION;
			while(tempPktPos < (i+checkCount)){
				if (tempPkt[tempPktPos] < 128) {
					*info_str += tempPkt[tempPktPos];
					tempPktPos++;
				}
				else {
					ansiChar[0] = *&tempPkt[tempPktPos];
					ansiChar[1] = *&tempPkt[tempPktPos + 1];
					temp_String = A2W((char*)ansiChar);
					temp_String = temp_String.Left(1);
					tempPktPos += 2;
					*info_str += temp_String;
				}
			}
		}
			break;
		case STRING_UTF8:
		{
			UTF8ToANSI((char*)&tempPkt[tempPktPos], info_str, &tempPktPos);
			/*
			if (tempPkt[tempPktPos] < 128) {
				*info_str += tempPkt[tempPktPos];
				tempPktPos++;
			}
			else {
				//tempAnsiChar = UTF8ToANSI((char*)&tempPkt[tempPktPos]);
				UTF8ToANSI((char*)&tempPkt[tempPktPos],info_str,&tempPktPos);
			}
			*/
			/*
			USES_CONVERSION;
			while (tempPktPos < (i + checkCount)) {
				utf8Char[0] = *&tempPkt[tempPktPos];
				utf8Char[1] = *&tempPkt[tempPktPos + 1];
				utf8Char[2] = *&tempPkt[tempPktPos + 2];
				temp_String += A2W((char*)utf8Char);
				temp_String = temp_String.Left(1);
				tempPktPos += 3;
				*info_str += temp_String;
			}
			*/
		}
			break;
		default:
			return FALSE;
			break;
		}
		i += checkCount;

		if (checkErrorCode == TRUE) {
			//Error code 처리
			CString errorCode = info_str->Right(5);
			set_errorCodeForInfoString(&errorCode, info_str);
			checkErrorCode = FALSE;
		}
	}
	free(tempPkt);
	return TRUE;
}

BOOL cWinPcapKDY::set_errorCodeForInfoString(CString* errorCode, CString* info_str)
{
	CString message = L"";
	if (errorCode->Compare(L"00000") == 0) {
		//Class 00 - Successful Completion
		message = L"Success";
	}
	else {
		CString checkClass = L"",checkException = L"";
		checkClass = errorCode->Left(2);
//		checkException = errorCode->Right(3);
		if (checkClass.Compare(L"01") == 0)
			message = L"Warning";
		else if (checkClass.Compare(L"02") == 0)
			message = L"No Data";
		else if (checkClass.Compare(L"03") == 0)
			message = L"SQL Statement Not Yet Complete";
		else if (checkClass.Compare(L"08") == 0)
			message = L"Connection Exception";
		else if (checkClass.Compare(L"09") == 0)
			message = L"Triggered Action Exception";
		else if (checkClass.Compare(L"0A") == 0)
			message = L"Feature Not Supported";
		else if (checkClass.Compare(L"0B") == 0)
			message = L"Invalid Transaction Initiation";
		else if (checkClass.Compare(L"0F") == 0)
			message = L"Locator Exception";
		else if (checkClass.Compare(L"0L") == 0)
			message = L"Invalid Grantor";
		else if (checkClass.Compare(L"0P") == 0)
			message = L"Invalid Role Specification";
		else if (checkClass.Compare(L"0Z") == 0)
			message = L"Diagnostics Exception";
		else if (checkClass.Compare(L"20") == 0)
			message = L"Case Not Found";
		else if (checkClass.Compare(L"21") == 0)
			message = L"Cardinality Violation";
		else if (checkClass.Compare(L"22") == 0)
			message = L"Data Exception";
		else if (checkClass.Compare(L"23") == 0)
			message = L"Integrity Constraint Violation";
		else if (checkClass.Compare(L"24") == 0)
			message = L"Invalid Cursor State";
		else if (checkClass.Compare(L"25") == 0)
			message = L"Invalid Transaction State";
		else if (checkClass.Compare(L"26"))
			message = L"Invalid SQL Statement Name";
		else if (checkClass.Compare(L"27") == 0)
			message = L"Triggered Data Change Violation";
		else if (checkClass.Compare(L"28") == 0)
			message = L"Invalid Authorization Specification";
		else if (checkClass.Compare(L"2B") == 0)
			message = L"Dependent Privilege Descriptors Still Exist";
		else if (checkClass.Compare(L"2D") == 0)
			message = L"Invalid Transaction Termination";
		else if (checkClass.Compare(L"2F") == 0)
			message = L"SQL Routine Exception";
		else if (checkClass.Compare(L"34") == 0)
			message = L"Invalid Cursor Name";
		else if (checkClass.Compare(L"38") == 0)
			message = L"External Routine Exception";
		else if (checkClass.Compare(L"39") == 0)
			message = L"External Routine Invocation Exception";
		else if (checkClass.Compare(L"3B") == 0)
			message = L"Savepoint Exception";
		else if (checkClass.Compare(L"3D") == 0)
			message = L"Invalid Catalog Name";
		else if (checkClass.Compare(L"3F") == 0)
			message = L"Invalid Schema Name";
		else if (checkClass.Compare(L"40") == 0)
			message = L"Transaction Rollback";
		else if (checkClass.Compare(L"42") == 0)
			message = L"Syntax Error or Access Rule Violation";
		else if (checkClass.Compare(L"44") == 0)
			message = L"WITH CHECK OPTION Violation";
		else if (checkClass.Compare(L"53") == 0)
			message = L"Insufficient Resources";
		else if (checkClass.Compare(L"53") == 0)
			message = L"Program Limit Exceeded";
		else if (checkClass.Compare(L"55") == 0)
			message = L"Object Not In Prerequisite State";
		else if (checkClass.Compare(L"57") == 0)
			message = L"Operator Intervention";
		else if (checkClass.Compare(L"58") == 0)
			message = L"System Error";
		else if (checkClass.Compare(L"F0") == 0)
			message = L"Configuration File Error";
		else if (checkClass.Compare(L"HV") == 0)
			message = L"Foreign Data Wrapper Error";
		else if (checkClass.Compare(L"P0") == 0)
			message = L"PL/pgSQL Error";
		else if (checkClass.Compare(L"XX") == 0)
			message = L"Internal Error";
		else
			message = L"ERROR";
	}

	message.Format(L"(" + message + L")");

	if (info_str->Find(L"(ERROR)") == -1) {
		return FALSE;
	}
	else
		info_str->Replace(L"(ERROR)", message);

	return TRUE;
}


BOOL cWinPcapKDY::set_AuthenticationRequestPassword(PACKET_INFO* info, CString* info_str, int type)
{
	switch (type) {
	case 5:
		//md5
		*info_str += L"MD5";
		break;
	default:
		*info_str += L"???";
		break;
	}
	return TRUE;
}

BOOL cWinPcapKDY::set_AuthenticationRequestLoginSuccess(PACKET_INFO* info, u_char* tempPkt, int dataSize, RAW_PACKET* packet)
{
	//2023.02.27 현재 PACKET_INFO* info에 리스트를 만들어서 관리하나 이렇게 하면 안되며
	// RAW_PACKET 즉, queue 영역에 리스트를 저장하여야 한다. 

	packet->authenticationRequestStatus = TRUE;

	// 무조건 Type(1) + Length(4) + Data(가변) + 00 + Data(가변) + 00으로 이루어져 있다.
	
	int length_int = 0;
	int pos = info->startDataPos;

	for (int i = 0; pos < dataSize;) {
		u_char* type = (u_char*)malloc(sizeof(u_char));
		memcpy(type, &tempPkt[pos+i], 1);
		pos += 1;
		packet->authenticationRequestDataList.push_back(type);
		//type에 따른 처리는 다음에.
		u_char* length = (u_char*)malloc(sizeof(int));
		memcpy(length, &tempPkt[pos+i], 4);
		pos += 4;
		packet->authenticationRequestDataList.push_back(length);
		length_int = get_byteToInt(length);
		u_char* data = (u_char*)malloc(length_int);
		memcpy(data, &tempPkt[pos+i], length_int - (sizeof(int)));
		packet->authenticationRequestDataList.push_back(data);
		pos += length_int - (sizeof(int));
//		i += length_int - 1;
		/*
		if (i == dataSize) {
			tempPkt -= dataSize;
		}
		*/
	}
	return TRUE;
}

BOOL cWinPcapKDY::set_AuthenticationRequest(CString* info_str, u_char* data, int dataSize)
{
	// Name					byte
	// Type					1
	// Length				4
	// Authentication type	4
	// Sault value			(md5일때 4byte).. 다른 경우엔 가변? 2023.02.27

	// 2023.02.27 추가+
	// Authentication request는 type에 따라, PDU가 나누어 진다.

	int totalLength = 0, type = 0,pos = 0;
	CString saltValue = L"",tempStr = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);

	memcpy(tempPkt, data, dataSize);

	pos++;	//Pass to type 1 byte

	totalLength = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	type = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	*info_str += L"Type : ";
	switch (type)
	{
	case 0:
		//login success..
		*info_str += L"Success(0)";
		//set_AuthenticationRequestLoginSuccess();
		break;
	case 5:
		*info_str += L"MD5 password(5) ";
		//set_AuthenticationRequestPassword(info, info_str, type);
		for (int i = pos; i <= totalLength; i++) {
			tempStr.Format(L" %02X", tempPkt[i]);
			saltValue += tempStr;
		}
		*info_str += L", ";
		*info_str += L"saltValue : " + saltValue;
		break;
	default:

		break;
	}
	free(tempPkt);
	return TRUE;
}

BOOL cWinPcapKDY::set_PasswordMessage(CString* info_str, u_char* data, int dataSize)
{
	// Name					byte
	// Type					1
	// Length				4
	// Password				가변

	int totalLength = 0;
	CString password = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);
	int pos = 0;

	memcpy(tempPkt, data, dataSize);

	pos++;	//Pass to type 1 byte

	totalLength = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	//int checkLast = (totalLength - pos);

	*info_str += L"Password : ";
	for (int i = pos; i < totalLength; i++) {
		password += tempPkt[i];
	}
	*info_str += password;
	free(tempPkt);

	return TRUE;
}

BOOL cWinPcapKDY::check_usePacket(PACKET_INFO* info,u_char* pkt_data)
{
	//사용할 첫 바이트의 값의 모음을 저장한다.
	u_char usedByte[] = { 0, 'Q', 'E', 'R', 'X','P' };
	int size = sizeof(usedByte);
	
	u_char* temp_pkt = (u_char*)malloc(info->caplen);
	memcpy(temp_pkt, pkt_data, info->caplen);
	int pos = 0;
	pos += info->startDataPos;

	u_char firstByte = temp_pkt[pos];
	BOOL check = FALSE;
	for (int i = 0;i < size; i++)
	{
		if (firstByte == usedByte[i]) {
			check = TRUE;
		}
	}
	free(temp_pkt);
	if (check == TRUE)
		return TRUE;
	else
		return FALSE;
}


std::vector<CString> cWinPcapKDY::parse_bind_params(u_char* message,int* before_pos) {
	std::vector<CString> params;

	int pos = 0;

	pos += 2;	//Pass to 2 byte(Portal, Statement)

	// 'B' 타입 메시지 파싱
	u_short param_count = get_byteToShort(&message[pos]); // 파라미터 개수 추출
	pos += sizeof(u_short);

	u_short* format = new u_short(param_count);
	int* values = new int(param_count);

	for (u_short i = 0; i < param_count; i++){
		format[i] = get_byteToShort(&message[pos]);
		pos += sizeof(u_short);
	}
	int col_length = 0;
	for (u_short i = 0; i < param_count; i++) {
		col_length = get_byteToInt(&message[pos]);
		pos += sizeof(int);
		values[i] = get_byteToInt(&message[pos]);
		
		pos += sizeof(int);
	}

	*before_pos += pos;

	return params;
}

BOOL cWinPcapKDY::set_parse(CString* info_str, u_char* tempPkt, int* Pkt_pos,u_char **oid_list)
{
	//TRUE => parameters의 값이 1 이상
	//FALSE => parameters의 값이 0

	//parse 영역을 처리한다.
	// Name					byte
	// P : Parse
	// Type					1
	// Length				4
	// statement(Type)		가변 + 00
	// Query				가변(Length만큼) + 00
	// Parameters			2
	// Type OID				4 (위의 Parameters의 값만큼 갯수가 존재함)

	int length = 0;
	int pos = *Pkt_pos;
	
	CString statement = L"";
	CString query = L"";
	int parameterCount = 0;

	pos++;		//pass to type
	
	length = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);


	int i = 0;
	for (; tempPkt[pos] != '\0'; pos++) {
		statement += tempPkt[pos + i];
	}

	pos++;	//pass to 00

	for (; tempPkt[pos] != '\0'; pos++) {
		query += tempPkt[pos + i];
	}

	if (query.Find(L'$') >= 0) {
		int ddd = 0;
	}

	pos++; //pass to 00

	parameterCount = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);

	if (parameterCount > 0) {
		int size = sizeof(int) * parameterCount;
		*oid_list = (u_char*)malloc(size);
		memcpy(*oid_list, &tempPkt[pos], size);
		pos += size;
	}

	*info_str += query;

	*Pkt_pos += (pos - *Pkt_pos);
	return TRUE;
}

int cWinPcapKDY::set_columnLengthOfParameter(int th, u_char* oid_list)
{
	switch (oid_list[th]) {
	case 16: // bool
		return 1;
	case 17: // bytea
	case 114: // json
	case 3802: // jsonb
		return -1;
	case 18: // char
	case 19: // name
	case 25: // text
	case 1042: // bpchar
	case 1043: // varchar
	case 2249: // xml
		return 4;
	case 20: // int8
	case 21: // int2
	case 23: // int4
	case 26: // oid
	case 702: // abstime
	case 703: // reltime
	case 704: // tinterval
	case 718: // circle
	case 719: // money
	case 790: // money_oid
	case 829: // macaddr
	case 869: // inet
	case 1041: // oidvector
	case 1082: // date
	case 1083: // time
	case 1114: // timestamp
	case 1184: // timestamptz
	case 1266: // timetz
	case 1700: // numeric
	case 1790: // refcursor
	case 2202: // regproc
	case 2203: // regprocedure
	case 2204: // regoper
	case 2205: // regoperator
	case 2206: // regclass
	case 2207: // regtype
	case 2950: // uuid
	case 3220: // pg_lsn
	case 3807: // jsonpath
		return 8;
	case 700: // float4
		return 4;
	case 701: // float8
	case 7901: // box2d
	case 7910: // circle_array
	case 7911: // macaddr_array
	case 7912: // inet_array
		return 8;
	default:
		return -2;
	}
}

CString cWinPcapKDY::get_bind_value_binary(CString* info_str, u_char* data, int dataLength, u_char** typeOid, int th)
{
	info_str->Format(*info_str + L" $%d : ", th + 1);
	CString str = L"";
	CString temp = L"";
	int i = 0, j = 0, pos = 0;
	int type = get_byteToInt(*typeOid);

	for (int i = 0; i < dataLength; i++) {
		str.AppendFormat(L" %02x", data[i]);
	}
	return str;

	int zzz = 0;

	//int type = ntohl(*reinterpret_cast<int*>(typeOid));
	switch (type) {
	case BOOLOID:
	{
		str = data ? "TRUE" : "FALSE";
	}
	break;
	case BYTEAOID:
		str = "\\x";
		for (int i = 0; i < dataLength; i++)
		{
			temp.Format(L"%02x", (unsigned char)data[i]);
			str += temp;
		}
		break;
	case CHAROID:
		str = data;
		break;
	case NAMEOID:
	{
		for (i = 0; i < dataLength; i++)
			str += data[i];
		break;
	}
	case INT8OID:
	{
		str.Format(_T("%I64d"), *((__int64*)ntohll(*reinterpret_cast<long long*>(data))));
		break;
	}
	case INT2OID:
	{
		str.Format(L"%hd", ntohs(*(short*)data));
		break;
	}
	case INT2VECTOROID:
	{
		temp = "ARRAY[";
		for (int i = 0; i < (dataLength / sizeof(short)); i++)
		{
			if (i > 0)
			{
				temp += ",";
			}
			str += temp;
			temp.Format(L"%hd", ntohs(*(short*)data[pos]));
			str += temp;
			pos += sizeof(short);
		}
		str += "]";
		break;
	}
	case INT4OID:
	{
		str.Format(L"%d", *(int*)data);
		break;
	}
	case REGPROCOID:
	{
		if (dataLength == 4) {
			str.Format(_T("%I64d"), *((__int64*)ntohll(*reinterpret_cast<long long*>(data))));
		}
		else if (dataLength == 8) {
			str.Format(L"%d", *(int*)data);
		}
		else
			str = L"NULL";
		break;
	}
	case TEXTOID:
	{
		for (i = 0; i < dataLength; i++) {
			str += *data;
		}
		break;
	}
	case OIDOID:
	{
		if (dataLength == 4) {
			str.Format(_T("%I64d"), *((__int64*)ntohll(*reinterpret_cast<long long*>(data))));
		}
		else if (dataLength == 8) {
			str.Format(L"%d", *(int*)data);
		}
		else
			str = L"Unsupported OIDOID(26) type";
		break;
	}
	case TIDOID:
	{
		if (dataLength == 6) {
			str.AppendFormat(L"(%u,%u)", (UINT*)data[0], (UINT*)data[1]);
			str.AppendFormat(L"[%u]", (USHORT)data[4]);
		}
		else
			str = L"Unsupported TIDOID(27) type";
		break;
	}
	case XIDOID:
	{
		str.Format(_T("%d"), *((UINT*)ntohl(*reinterpret_cast<UINT*>(data))));
		break;
	}
	case CIDOID:
	{
		str.Format(_T("%d"), (UINT*)data);
		break;
	}
	case OIDVECTOROID:
	{
		str += "ARRAY[";
		for (int i = 0; i < *(int*)data[pos]; i++)
		{
			if (i == 0) {
				pos += 4;
			}

			CString value;
			switch (*(unsigned int*)data[pos])
			{
				pos += 4;
				// handle other types as needed
			case INT2OID:
			{
				str.AppendFormat(_T("%d"), (short*)data[pos]);
				pos += sizeof(short);
				break;
			}
			case INT4OID:
			{
				str.AppendFormat(_T("%d"), (int*)data[pos]);
				pos += sizeof(int);
				break;
			}
			case TEXTOID:
			{
				for (j = pos; j < dataLength; j++) {
					str += data[j];
				}
				break;
			}
			default:
			{
				str = "unknown";
				break;
			}
			}
			if (i < count - 1)
				str += L",";
		}
		str += L"]";
		break;
	}
	}
	return str;
}


CString cWinPcapKDY::get_value_Of_OID_NumericTypes(struct oid_struct oid_info, u_char* data,int dataLength) {
	CString str = L"";

	if (oid_info.typlen == 2) {
		//int2
		str.AppendFormat(L"%hd", *(endswap_t((short*)data)));
	}
	else if (oid_info.typlen == 4) {
		if (oid_info.name.Find(L"INT") == -1)
			str.AppendFormat(L"%f", *(endswap_t((float*)data)));
		else
			str.AppendFormat(L"%d", *(endswap_t((int*)data)));
	}
	else if (oid_info.typlen == 8) {
		if (oid_info.name.Find(L"INT") == -1)
			str.AppendFormat(L"%lf", *(endswap_t((double*)data)));
		else
			str.AppendFormat(L"%lld", *(endswap_t((long long*)data)));
	}
	else if (oid_info.typlen == -1)
		str = setCharacterSet(data, dataLength);
	else
		TRACE(L"typlen is wrong\n");

	return str;
}

CString cWinPcapKDY::get_value_Of_OID_BooleanType(struct oid_struct oid_info,u_char* data)
{
	CString str = L"";
	str = data ? "TRUE" : "FALSE";
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_StringType(struct oid_struct oid_info, u_char* data,int dataLength)
{
	CString str = L"";
	str = setCharacterSet(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_DateTimeType(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str = setCharacterSet(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_UserDefinedType(struct oid_struct oid_info, u_char* data,int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_PseudoTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_ArrayTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	//Internal Storage Format(ISF)
	//모든 array가 그런지는 모르겠다.
	CString str = L"(ARRAY)";

	int pos = 0;
	
	pos += 4;	//???
	pos += 4;	//padding
	
	int whatIsOid = 0;
	memcpy(&whatIsOid, &data[pos], sizeof(int));
	endswap(&whatIsOid);
	pos += sizeof(int);

	oid_struct temp_struct = (*oid_map.find((UINT)whatIsOid)).second;
	
	int dataCount = 0;
	memcpy(&dataCount, &data[pos], sizeof(int));
	endswap(&dataCount);
	pos += sizeof(int);

	pos += 4; //pass to column pos

	//실제 데이터 시작
	int checkCount = 0;
	u_char* tempChar = 0;
	int dataSize = 0;
	for (int i = pos; checkCount < dataCount; i++) {
		if(i == pos)
			str.AppendFormat(L"[");
		memcpy(&dataSize, &data[pos], sizeof(int));
		endswap(&dataSize);
		pos += sizeof(int);
		tempChar = (u_char*)malloc(dataSize);
		memcpy(tempChar, &data[pos], dataSize);
		u_char* copy = tempChar;
		str += get_value_Of_OID_findAndDestroy(temp_struct, tempChar, dataSize);
		free(tempChar);
		pos += dataSize;
		checkCount++;
		if (checkCount != dataCount)
			str.AppendFormat(L",");
		else
			str.AppendFormat(L"]");
	}
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_BitStringTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_GeometricTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	//str += get_hexStringAndUtf8String(data, dataLength);
	str += setCharacterSet(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_NetworkAddressTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::getget_value_Of_OID_CompositeTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_RangeTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	//here
	CString str = L"";
	str += setCharacterSet(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_TimespanTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	//here
	CString str = L"";
	str += setCharacterSet(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_UnknownTypes(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	str += get_hexStringAndUtf8String(data, dataLength);
	return str;
}

CString cWinPcapKDY::get_hexStringAndUtf8String(u_char* data, int dataLength)
{
	CString str = L"";
	str = "\\x";
	for (int i = 0; i < dataLength; i++)
	{
		str.AppendFormat(L" %02x", (unsigned char)data[i]);
	}
	CString utf = setCharacterSet(data, dataLength);
	str += L" (" + utf + L")";
	return str;
}

CString cWinPcapKDY::get_value_Of_OID_findAndDestroy(struct oid_struct oid_info, u_char* data, int dataLength)
{
	CString str = L"";
	CString typcategory = oid_info.typcategory;

	if (typcategory == 'A')
		str = get_value_Of_OID_ArrayTypes(oid_info, data,dataLength);
	else if (typcategory == 'B')
		str = get_value_Of_OID_BooleanType(oid_info, data);
	else if (typcategory == 'C')
		str = getget_value_Of_OID_CompositeTypes(oid_info, data, dataLength);
	else if (typcategory == 'D')
		str = get_value_Of_OID_DateTimeType(oid_info,data,dataLength);
	else if (typcategory == 'G')
		str = get_value_Of_OID_GeometricTypes(oid_info, data, dataLength);
	else if (typcategory == 'I')
		str = get_value_Of_OID_NetworkAddressTypes(oid_info, data, dataLength);
	else if (typcategory == 'N')
		str = get_value_Of_OID_NumericTypes(oid_info,data,dataLength);
	else if (typcategory == 'P')
		str = get_value_Of_OID_PseudoTypes(oid_info, data, dataLength);
	else if (typcategory == 'R')
		str = get_value_Of_OID_RangeTypes(oid_info, data, dataLength);
	else if (typcategory == 'S')
		str = get_value_Of_OID_StringType(oid_info,data,dataLength);
	else if (typcategory == 'T')
		str = get_value_Of_OID_TimespanTypes(oid_info, data, dataLength);
	else if (typcategory == 'U')
		str = get_value_Of_OID_UserDefinedType(oid_info, data, dataLength);
	else if (typcategory == 'V')
		str = get_value_Of_OID_BitStringTypes(oid_info, data, dataLength);
	else if (typcategory == 'X')
		str = get_value_Of_OID_UnknownTypes(oid_info,data,dataLength);
	else
		str = L"";
	return str;
}

CString cWinPcapKDY::get_bind_value(CString* info_str, u_char* data, int dataLength, u_char** typeOid, int th)
{
	info_str->Format(*info_str + L" $%d", th + 1);
	CString str = L"";
	int type = 0;
	memcpy(&type, *typeOid, sizeof(UINT));
	//2023.03.31 oid_map에서 찾는다.
	//std::map<UINT,oid_struct>::iterator iter = oid_map.find(type);
	if (oid_map.find(type) == oid_map.end()) {
		return str;
	}
	oid_struct temp_struct = (*oid_map.find(type)).second;

	info_str->Append(L"(" + temp_struct.name + L")");
	info_str->AppendFormat(L" : ");
	str = get_value_Of_OID_findAndDestroy(temp_struct, data, dataLength);
	return str;
}

BOOL cWinPcapKDY::set_bind(CString* info_str, u_char* data,int dataSize,std::list<UINT> oid_list)
{
	// Name					byte
	// B : Bind
	// Type					1
	// Length				4
	// Porter				가변 + 00
	// Statement			가변 + 00
	// Parameter Format		2 
	// Format				2 (위의 Parameters의 값만큼 갯수가 존재함)
	// Parameter values		2
	// Column Length		4 (위의 Parameter values의 값만큼 갯수가 존재함) |
	// Data					4 (위의 Parameter values의 값만큼 갯수가 존재함) | Column Length 와 Data는 2개에 한세트
	// Result formats		2

	u_char* tempPkt = (u_char*)malloc(dataSize);
	memcpy(tempPkt, data, dataSize);

	int length = 0;
	int pos = 0;

	CString portal = L"";
	CString statement = L"";
	USHORT parameterFormatCount = 0;
	USHORT parameterValueCount = 0;

	u_char type;
	memcpy(&type, &tempPkt[pos], sizeof(u_char));
	pos++;		//pass to type
	if (type != 'B')
		return FALSE;

	length = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	int i = 0;
	for (i = pos; tempPkt[i] != '\0'; i++) {
		portal += tempPkt[i];
	}
	if (i == pos)
		pos++;
	else
		pos+=i;
	i = 0;
	for (i=pos; tempPkt[i] != '\0'; i++) {
		statement += tempPkt[i];
	}
	if (i == pos)
		pos++;
	else
		pos += i;

	parameterFormatCount = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);

	USHORT* paramterFormatList = 0;
	if (parameterFormatCount > 0) {
		paramterFormatList = (USHORT*)malloc(sizeof(USHORT) * parameterFormatCount);
		memcpy(paramterFormatList, &tempPkt[pos], sizeof(short) * parameterFormatCount);
		pos += sizeof(short) * parameterFormatCount;
	}

	parameterValueCount = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);
	

	UINT col_length = 0;
	int th = 0;
	for (auto iter = oid_list.begin(); iter != oid_list.end(); ++iter) {
		memcpy(&col_length, &tempPkt[pos], sizeof(UINT));
		endswap(&col_length);
		pos += sizeof(UINT);
		u_char* tempOidType = (u_char*)malloc(sizeof(UINT));
		memcpy(tempOidType, &(*iter), sizeof(UINT));
		*info_str += get_bind_value(info_str, &tempPkt[pos], col_length,&tempOidType, th);
		th += 1;
		free(tempOidType);
		pos += col_length;
		if (th != parameterValueCount)
			*info_str += L", ";
	}
	


	return TRUE;
}

BOOL cWinPcapKDY::set_bind(CString* info_str, u_char* tempPkt, int* Pkt_pos, u_char** oid_list)
{
	// Name					byte
	// B : Bind
	// Type					1
	// Length				4
	// Porter				가변 + 00
	// Statement			가변 + 00
	// Parameter Format		2 
	// Format				2 (위의 Parameters의 값만큼 갯수가 존재함)
	// Parameter values		2
	// Column Length		4 (위의 Parameter values의 값만큼 갯수가 존재함) |
	// Data					4 (위의 Parameter values의 값만큼 갯수가 존재함) | Column Length 와 Data는 2개에 한세트
	// Result formats		2

	int length = 0;
	int pos = 0;

	CString portal = L"";
	CString statement = L"";
	int parameterFormatCount = 0;
	int parameterValueCount = 0;

	pos++;		//pass to type

	length = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(int);

	int i = 0;
	for (; tempPkt[pos] != '\0'; pos++) {
		portal += tempPkt[pos + i];
	}

	pos++;	//pass to 00

	for (; tempPkt[pos] != '\0'; pos++) {
		statement += tempPkt[pos + i];
	}

	pos++; //pass to 00


	parameterFormatCount = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);

	USHORT* paramterFormatList = 0;
	if (parameterFormatCount > 0) {
		paramterFormatList = (USHORT*)malloc(sizeof(USHORT) * parameterFormatCount);
		memcpy(paramterFormatList, &tempPkt[pos], sizeof(short) * parameterFormatCount);
		pos += sizeof(short) * parameterFormatCount;
	}

	parameterValueCount = get_byteToShort(&tempPkt[pos]);
	pos += sizeof(short);
	int col_length = 0;
	u_char* parameterValue = NULL;
	int j = 0;
	int oid_pos = 0;
	if (parameterFormatCount > 0) {
		*info_str += L", ";
		for (i = 0; i < parameterValueCount; i++) {
			if (i > 0)
				*info_str += L" ,";
			col_length = get_byteToInt(&tempPkt[pos]);
			pos += sizeof(int);
			parameterValue = (u_char*)malloc(sizeof(u_char) * col_length);
			u_char* oid = (u_char*)malloc(sizeof(int));
			memcpy(parameterValue, &tempPkt[pos], col_length);
			memcpy(oid, &oid_list[0][oid_pos],sizeof(int));
			
			if (&paramterFormatList[i] == 0) {
				//text
				*info_str += get_bind_value(info_str, parameterValue, col_length, &oid, i);
			}
			else{
				//binary
				*info_str += get_bind_value(info_str, parameterValue, col_length, &oid, i);
				//*info_str += get_bind_value_binary(info_str, parameterValue, col_length, &oid, i);
			}
			//*info_str += get_bind_value(info_str,parameterValue,col_length, &oid, i);
			oid_pos += sizeof(int);
			pos += col_length;
			free(parameterValue);
			free(oid);
		}
		free(oid_list[0]);
	}
	*Pkt_pos += (pos - *Pkt_pos);
	return TRUE;
}

/*
void a(u_char* pkt) {
	u_char* oid_list = NULL;
	b(&oid_list,pkt);
	c(&oid_list);
	if(oid_list != NULL)
		free(oid_list);
}

void b(u_char** oid_list,u_char* pkt) {
	int pos = 0;
	int count = 2;
	*oid_list = (u_char*)malloc(sizeof(int) * count);
	memcpy(oid_list, pkt, sizeof(int) * count);
}

void c(u_char** oid_list) {
	int pos = 0;
	for (int i = 0; i < 2; i++) {
		u_char* copy_oid = (u_char*)malloc(sizeof(int));
		memcpy(copy_oid, &oid_list[pos], sizeof(int));
		pos += sizeof(int);
		//...
		free(copy_oid);
	}
}
*/

BOOL cWinPcapKDY::set_ExtendedQuery(CString* info_str, u_char* data, int dataSize)
{
	int totalLength = 0;
	CString password = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);
	int pos = 0;


	CString statement = L"";

	*info_str += L"Query : ";

	memcpy(tempPkt, data, dataSize);

	pos++;	//pass to type
	
	UINT length = 0;
	length = get_byteToInt(&tempPkt[pos]);
	pos += sizeof(UINT);

	if (length > 80)
		int asdd = 0;

	int stateMentCount = 0;
	if (tempPkt[pos] != 0) {
		//STATEMENT
		while (tempPkt[pos + stateMentCount] != 0) {
			statement += tempPkt[pos];
			//pos++;
			stateMentCount++;
		}
	}
	if (stateMentCount != 0)
		pos += stateMentCount;
	else
		pos += 1;
	//query length 구하기.
	UINT queryLen = 0;
	while (tempPkt[pos + queryLen] != '\0')
	{
		queryLen++;
	}
	u_char* temp = (u_char*)calloc(1, queryLen);
	memcpy(temp, &tempPkt[pos], queryLen);
	*info_str += setCharacterSet(&tempPkt[pos], queryLen);
	pos += queryLen + 1;	//1 is 00 1byte
	*info_str += L")";



	USHORT parameterCount = 0;
	memcpy(&parameterCount, &tempPkt[pos], sizeof(USHORT));
	pos += sizeof(USHORT);
	endswap(&parameterCount);

	UINT oid = 0;
	std::list<UINT> oid_list;
	for (int i = 0; i < parameterCount; i++) {
		memcpy(&oid, &tempPkt[pos], sizeof(UINT));
		endswap(&oid);
		oid_list.push_back(oid);
		pos += sizeof(UINT);
	}

	//here.kkk
	if(oid_list.size() > 0)
		set_bind(info_str, &tempPkt[pos],dataSize - pos, oid_list);

	



	/*
	u_char* oid_list = NULL;
	if (!set_parse(info_str, &tempPkt[pos], &pos, &oid_list)) {
		return TRUE;
	}
	if (tempPkt[pos] != 'B') {
		return FALSE;
	}
	else {
		set_bind(info_str, &tempPkt[pos], &pos, &oid_list);
	}
	int checkLast = (totalLength - pos);
	*/
	free(tempPkt);
	return TRUE;
}

BOOL cWinPcapKDY::set_ExtendedQuery(CString* info_str,PACKET_INFO* info, u_char* data, int dataSize)
{
	int totalLength = 0;
	CString password = L"";
	u_char* tempPkt = (u_char*)malloc(dataSize);
	int pos = 0;

	memcpy(tempPkt, data, dataSize);

	u_char* oid_list = NULL;
	if (!set_parse(info_str, &tempPkt[pos], &pos, &oid_list)) {
		return TRUE;
	}
	if (tempPkt[pos] != 'B') {
		return FALSE;
	}
	else {
		set_bind(info_str, &tempPkt[pos], &pos, &oid_list);
	}
	int checkLast = (totalLength - pos);
	free(tempPkt);
	return TRUE;
}

BOOL cWinPcapKDY::set_parseComletion(CString * info_str, u_char * data, int dataSize)
{
	CString temp_str = L"";
	u_char* temp_pkt = (u_char*)malloc(dataSize);
	memcpy(temp_pkt, data, dataSize);

	int pos = 0;

	int tempLen = 0;

	for (; pos < dataSize;) {
		u_char type;
		memcpy(&type, &temp_pkt[pos], sizeof(u_char));
		pos += sizeof(u_char);
		UINT length = 0;
		memcpy(&length, &temp_pkt[pos], sizeof(UINT));
		pos += sizeof(UINT);
		endswap(&length);
		u_char* tempData = (u_char*)malloc(length);
		if (tempData != NULL)
			memset(tempData, 0, length);
		if (length - 4 > 0) {
			memcpy(tempData, &temp_pkt[pos], length - 4);
		}
		else
			free(tempData);

		pos += length - 4;

		switch (type) {
		case '1':
			*info_str += L"Parse completion";
			break;
		case '2':
			*info_str += L"Bind completion";
			break;
		case 'C':
			*info_str += L"Command completion(Tag : ";
			
			for (UINT j = 0; j < length - 4 - 1; j++) {
				*info_str += tempData[j];
			}
			*info_str += L")";
			break;
		case 'S':
			*info_str += L"Parameter status(";

			for (UINT j = 0,check = 0; j < length - 4 - 1; j++) {
				if (check == 0) {
					*info_str += L"Parameter name : ";
					check = 1;
				}
				else {
					*info_str += L"Parameter value : ";
					check = 0;
				}
				while (tempData[j] != 0) {
					*info_str += tempData[j];
					j++;
				}
				if(j + 1 != length - 4)
					*info_str += L",";
			}
			*info_str += L")";
			break;
		case 'Z':
			*info_str += L"Ready For query";
			break;
		case 'T':
			*info_str += L"Row Description";
			break;
		default:
			TRACE(L"Type is %d. can't find solution of this type\n", type);
		}
		if (pos != dataSize)
			*info_str += L", ";


		if(length - 4 > 0)
			free(tempData);
	}


	return TRUE;
}

void cWinPcapKDY::postgreSQL_packet(REAL_PACKET* packet, CString* postgreSql_str)
{
	CString temp_str = L"", str = L"";
	u_char* temp_pkt = (u_char*)malloc(packet->totalLength);
	memcpy(temp_pkt, packet->realData, packet->totalLength);

	int pos = 0;

	u_char firstByte = temp_pkt[pos];

	TRACE(L"first byte is %d\n", firstByte - 30);

	
	if (firstByte == 0)
	{
		//startup message
		TRACE(L"Start up message~\n");
		*postgreSql_str = L"";

		if (!set_startUpMessage(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
	}
	else if (firstByte == 'R')
	{
		*postgreSql_str = L"(Authentication request) ";
		if (!set_AuthenticationRequest(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
	}
	else if (firstByte == 'Q')
	{
		//simple query
		*postgreSql_str = L"(QUERY) ";
		if (!set_SimpleQueryMessage(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
		TRACE(L"Simple query message~\n");
	}
	//
	else if (firstByte == 'E')
	{
		//Error
		*postgreSql_str = L"(ERROR) ";
		if (!set_ErrorMessage(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
		int aaaaa = 0;
	}
	else if (firstByte == '1')
	{
		//*postgreSql_str = L"(Password message) ";
		if (!set_parseComletion(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
	}

	else if (firstByte == 'X')
	{
		*postgreSql_str = L"(Termination) ";
	}
	else if (firstByte == 'p')
	{
		*postgreSql_str = L"(Password message) ";
		if (!set_PasswordMessage(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
	}
	else if (firstByte == 'P')
	{
		*postgreSql_str = L"(Query[Parameter]) ";
		if (!set_ExtendedQuery(postgreSql_str, &temp_pkt[pos], packet->totalLength)) {
			*postgreSql_str = L"ERROR";
		}
	}
	free(temp_pkt);

}

void cWinPcapKDY::postgreSQL_packet(PACKET_INFO* info, RAW_PACKET* packet,CString* postgreSql_str)
{
	CString temp_str = L"", str = L"";
	u_char* temp_pkt = (u_char*)malloc(info->caplen);
	memcpy(temp_pkt, packet->pkt_data, info->caplen);

	int pos = 0;

	pos += info->startDataPos;

	u_char firstByte = temp_pkt[pos];

	TRACE(L"first byte is %d\n", firstByte-30);

	if (firstByte == 0)
	{
		//startup message
		TRACE(L"Start up message~\n");
		*postgreSql_str = L"";
		
		if (!set_startUpMessage(postgreSql_str, &temp_pkt[pos],info->endDataPos - info->startDataPos)) {
			*postgreSql_str = L"ERROR";
		}
	}
	else if (firstByte == 'Q')
	{
		//simple query
		*postgreSql_str = L"(QUERY) ";
		if (!set_SimpleQueryMessage(postgreSql_str, &temp_pkt[pos], info->endDataPos - info->startDataPos)) {
			*postgreSql_str = L"ERROR";
		}
		TRACE(L"Simple query message~\n");
	}
	else if (firstByte == 'E')
	{
		//Error
		*postgreSql_str = L"(ERROR) ";
		if (!set_ErrorMessage(postgreSql_str, &temp_pkt[pos], info->endDataPos - info->startDataPos)) {
			*postgreSql_str = L"ERROR";
		}
	}
	/*
	else if (firstByte == 'R')
	{
		*postgreSql_str = L"(Authentication request) ";
		if (!set_AuthenticationRequest(info,postgreSql_str,packet)) {
			*postgreSql_str = L"ERROR";
		}
	}
	*/
	else if (firstByte == 'X')
	{
		*postgreSql_str = L"(Termination) ";
	}
	else if (firstByte == 'p')
	{
		*postgreSql_str = L"(Password message) ";
		if (!set_PasswordMessage(postgreSql_str, &temp_pkt[pos], info->endDataPos - info->startDataPos)) {
			*postgreSql_str = L"ERROR";
		}
	}
	else if (firstByte == 'P')
	{
		*postgreSql_str = L"(Query[Parameter]) ";
		if (!set_ExtendedQuery(postgreSql_str,info,&temp_pkt[pos],info->endDataPos - info->startDataPos)) {
			*postgreSql_str = L"ERROR";
		}
	}
	
	/*
	//data
	temp_str += "DATA : ";
	for (int i = info->startDataPos; i < info->caplen; i++) {
		//		str.Format(L" %02x ", (unsigned int)*(pkt_data + i));
		str.Format(L"%c", (unsigned char)*(temp_pkt + i));
		if (i == info->startDataPos &&
			(((unsigned char)*(temp_pkt + i)) == 0))
		{
			//Start Up
			//
		}
		else
		{
			//00으로 시작하는건 Start up 메세지 뿐? 2022.02.03
		}
		temp_str += str;
	}
	temp_str += " || ";
	TRACE(temp_str + L"\n");
	*/
//	temp_pkt -= info->startDataPos;
	free(temp_pkt);

}

/*
BOOL cWinPcapKDY::settingEtherFrame(const u_char* pkt_data, ETHER_HEADER* ether_header,
	int pkt_length, PACKET_INFO* info)
{
	packet_seek = 0;
	u_char* pos_pkt = (u_char*)pkt_data;
	int headerTotalLength = 0;
	u_char* data;

	info->etherHeaderPos = 0;
	packet_seek += 14;

	info->ipHeaderPos = packet_seek;
	memcpy(ether_header, pkt_data, 14);
	pos_pkt += 14;
	
	info->type = ntohs(ether_header->type);

	switch (info->type)
	{
	case IPV4_TYPE:
		IPV4_HEADER ipV4Header;
		settingIpv4Header(pkt_data,&ipV4Header);
		info->protocol = ipV4Header.proto;
		break;
	case ARP_TYPE:
		ARP_HEADER arpHeader;
		settingArpHeader(pkt_data, &arpHeader);
		info->protocol = arpHeader.protoType;
		break;
	case IPV6_TYPE:
		IPV6_HEADER ipV6Header;
		settingIpv6Header(pkt_data, &ipV6Header);
		info->protocol = ipV6Header.nextHeader;
		break;
	case VLAN_TYPE:
		break;
	default:

		break;
	}

	info->protocolPos = packet_seek;

	if (info->type != NULL) {
		switch (info->protocol) {
		case TCP_PROTOCOL:
			TCP_HEADER tcpHeader;
			settingTcpPacket(pkt_data, &tcpHeader);
			break;
		}
	}
	info->startDataPos = packet_seek + 1;
	info->endDataPos = info->caplen - sizeof(ether_header->FCS);

	if(info->endDataPos > info->startDataPos){
		//exist data
		int dataLength = info->endDataPos - info->startDataPos;
		data = (u_char*)malloc(sizeof(u_char) * dataLength);
		pos_pkt += info->startDataPos;
		memcpy(data, pos_pkt, sizeof(u_char) * dataLength);
		pos_pkt = (u_char*)pkt_data;
		pos_pkt += info->endDataPos;

		packet_seek += dataLength;
	}
	else {
		//no data
		pos_pkt = (u_char*)pkt_data;
		pos_pkt += (info->caplen - sizeof(ether_header->FCS));
		memcpy(&ether_header->FCS, pos_pkt, sizeof(ether_header->FCS));
	}
	return TRUE;
}
*/
std::list<u_char> temp_vec;
BOOL cWinPcapKDY::settingIpv4Header(const u_char* pkt_data, IPV4_HEADER* ip_header,int& position)
{
	int ipv4DefaultHeaderSize = 20;

	unsigned char filter = 0x80;
	
	int headerLength = 0;
	int version_ip = 0;

	//get version , header length
	ip_header->ver_ihl = pkt_data[position];
	int number = 0;
	for (int i = 0, j = 1; i < 8; i++, j *= 2) {
		number = getAbit(ip_header->ver_ihl, i);
		if (i == 4)
			j = 1;
		if (i < 4) {
			if (number == 1)
				headerLength += j;
		}
		else {
			if (number == 1)
				version_ip += j;
		}
	}
	headerLength *= 4;

	u_char* tempPkt = (u_char*)malloc(ipv4DefaultHeaderSize);
	memcpy(tempPkt, &pkt_data[position], ipv4DefaultHeaderSize);
	memcpy(ip_header, &tempPkt[0], ipv4DefaultHeaderSize);
	position += ipv4DefaultHeaderSize;
	if (headerLength > ipv4DefaultHeaderSize) {
		//20 is ipv4's default header length..
		ip_header->op_pad = TRUE;
		ip_header->option = (u_char*)malloc(headerLength - ipv4DefaultHeaderSize);
		memcpy(ip_header->option, &pkt_data[position], headerLength - ipv4DefaultHeaderSize);
		position += headerLength - ipv4DefaultHeaderSize;
	}

	free(tempPkt);

	//endswap(&ip_header->tlen);
	return TRUE;
}

BOOL cWinPcapKDY::settingIpv6Header(const u_char* pkt_data, IPV6_HEADER* ip_header, int& position)
{
	unsigned char filter = 0x80;

	int headerLength = sizeof(IPV6_HEADER);
	int version_ip = 0;
	u_char* pos_pkt = (u_char*)pkt_data;

	memcpy(ip_header, &pos_pkt[position], sizeof(IPV6_HEADER));
	int num = 0;
	for (int i = 8, j = 1; i < 12; i++, j *= 2) {
		num = getAbit(ntohs(ip_header->ver_ihl), i);
		if (num == 1)
			headerLength += (num * j);
	}
	for (int i = 12, j = 1; i < 16; i++, j *= 2) {
		num = getAbit(ntohs(ip_header->ver_ihl), i);
		if (num == 1)
			version_ip += (num * j);
	}

//	headerLength = (headerLength * 32) / 8;
	position += headerLength;
	return TRUE;
}

BOOL cWinPcapKDY::settingArpHeader(const u_char* pkt_data, ARP_HEADER* ip_header, int& position)
{
	int headerLength = sizeof(ARP_HEADER);
	u_char* pos_pkt = (u_char*)pkt_data;
	memcpy(ip_header, &pos_pkt[position], sizeof(ARP_HEADER));
	position += headerLength;
	return TRUE;
}

/*
BOOL checkTcpOptions(const u_char* pkt_data)
{
	packet_seek = 0;
//	ip_header temp_header;
//	ether_header temp_header_ether;
	
//	settingEtherFrame(pkt_data, &temp_header_ether);
//	settingIpHeader(pkt_data, &temp_header);


//	memcpy(&temp_header, pkt_data, sizeof(ip_header));
	
	int k = 0;


	return TRUE;
}
*/
int get_TCP_HeaderLength(const u_char* copy_pkt_data,int seek,int beforeOfFlagSize)
{
	copy_pkt_data = copy_pkt_data + beforeOfFlagSize ;
	
	u_short* flag = new u_short;

	memset(flag, 0, sizeof(u_short));
	memcpy(flag, copy_pkt_data, sizeof(u_short));

	// 첫 4bit가 header length이다.
	int length = 0;
	for (int i = 4,j = 1; i < 8; i++,j*=2)
	{
		length += j * getAbit(flag, i);
	}

	delete flag;

	length *= 4;

	return length;
}

BOOL cWinPcapKDY::settingTcpPacket(const u_char* pkt_data,TCP_HEADER* tcp_packet_info, int& position) {
	int default_headerLength = 20;

	//set header length..
	int headerLength = 0;
	int headerLengthPosition = 12;
	BYTE len = pkt_data[position + headerLengthPosition];
	int number = 0;
	for (int i = 4, j = 1; i < 8; i++, j *= 2) {
		number = getAbit(len, i);
		if (number == 1)
			headerLength += j;
	}
	headerLength *= 4;

	if (headerLength > default_headerLength) {
		tcp_packet_info->optionExist = TRUE;
	}
	memcpy(tcp_packet_info, &pkt_data[position], default_headerLength);
	position += default_headerLength;
	if (tcp_packet_info->optionExist == TRUE) {
		tcp_packet_info->options = (u_char*)malloc(headerLength - default_headerLength);
		memcpy(tcp_packet_info->options, &pkt_data[position], headerLength - default_headerLength);
		position += headerLength - default_headerLength;
	}
	return TRUE;
}

BOOL cWinPcapKDY::settingUdpPacket(const u_char* pkt_data, UDP_HEADER* udp_packet_info, int& position) {
	//	BYTE* btPacket = (BYTE*)malloc(sizeof(length));
	BYTE* btPacket = (BYTE*)pkt_data;
	u_char* pos_pkt = (u_char*)pkt_data;
//	pos_pkt += packet_seek;
	int default_headerLength = sizeof(UDP_HEADER);
	int plus_option = 0;

	int temp = 0;
	memcpy(udp_packet_info, &pos_pkt[position], default_headerLength);

	position += default_headerLength;

	return TRUE;
}

void cWinPcapKDY::makeEmptyQueue(std::queue<RAW_PACKET*>* queue)
{
	std::queue<RAW_PACKET*>* pElement;
	
	checkRecvPacket = RECV_CHECK_END;
	pElement = queue;
	while (pElement->size() > 0)
	{
		//delete pElement->front()->pkt_data;
		free(pElement->front()->pkt_data);

		//list 추가로 인한 free 추가
		if (pElement->front()->authenticationRequestStatus == TRUE) {
			for (auto iter : pElement->front()->authenticationRequestDataList) {
				if(iter != NULL)
					free(iter);
			}
		}

		delete pElement->front();
		pElement->pop();
	}
}

void cWinPcapKDY::stopRecvPacket()
{
	//this->check_count = 0;
	//2022.01.10
	//현재 패킷을 수집 후 저장하는 queue를 종료시켜야 한다.
	//thread_PacketRecv.detach();
	for (UINT i = 0; i < thread_PacketRecv.size(); i++) {
		//thread_PacketRecv.at(i).~thread();
		//if(thread_PacketRecv.at(i).joinable())
		SuspendThread(thread_PacketRecv.at(i).native_handle());
		checkRecvPacket = RECV_CHECK_END;
	}
}

void cWinPcapKDY::startRecvPacket()
{
	//this->check_count = 0;
	//2022.01.10
	//현재 패킷을 수집 후 저장하는 queue를 종료시켜야 한다.
	//thread_PacketRecv.();
	for (UINT i = 0; i < thread_PacketRecv.size(); i++) {
		if (!thread_PacketRecv.at(i).joinable())
			ResumeThread(thread_PacketRecv.at(i).native_handle());
		checkRecvPacket = RECV_CHECK_START;
	}
}

#define WTAP_MAX_PACKET_SIZE 65535

void cWinPcapKDY::savePcapFile(CString filePath,CListCtrl* m_list)
{
	PCAP_HDR_S globalHeader;
	PCAPREC_HDR_S packetHeader;
	
	globalHeader.magic_number = 0xa1b2c3d4;
	globalHeader.major_ver = 2;
	globalHeader.minor_ver = 4;
	globalHeader.thiszone = 0;
	globalHeader.sigfigs = 0;
	globalHeader.snapLen = WTAP_MAX_PACKET_SIZE;
	globalHeader.linkType = 1;

	FILE* fp = 0;
//	const char* st = CT2CA((LPCWSTR)filePath);
	
//	if (fp = fopen("C:\\AAA\\pcap_sample\\2.pcap", "wb+"))
	if (fp = fopen(CT2CA((LPCWSTR)filePath), "wb+"))
	{
		fwrite(&globalHeader, sizeof(PCAP_HDR_S), 1, fp);
	}

	for (int i = 0; i < m_list->GetItemCount(); i++) {
		RAW_PACKET* pRawPacket = (RAW_PACKET*)m_list->GetItemData(i);
		packetHeader.ts_sec = pRawPacket->tv.tv_sec;
		packetHeader.ts_usec = pRawPacket->tv.tv_usec;
		packetHeader.incl_len = pRawPacket->pkt_len;
		packetHeader.orig_len = pRawPacket->len;
		
		fwrite(&packetHeader, sizeof(PCAPREC_HDR_S), 1, fp);
		fwrite(pRawPacket->pkt_data, sizeof(u_char)*pRawPacket->pkt_len, 1, fp);
		
	}
	int check = fclose(fp);

}

CString cWinPcapKDY::pgsql_checkMessage()
{
	//1. pgsql인지 파악할 수 있어야 한다.
	//들어온 패킷에 대해서 실제 데이터 부를 제외한 나머지를 가져와야 한다.
	//get_packetData
	CString str = L"";
	return str;
}

BOOL get_PacketData(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	cWinPcapKDY* packet_data = (cWinPcapKDY*)(param);
	PACKET_INFO info;
	
	packet_data->settingPacketInfo(pkt_data, header->caplen, &info);
	//info setting 완료

	//현재 info엔 데이터부가 없다.
	int k = 0;
	
	return TRUE;
}
/*
u_char* cWinPcapKDY::make_oneData(REAL_PACKET* real, std::list<POSTGRESQL_TYPE_LIST*> list) {

	UINT totalLength = 0;
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		totalLength += (*iter)->length;
	}
	real->totalLength = totalLength;
	u_char* tempPkt = (u_char*)malloc(totalLength);

	//return tempPkt;

	int pos = 0;
	UINT len = 0;
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		memcpy(&tempPkt[pos], &(*iter)->type, 1);
		pos += 1;
		//return tempPkt;
		len = (*iter)->length;
		endswap(&len);
		memcpy(&tempPkt[pos], &len, sizeof(int));
		pos += 4;
		//return tempPkt;
		memcpy(&tempPkt[pos], &(*iter)->data[0], (*iter)->current_data_length);
		pos += (*iter)->current_data_length;
		return tempPkt;	//2023.04.16 이 줄에서 return tempPkt;를 주석처리하지 않으면, 문제 없음, 무엇이 문제인지?

	}

	return tempPkt;
}
*/

UINT temp_count = 0;

u_char* cWinPcapKDY::make_oneData(REAL_PACKET* real, std::list<POSTGRESQL_TYPE_LIST*> list){
	
	UINT totalLength = 0;
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		totalLength += (*iter)->length + 1;
	}
	real->totalLength = totalLength;
	//u_char* tempPkt = (u_char*)malloc(totalLength);

	real->realData = (u_char*)realloc(real->realData, real->totalLength);
	memset(real->realData, 0, real->totalLength);
	

	int pos = 0;
	UINT len = 0;
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		temp_count++;
		//TRACE(L"temp_count = %d\n", temp_count);

		if (temp_count == 2) {
			int abcd = 0;
		}

		memcpy(&real->realData[pos], &(*iter)->type, 1);
		pos += 1;
		//return tempPkt;
		len = (*iter)->length;
		endswap(&len);
		memcpy(&real->realData[pos], &len, sizeof(UINT));
		pos += 4;
		//return tempPkt;
		memcpy(&real->realData[pos], &(*iter)->data[0], (*iter)->current_data_length);
		pos += (*iter)->current_data_length;
		//return real->realData;	//2023.04.16 이 줄에서 return tempPkt;를 주석처리하지 않으면, 문제 없음, 무엇이 문제인지?
		
	}

	return real->realData;
}

BOOL push_packet(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	std::mutex m;
	
	//2023.04.06 MULTI PACKET 처리
	//완성된 패킷만 사용하자.
	//[SYN], [SYN,ACK], [ACK] 과정은 무시한다. -> 삭제하는 것이 아닌 ORIGINAL_PAKCET에 쌓는다(pcap 파일 저장을 위해)
	//[FIN], [FIN,ACK], [ACK] 과정이 발생하면 queue_packet에 쌓은 다음 MAP에서 지워준다.
	struct TCP_FLAG flag_info;
	BOOL check = FALSE;

	// 2023.04.17 추가..
		// header->caplen이 1514 이상일 시엔 return;(즉, 무시한다.)로 예정.
		// 사유 : TCP/IP의 최대 수신 패킷 크기는 1514byte이기 때문..
	/*
	if (header->caplen > 1514)
	{
		return FALSE;
	}
	*/

//	m.lock();
	cWinPcapKDY* packet_data = (cWinPcapKDY*)(param);
	if (packet_data->checkRecvPacket == RECV_CHECK_START) {
		PACKET_INFO info;
		info.tv.tv_usec = header->ts.tv_usec;
		info.tv.tv_sec = header->ts.tv_sec;
		info.caplen = header->caplen;

		//info.pkt_data = (u_char*)malloc(info.caplen);	//간혈적으로 이곳에서 heap 손상 발생.
		if (temp_count >= 0)
			TRACE(L"temp_count = %d, capeln = %d\n", temp_count, header->caplen);

		info.pkt_data = (u_char*)calloc(1, header->caplen);
		
		//memset(info.pkt_data, 0, info.caplen); //malloc을 calloc으로 대체
		try {
			//std::copy(&pkt_data[0], &pkt_data[header->caplen], info.pkt_data);
			memcpy(info.pkt_data, pkt_data, header->caplen);
		}
		catch (std::exception& e) {
			TRACE(L"%s\n",e.what());
		}
		
		//memcpy(info.pkt_data, pkt_data, header->caplen);
		//TRACE(L"memcpy done\n");
		check = packet_data->settingPacketInfo(pkt_data, header->caplen , &info);

		if (check == FALSE) {
			//do not recv this packet..
			return FALSE;
		}
		//2023.04.11추가.
		if (info.dataLength == 0) {
			//2023.04.16 추가
			if(info.caplen > 0)
				free(info.pkt_data);
			if(info.exist_protocol_data == TRUE)
				free(info.protocol_data);
			return FALSE;
			//2023.04.16 추가 끝
		}
		
		struct_session session;
		check = packet_data->set_structSession(&info, &session);
		if (check == FALSE) {
			return FALSE;
		}

		CString key = packet_data->make_mapSessionKey(session);
		check = packet_data->check_sessionPacket(&info);
		if (check == TRUE) {
			//TCP
			packet_data->checkTcpFlag((u_char*)info.protocol_data, flag_info);
			if ((flag_info.PSH && flag_info.ACK) ||
				((!flag_info.PSH && flag_info.ACK) &&
					(info.dataLength != 0)
						)) {
				//REAL_PACKET 세팅.
				REAL_PACKET* real = new REAL_PACKET;
				packet_data->set_postgreSQL_real_packet(&info, real);

				BOOL multiCheck = FALSE;
				std::list<POSTGRESQL_TYPE_LIST*> temp_list;
				if (packet_data->type_list_map.count(key)) {
					//만약 multi라면?
					temp_list = packet_data->make_postgreSQL_type_list(real->realData, real->currentLength,key);
					auto iter = packet_data->type_list_map.find(key);
					if (iter != packet_data->type_list_map.end()) {
						multiCheck = packet_data->check_postgreSQL_type_list_done((*iter).second);
					}
					else{
						multiCheck = packet_data->check_postgreSQL_type_list_done(temp_list);
					}
				}
				else {
					//return;
					//multi가 아닌, 혹은 첫번째 패킷이라면?
					//아래 코드에서 문제가 생김. 2023.04.03
					temp_list = packet_data->make_postgreSQL_type_list(real->realData, real->currentLength,key);

					//2023.04.16 수정! 알수 없는 메모리 에러, 1th
					//변경사항, multiCheck가 TRUE이면,(인자로 들어온 리스트의 데이터 구조체 중 done이라는 항목이 하나라도 FALSE이면 TRUE를 반환)
					// insert하는 구조이다. 기존엔 if문이 없어서 insert로 이전 맵 데이터를 덮어쓰기를 해버려서 로직상 에러였다.
					multiCheck = packet_data->check_postgreSQL_type_list_done(temp_list);
					if(multiCheck == TRUE)
						packet_data->type_list_map.insert({ key,temp_list });
				}

				if (multiCheck == FALSE && temp_list.size() != 0) {
					//multiCheck가 TRUE라면, 현재 처리가 아니어야 한다.
					//FALSE라면, 처리한다.
					packet_data->make_oneData(real, temp_list);
					struct_session original_session;
					original_session.server_ip = info.sourceIP;
					original_session.server_port = info.sourcePort;
					original_session.client_ip = info.destinationIP;
					original_session.client_port = info.destinationPort;
					real->session = original_session;		
					packet_data->FOR_UI.push(real);
					packet_data->type_list_map.erase(key);
				}
				
				if (temp_list.size() > 0) {
					temp_list.clear();
				}
			}
		}
	}
//	m.unlock();
}

BOOL cWinPcapKDY::set_postgreSQL_real_packet_totalLength(REAL_PACKET* real)
{
	u_char* tempPkt = (u_char*)malloc(real->currentLength);
	memcpy(tempPkt, real->realData, real->currentLength);
	int pos = 0;

	int totalLength = 0;
	int checkLength = 0;

	u_char firstByte = tempPkt[pos];
	pos++;

	TRACE(L"first byte is %d\n", firstByte - 30);

	if (firstByte == 0)
	{
		//startup message
		pos--;
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == 'Q')
	{
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == 'E')
	{
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == 'R')
	{
		memcpy(&checkLength, &tempPkt[pos], sizeof(int));
		endswap(&checkLength);
	}
	else if (firstByte == 'X')
	{
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == 'p')
	{
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == 'P')
	{
		memcpy(&real->totalLength, &tempPkt[pos], sizeof(int));
	}
	else if (firstByte == '1')
	{
			int kkk = 0;
	}
	endswap(&real->totalLength);
	free(tempPkt);
	if (real->totalLength <= 0)
		return FALSE;

	return TRUE;
}

BOOL cWinPcapKDY::set_postgreSQL_real_packet(PACKET_INFO* info, REAL_PACKET* realPacket)
{
	u_char* tempProtocol = (u_char*)malloc(info->headerLength.protocol);
	
	memcpy(tempProtocol, &info->pkt_data[info->protocolPos], info->headerLength.protocol);
	//현재 들어온 패킷의 SEQ, ACK를 가져온다.
	memcpy(&realPacket->seqNum, &tempProtocol[4], sizeof(int));
	memcpy(&realPacket->ackNum, &tempProtocol[8], sizeof(int));
	realPacket->currentLength = info->dataLength;
	realPacket->realPacket = info;
	realPacket->realData = (u_char*)malloc(realPacket->currentLength);
	memcpy(realPacket->realData, &info->pkt_data[info->startDataPos], info->dataLength);
	free(tempProtocol);
	return TRUE;
}

BOOL cWinPcapKDY::delete_struct_PACKET_INFO(PACKET_INFO* info)
{
	if (info->caplen > 0)
		free(info->pkt_data);
	if (info->exist_protocol_data == TRUE )
		free(info->protocol_data);
	return TRUE;
}

BOOL cWinPcapKDY::delete_struct_REAL_PACKET(REAL_PACKET* real)
{
	delete_struct_PACKET_INFO(real->realPacket);
	if (real->currentLength > 0)
		free(real->realData);

	return TRUE;
}

BOOL cWinPcapKDY::delete_queue_struct_REAL_PACKET(std::queue<REAL_PACKET*> queue)
{
	delete_struct_REAL_PACKET(queue.front());
	return TRUE;
}

BOOL cWinPcapKDY::erase_map_key_queue(CString key,std::map<CString, std::queue<REAL_PACKET*>> map)
{
	std::map<CString, std::queue<REAL_PACKET*>>::iterator map_iter = map.find(key);
	std::queue<REAL_PACKET*>& temp_queue = (*map_iter).second;
	
	for (UINT i = 0; i < temp_queue.size(); i++) {
		REAL_PACKET* iter = temp_queue.front();
		free(iter->realPacket->pkt_data);
		free(iter->realPacket->protocol_data);
		temp_queue.pop();
	}
	return TRUE;
}

BOOL cWinPcapKDY::make_intact_realData(REAL_PACKET* returnReal,CString key)
{
	std::map<CString, std::queue<REAL_PACKET*>>::iterator iter = realPacketMap.find(key);
	int pos = 0, tlen = 0;
	REAL_PACKET* real_iter = (*iter).second.front();
	u_char* pkt = 0;

	for (UINT i = 0; i < (*iter).second.size(); i++) {
		if (tlen == 0) {
			tlen = real_iter->totalLength;
			if (tlen == 0)
				//단일 패킷
				tlen = real_iter->currentLength;

			pkt = (u_char*)malloc(tlen);
		}
		else
			real_iter = (*iter).second.front();
		memcpy(&pkt[pos], (*real_iter).realData, (*real_iter).currentLength);
		pos += (*real_iter).currentLength;


		TRACE(L"here!\n");
//		TRACE(get_hexStringAndUtf8String((*iter).second.front()->realData,(*iter).second.front()->totalLength) + L"\n");
		delete_queue_struct_REAL_PACKET((*iter).second);
		
		(*iter).second.pop();
	}
	realPacketMap.erase(key);


	returnReal->realData = (u_char*)malloc(tlen);
	memcpy(returnReal->realData, &pkt[0], tlen);
	returnReal->totalLength = tlen;
	returnReal->check_Use_realPacket = TRUE;
	CString tempString = get_hexStringAndUtf8String(&pkt[0],tlen);

//	TRACE(L"STR : " + tempString + L"\n");

	return TRUE;
}

std::list<POSTGRESQL_TYPE_LIST*> cWinPcapKDY::make_postgreSQL_type_list(u_char* pkt_data, UINT dataLength,CString key)
{
	std::list<POSTGRESQL_TYPE_LIST*> temp;

	UINT current_length = 0;
	int pos = 0;
	POSTGRESQL_TYPE_LIST* data;

	BOOL check_push = TRUE;

	current_length = dataLength;
	//return temp;
	for (UINT i = 0; i < dataLength;)
	{
		data = new POSTGRESQL_TYPE_LIST;
		
		//type
		data->type = pkt_data[i];
		//return temp;

		if (type_list_map.size() > 0) {
			//return temp;
			auto iter = type_list_map.find(key);
			auto last_list = (*iter).second.begin();
			for (; (*last_list)->done != FALSE; ++last_list);
			if ((*last_list)->done == FALSE) {
				UINT len = (*last_list)->length - (*last_list)->current_data_length - 4;
				if (len > dataLength) {
					//추가적인 패킷 존재.
					memcpy(&(*last_list)->data[(*last_list)->current_data_length], &pkt_data[0], dataLength);
					(*last_list)->current_data_length += dataLength;

					if ((*last_list)->current_data_length == (*last_list)->length) {
						(*last_list)->done = TRUE;
						i += (*last_list)->current_data_length + len;
					}
					else
						i = dataLength;

					check_push = FALSE;
				}
				else {
					memcpy(&(*last_list)->data[(*last_list)->current_data_length], &pkt_data[0], len);
					(*last_list)->done = TRUE;
					(*last_list)->current_data_length += len;

					data->type = (*last_list)->type;
					data->done = (*last_list)->done;
					data->current_data_length = (*last_list)->current_data_length;
					data->length = (*last_list)->length;
					data->data = (u_char*)malloc(data->current_data_length);
					memcpy(&data->data[0], &(*last_list)->data[0], data->current_data_length);
					check_push = TRUE;
					i += len;
					//free(&(*last_list)->data[0]);

					for (auto rePushIter = (*iter).second.begin(); rePushIter != (*iter).second.end(); ++rePushIter) {
						if ((*rePushIter)->done == TRUE && (*rePushIter) != (*iter).second.back())
							temp.push_back((*rePushIter));
					}
					type_list_map.erase(key);
				}
			}
			else {
				//multi 부분 done 완료
				if (data->type != 0) {
					current_length -= 1;
					i++;
				}
				
				
				//length
				memcpy(&data->length, &pkt_data[i], sizeof(int));
				i += sizeof(int);
				current_length -= 4;
				endswap(&data->length);

				//data
				data->data = (u_char*)malloc(data->length - 4);
				memcpy(data->data, &pkt_data[i], data->length - 4);

				if (data->length > current_length + 4) {
					data->current_data_length = current_length;
					data->done = FALSE;
					i = dataLength;
				}
				else {
					data->current_data_length = data->length - 4;
					current_length -= data->length - 4;
					i += data->length - 4;
					data->done = TRUE;
				}
			}
		}
		else {

			//current_length => 데이터 영역의 총 크기.
			//data->length => 현재 타입 part의 데이터 영역의 총 크기. , (length + data)
			//data->current_data_length => 현재 타입 part의 데이터 영역의 총 크기 - 현재 저장된 크기. length - (sizeof(int))
			

			//return temp;
			if (data->type != 0) {
				current_length -= 1;
				i++;
			}

			if (data->type == 'T')
				int kkkkk = 0;

			//length
			memcpy(&data->length, &pkt_data[i], sizeof(int));
			i += sizeof(int);
			current_length -= 4;
			endswap(&data->length);

			//return temp;
			//data
			if (data->length > current_length + 4) {
				//multi
				data->current_data_length = current_length;
				data->done = FALSE;
				//i = dataLength;
			}
			else {
				//non-multi
				data->current_data_length = data->length - 4;
				current_length -= data->length - 4;
				//i += data->length - 4;
				data->done = TRUE;
			}

			if (data->current_data_length == 0) {
				data->data = (u_char*)malloc(data->current_data_length);
				memcpy(data->data, &pkt_data[i], data->current_data_length);
				i += data->current_data_length;
			}
			else if (data->current_data_length > 0) {
				if (data->done == TRUE)
					data->data = (u_char*)malloc(data->current_data_length);
				else {
					//data->data = (u_char*)malloc(data->length - data->current_data_length - 4);
					//memset(data->data, 0, data->length - data->current_data_length - 4);

					data->data = (u_char*)malloc(data->length - 4);
					memset(data->data, 0, data->length - 4);

				}
				memcpy(data->data, &pkt_data[i], data->current_data_length);
				i += data->current_data_length;
			}
			//return temp;	//2023.04.16 위의 return temp; 와 이곳 중간에 문제 생김.
			//+ 2023.04.03. 위의 data->length에 알 수 없는 큰 값이 들어와 pkt_data의 할당된 원래의 메모리가 아닌 다른
			// 메모리를 걸들이는 문제를 발견. ->  현재 로직에서는 멀티 패킷인걸 확인하면, type_list_map에 insert하는 구조인데, insert하지 않으므로
			// 멀티인 기존의 패킷을 처리하지 않고, 들어온 데이터를 그대로 다시 type 과 length를 계산하여 생긴 문제이다.
		}
		

		if(check_push == TRUE || data->done == TRUE)
			temp.push_back(data);
	}
	return temp;
}

/*
BOOL cWinPcapKDY::check_postgreSQL_type_list_first_done(std::list<POSTGRESQL_TYPE_LIST*> list)
{
	auto iter = list.front();
	if (iter->done == TRUE) {
		return TRUE;
	}
	return FALSE;
}
*/
BOOL cWinPcapKDY::check_postgreSQL_type_list_done(std::list<POSTGRESQL_TYPE_LIST*> list)
{
	int i = list.size();
	for (auto iter = list.begin(); i != 0; ++iter,i--) {
		if ((*iter)->done == FALSE)
			return TRUE;
	}
	return FALSE;
}


std::list<POSTGRESQL_TYPE_LIST*> cWinPcapKDY::make_combine_list(std::list<POSTGRESQL_TYPE_LIST*> list1, std::list<POSTGRESQL_TYPE_LIST*> list2)
{
	std::list<POSTGRESQL_TYPE_LIST*> combine_list(list1.begin(),list1.end());
	auto iter = combine_list.end();
	combine_list.splice(iter, list2);
	return combine_list;
}

BOOL cWinPcapKDY::check_postgreSQL_packet_multi_end(REAL_PACKET* realPacket_front, CString key)
{
	//SEQ , ACK
	//다음 패킷의 SEQ가 현재의 ACK와 같다면 세트이다.
	//즉, multi packet은 이전 패킷의 SEQ, ACK가 같은 값이 들어오다가 다음에 들어온
	//패킷의 SEQ가 ACK와 같다면 끝인것이다.
	std::map<CString, std::queue<REAL_PACKET*>>::iterator iter = realPacketMap.find(key);
	REAL_PACKET* last = (*iter).second.back();
	REAL_PACKET* first = (*iter).second.front();
	BOOL multiEndCheck = FALSE;

	int totalLength = first->totalLength;
	int length = 0;

	for (auto iter = realPacket_front; iter != NULL; ++iter) {
		if (totalLength <= length) {
			multiEndCheck = TRUE;
		}
		else
			length += iter->currentLength;
	}

	if (multiEndCheck == TRUE)
		return multiEndCheck;
	else
		return multiEndCheck;
}

void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data )
{
	std::mutex m;

	m.lock();
	push_packet(param, header, pkt_data);	//2023.02.01 lock, unlock을 사용하여 처리, 테스트 필요.
	m.unlock();
}

void packet_handler_for_pcap_file(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	errno_t err;
	FILE* fp;
	err = fopen_s(&fp,"C:\\AAA\\abcd.txt", "w");
//	printf("-------------No.%d-----------------\n", count);
	/* 패킷 출력 */
	long sec = header->ts.tv_sec;
	long usec = header->ts.tv_usec;
	sec /= 10000;
	usec /= 100000;
	unsigned char* x = (unsigned char*)sec;
	unsigned char* z = (unsigned char*)usec;

	fwrite(&x, sizeof(x), 1, fp);
	fwrite(&z, sizeof(z), 1, fp);
	fwrite((unsigned char*)&header->caplen, sizeof(header->caplen), 1, fp);
	fwrite((unsigned char*)&header->len, sizeof(header->len), 1, fp);

	CString str = L"";
	for (int i = 0; i < (int)header->caplen; i++)
	{
		fwrite(&pkt_data, header->caplen, 1, fp);
	}
	int k = 0;
	if (count == 50)
	{
		fclose(fp);
		exit(0);
	}
	count++;
}

void print_ether_header(const unsigned char* data) {
//	struct ether_header* eh;

}