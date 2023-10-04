#include "pch.h"
#include "fileInputOutput.h"

//현재 시스템의 경로를 가져온다.
BOOL get_programDirectory(CString* dirPath)
{
	TCHAR path[512];
//	GetSystemDirectoryW(path, 500);
	_tgetcwd(path, 500);
	dirPath->Format(_T("%s"), path);
	if (dirPath->GetLength() != 0)
		return TRUE;
	else
		return FALSE;
}

BOOL check_existPostgresOidTypeVer11Txt(CString fileName) {
	CString tempDirPath = L"";
	//tempDirPath.Format(L"%s",dirPath->GetBuffer());

	if (tempDirPath.GetLength() == 0)
		get_programDirectory(&tempDirPath);

	tempDirPath += L"\\info";

	if (_waccess(tempDirPath.GetBuffer(), 0) == -1) {
		TRACE(L"none exist info Folder!\n");
		if ((::CreateDirectory(tempDirPath, NULL)) != 0) {
			TRACE(L"make info Folder success\n");
		}
		else
			return FALSE;
	}


	tempDirPath += L"\\" + fileName + L".txt";
	CString traceStr = L"";
	if (_waccess(tempDirPath.GetBuffer(), 0) == -1) {
		traceStr = L"none exist" + fileName + L".txt File!\n";
		TRACE(traceStr);
		if (make_serverListTextFile(&tempDirPath) == TRUE) {
			traceStr = L"make" + fileName + L"file success\n";
			TRACE(traceStr);
		}
		else
			return FALSE;
	}
	traceStr = L"check" + fileName + L"file success\n";
	TRACE(traceStr);

	oid_type_filePath = tempDirPath;

	return TRUE;
}

//현재 시스템 경로의 서버 리스트 파일(server_list.txt) 파일의 존재 여부를 묻고
//없으면 새로 만든 뒤 유저에게 따로 알려준다
BOOL check_existServerListTxt()
{
	CString tempDirPath = L"";
	//tempDirPath.Format(L"%s",dirPath->GetBuffer());

	if (tempDirPath.GetLength() == 0)
		get_programDirectory(&tempDirPath);

	tempDirPath += L"\\info";

	if (_waccess(tempDirPath.GetBuffer(), 0) == -1) {
		TRACE(L"none exist info Folder!\n");
		if ((::CreateDirectory(tempDirPath, NULL)) != 0) {
			TRACE(L"make info Folder success\n");
		}
		else
			return FALSE;
	}
	

	tempDirPath += L"\\server_list.txt";
	
	if (_waccess(tempDirPath.GetBuffer(), 0) == -1) {
		TRACE("none exist server_list.txt File!\n");
		if (make_serverListTextFile(&tempDirPath) == TRUE) {
			TRACE(L"make server_list file success\n");
		}
		else
			return FALSE;
	}
	TRACE(L"check server_list file success\n");

	server_list_filePath = tempDirPath;

	return TRUE;
}

// /info 폴더가 존재한다는 전재하에 이 함수 실시하여야 한다.
BOOL make_serverListTextFile(CString* filePath)
{
	WCHAR* str = T2W(filePath->GetBuffer());
	std::ofstream o_file;
	//o_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\server_list.txt");
	o_file.open(filePath->GetBuffer());
	filePath->ReleaseBuffer();

	if (o_file.fail()) {
		TRACE(L"server_list.txt file create failed...\n");
		return FALSE;
	}
	o_file << "## \"##\" is annotation ex) ## here is annotation" << std::endl;
	o_file << std::endl;
	o_file << "#SERVER IP:PORT" << std::endl;

	o_file.close();
	
	TRACE(L"now server_list.txt file is created.. but need to write SERVER IP:PORT\n");
	return TRUE;
}

BOOL check_ipV4(CString ip)
{
	//개인 IPv4 주소의 규칙
	//1. '.'은 세개가 무조건 들어가 있어야 한다.
	//2. 각 숫자 포지션엔 3자리수 & 255 이하여야 한다.

	TCHAR type = L' ';
	CString tempIp = ip;
	CString id = L"";
	int pos = 0, id_check = 0;
	int checkCount = 0;
	int breakCount = 0;
	while (checkCount != 4) {
		//checkCount는 4 이상이 나올 수 없다. 허나 만약 4 미만이어도
		// '.'의 갯수가 2개 이하인 상태에서 해당 loop가 계속 돌수도 있으니
		// 그걸 위해 breakCount를 따로 만들어 해당 breakCount가 10이상이 되면
		// break
		//이곳에선 network id, host id를 체크한다.
		pos = tempIp.Find('.');

		//pos == 0 이면 '.'부터 만났다. -> 실패
		//pos >= 4 이면 id 자릿수가 3자리가 넘어간다 -> 실패
		//pos == -1 이면 아직 checkCount가 끝까지 도달하지 못했는데 ',' 못찾았다. 실패.
		if (pos == 0 || pos >= 4 && (pos == -1 && checkCount != 3)) {
			TRACE(L"IP's Network ID is error\n");
			break;
		}
		if (pos == -1)
			pos = tempIp.GetLength();
		id = tempIp.Mid(0, pos);
		//id가 255이 넘거나 0이하인가?
		id_check = _ttoi(id);
		if (id_check < 0 || id_check > 255) {
			TRACE(L"IP's Network ID or Host ID is error, pos : %d\n", checkCount + 1);
			break;
		}

		//처음온 id의 범위에 따라 class가 나뉘어 진다.
		//현재는 활용 방안이 딱히 없다... loopback인지 정도만 확인하여 활용 가능성?
		if (checkCount == 0) {
			if (id_check >= 0 && id_check <= 127)
				type = 'A';
			else if (id_check >= 128 && id_check <= 191)
				type = 'B';
			else if (id_check >= 192 && id_check <= 223)
				type = 'C';
			else if (id_check >= 224 && id_check <= 239)
				type = 'D';
			else if (id_check >= 240 && id_check <= 255)
				type = 'E';
			else
				TRACE(L"??? class, populor to check IP\n");
		}
		checkCount++;

		breakCount++;
		if (breakCount == 10) {
			//무슨 문제가 생겼다.
			TRACE(L"happend any problem in this LOOP\n");
			return FALSE;
		}
		tempIp = tempIp.Mid(pos+1,tempIp.GetLength());
	}
	if (type == 'D' || type == 'E') {
		TRACE(L"can't use this IP class type. class type : %c\n", type);
		return FALSE;
	}

	if (checkCount-1 == 3)
		return TRUE;

	TRACE(L"IP's Network ID or Host ID is error, pos : %d\n", checkCount + 1);
	return FALSE;
}

BOOL check_port(CString port)
{
	int int_port = _ttoi(port);
	
	for (int i = 0; i < port.GetLength(); i++) {
		if (isdigit(port.GetAt(i)) == 0) {
			TRACE(L"Port value is wrong... value : %c", port.GetAt(i));
			return FALSE;
		}		
	}

	if (int_port < 0 || int_port > 65535) {
		TRACE(L"Port value is wrong... port : %d", int_port);
		return FALSE;
	}

	return TRUE;
}

BOOL check_port(int port)
{
	if (port < 0 || port > 65535) {
		TRACE(L"Port value is wrong... port : %d", port);
		return FALSE;
	}
	return TRUE;
}

BOOL add_server_IpPort(struct_ipPort ipPort, std::vector<struct_ipPort> vec)
{
	std::wfstream io_file;
	//o_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\server_list.txt");

	BOOL test = FALSE;

	//test value
	// 100.100.100.100:1234
	//test = TRUE;
	if (test == TRUE) {
		ipPort.ip = L"100.100.100.100";
		ipPort.s_port = L"1234";
		ipPort.port = 1234;
	}

	if (ipPort.ip != L"") {
		if (!check_ipV4(ipPort.ip)) {
			TRACE(L"IP check is failed.. IPv4\n");
			return FALSE;
		}
	}

	if (ipPort.s_port == L"") {
		if (!check_port(ipPort.s_port) || !check_port(ipPort.port)) {
			TRACE(L"Port value is wrong\n");
			return FALSE;
		}
	}

	if (server_list_filePath.GetLength() == 0) {
		if (!check_existServerListTxt()) {
			TRACE(L"server_list.txt file create or check failed...\n\n");
			return FALSE;
		}
	}
	io_file.open(server_list_filePath.GetBuffer());
	server_list_filePath.ReleaseBuffer();

	if (io_file.fail()) {
		TRACE(L"server_list.txt file create failed...\n");
		return FALSE;
	}
	wchar_t line[256] = { 0, };
	BOOL findServerIpPortPos = FALSE;
	CString s = L"";
	UINT seekCount = 0;
	int exitCount = 0;
	//while (io_file.getline(line, 256)) {
	while (io_file.getline(line, 1024)) {

		if (findServerIpPortPos == TRUE) {
			// SERVER IP:PORT 위치 찾음.
		}

		//2023.02.08 SEEK 위치를 찾기 위한 seekCount
		if (lstrlenW(line))
			seekCount += lstrlenW(line);
		seekCount += 1;

		//		if (!s.empty() &&s.at(0) == '#') {
		if (line[0] == '#') {
			//			if (s.at(1) == '#') {
			if (line[1] == '#') {
				// 주석
			}
			else {
				//주석 아님.
				wchar_t* p = StrStrIW(line, L"SERVER IP:PORT");
				//				std::size_t pos = s.find("SERVER IP:PORT");
				if (p != NULL) {
					seekCount += 1;
					io_file.seekg(seekCount, std::ios::beg);
					io_file << std::endl;
					//int k = io_file.tellg();
					CString appendStr = ipPort.ip + ":" + ipPort.s_port;
					//io_file.write("\r\n", strlen("\r\n"));
					io_file << appendStr.GetBuffer() << std::endl;
					break;
				}
			}
		}

		exitCount++;
		if (exitCount == 10)
			break;

	}
	io_file.close();

	if (findServerIpPortPos == TRUE)
		return TRUE;
	else {
		TRACE(L"any problem to find IP:PORT text in file..\n");
		return FALSE;
	}
}

BOOL add_server_IpPort(struct_ipPort ipPort)
{
	std::wfstream io_file;
	//o_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\server_list.txt");
	
	BOOL test = FALSE;

	//test value
	// 100.100.100.100:1234
	//test = TRUE;
	if (test == TRUE) {
		ipPort.ip = L"100.100.100.100";
		ipPort.s_port = L"1234";
		ipPort.port = 1234;
	}

	if (ipPort.ip != L"") {
		if (!check_ipV4(ipPort.ip)) {
			TRACE(L"IP check is failed.. IPv4\n");
			return FALSE;
		}
	}

	if (ipPort.s_port == L"") {
		if (!check_port(ipPort.s_port) || !check_port(ipPort.port)) {
			TRACE(L"Port value is wrong\n");
				return FALSE;
		}
	}

	if (server_list_filePath.GetLength() == 0) {
		if (!check_existServerListTxt()) {
			TRACE(L"server_list.txt file create or check failed...\n\n");
			return FALSE;
		}
	}
	io_file.open(server_list_filePath.GetBuffer());
	server_list_filePath.ReleaseBuffer();

	if (io_file.fail()) {
		TRACE(L"server_list.txt file create failed...\n");
		return FALSE;
	}
	wchar_t line[256] = { 0, };
	BOOL findServerIpPortPos = FALSE;
	CString s = L"";
	UINT seekCount = 0;
	int exitCount = 0;
	//while (io_file.getline(line, 256)) {
	while (io_file.getline(line,1024)) {
		//2023.02.08 SEEK 위치를 찾기 위한 seekCount
		if (lstrlenW(line)) {
			seekCount += lstrlenW(line);
			seekCount += 2;
		}

		io_file.seekg(seekCount-1, std::ios::beg);
		TRACE(L"seekCount - 1's value is %c\n", io_file.get());
		io_file.seekg(seekCount, std::ios::beg);
		TRACE(L"seekCount's value is %c\n", io_file.get());
		io_file.seekg(seekCount+1, std::ios::beg);
		TRACE(L"seekCount + 1's value is %c\n", io_file.get());
		io_file.clear();

//		if (!s.empty() &&s.at(0) == '#') {
		if (line[0] == '#') {
//			if (s.at(1) == '#') {
			if (line[1] == '#') {
				// 주석
			}
			else {
				//주석 아님.
				wchar_t* p = StrStrIW(line, L"#SERVER IP:PORT");
//				std::size_t pos = s.find("SERVER IP:PORT");
				if (p != NULL) {
//					seekCount += lstrlenW(line);
//					io_file.seekg(seekCount, std::ios::beg);
					findServerIpPortPos = TRUE;
				}
			}
		}

		if (findServerIpPortPos == TRUE) {
			// SERVER IP:PORT 위치 찾음.
			//seekCount += lstrlenW(line);
			//seekCount += 2;
			//192.168.2.190:5432
			io_file.seekg(seekCount, std::ios::beg);
			CString appendStr = ipPort.ip + ":" + ipPort.s_port + L"\r\n";
			//io_file.write("\r\n", strlen("\r\n"));
			io_file << appendStr.GetBuffer();
			findServerIpPortPos = TRUE;
			break;
		}

		exitCount++;
		if (exitCount == 10)
			break;

	}
	io_file.close();

	if (findServerIpPortPos == TRUE)
		return TRUE;
	else {
		TRACE(L"any problem to find IP:PORT text in file..\n");
		return FALSE;
	}
}

BOOL get_IpPortList(std::vector<struct_ipPort>* vec)
{
	std::ifstream i_file;
	//o_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\server_list.txt");

	if (server_list_filePath.GetLength() == 0) {
		if (!check_existServerListTxt()) {
			TRACE(L"server_list.txt file create or check failed...\n\n");
			return FALSE;
		}
	}

	i_file.open(server_list_filePath.GetBuffer());
	server_list_filePath.ReleaseBuffer();

	if (i_file.fail()) {
		TRACE(L"server_list.txt file open failed...\n");
		return FALSE;
	}

	char line[256] = { 0, };
	BOOL findServerIpPortPos = FALSE;
	UINT zeroCount = 0;
	while (i_file.getline(line, 256)) {
		if (findServerIpPortPos == TRUE) {
			// SERVER IP:PORT 위치 찾음.
			if (line[0] == '#' || (strlen(line) == 0) || line[0] == '\r') {
				//findServerIpPortPos = FALSE;
				if (strlen(line) == 0)
					zeroCount++;
				if (line[0] == '#' || zeroCount == 2) {
					findServerIpPortPos = FALSE;
					break;
				}
				continue;
			}
			else {
				//주석이 아님.

				CString temp = CString(line);
				UINT findPortPos = temp.Find(':', 0);
				struct_ipPort ipPort;
				ipPort.ip = temp.Mid(0, findPortPos);
				ipPort.s_port = temp.Mid(findPortPos + 1, temp.GetLength());
				ipPort.port = _ttoi(ipPort.s_port);
				if (check_ipV4(ipPort.ip) && check_port(ipPort.s_port))
					vec->push_back(ipPort);
				else {
					ipPort.ip = L"";
					ipPort.port = 0;
					ipPort.s_port = L"";
				}
				zeroCount = 0;
			}
		}
		if (line[0] == '#') {
			if (line[1] == '#') {
				// 주석
			}
			else {
				//주석 아님.
				char* p = strstr(line, "SERVER IP:PORT");
				if (p == NULL) {
					i_file.close();
					return FALSE;
				}
				else {
					findServerIpPortPos = TRUE;
					continue;
				}
			}
		}
	}
	i_file.close();
	return TRUE;
}

std::vector<struct_ipPort> get_IpPortList()
{
	std::vector<struct_ipPort> vec;
	std::ifstream i_file;
	//o_file.open(L"C:\\Users\\KDY\\source\\repos\\winpcap_first_KDY\\winpcap_first_KDY\\info\\server_list.txt");

	if (server_list_filePath.GetLength() == 0) {
		if (!check_existServerListTxt()) {
			TRACE(L"server_list.txt file create or check failed...\n\n");
			return vec;
		}
	}

	i_file.open(server_list_filePath.GetBuffer());
	server_list_filePath.ReleaseBuffer();

	if (i_file.fail()) {
		TRACE(L"server_list.txt file open failed...\n");
		return vec;
	}

	char line[256] = { 0, };
	BOOL findServerIpPortPos = FALSE;
	UINT zeroCount = 0;
	while (i_file.getline(line, 256)) {
		if (findServerIpPortPos == TRUE) {
			// SERVER IP:PORT 위치 찾음.
			if (line[0] == '#' || (strlen(line) == 0) || line[0] == '\r') {
				//findServerIpPortPos = FALSE;
				if (strlen(line) == 0)
					zeroCount++;
				if (line[0] == '#' || zeroCount == 2) {
					findServerIpPortPos = FALSE;
					break;
				}
				continue;
			}
			else {
				//주석이 아님.
				
				CString temp = CString(line);
				UINT findPortPos = temp.Find(':', 0);
				struct_ipPort ipPort;
				ipPort.ip = temp.Mid(0, findPortPos);
				ipPort.s_port = temp.Mid(findPortPos + 1, temp.GetLength());
				ipPort.port = _ttoi(ipPort.s_port);
				if(check_ipV4(ipPort.ip) && check_port(ipPort.s_port))
					vec.push_back(ipPort);
				else {
					ipPort.ip = L"";
					ipPort.port = 0;
					ipPort.s_port = L"";
				}
				zeroCount = 0;
			}
		}
		if (line[0] == '#') {
			if (line[1] == '#') {
				// 주석
			}
			else {
				//주석 아님.
				char *p = strstr(line, "SERVER IP:PORT");
				if (p == NULL) {
					i_file.close();
					return vec;
				}
				else {
					findServerIpPortPos = TRUE;
					continue;
				}
			}
		}
	}
	i_file.close();
	return vec;
}