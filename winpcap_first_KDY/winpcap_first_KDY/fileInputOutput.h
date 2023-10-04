#pragma once
//#include "DlgMonitoring.h"	//2023.02.10 �ּ�
#include <io.h>
#include <fstream>
#include <vector>

typedef struct struct_ipPort {
	CString ip;
	CString s_port;
	UINT port;
}struct_ipPort;


// 2023.02.09 class ���信�� �׳� header ��, �����Լ�, 
// ���������� ���� �ٸ����� ȣ���ؼ� ����.
// 2023.02.10 �Ϸ�

static CString server_list_filePath = L"";
static CString oid_type_filePath = L"";
extern std::vector<struct_ipPort> vector_serverList;

BOOL get_programDirectory(CString* dirPath);
BOOL check_existServerListTxt();
BOOL make_serverListTextFile(CString* filePath);
BOOL add_server_IpPort(struct_ipPort ipPort, std::vector<struct_ipPort> vec);
BOOL add_server_IpPort(struct_ipPort ipPort);
BOOL delete_server_IpPort(struct_ipPort ipPort);
//Server list

BOOL check_existPostgresOidTypeVer11Txt(CString fileName);
//oid list

std::vector<struct_ipPort> get_IpPortList();
BOOL get_IpPortList(std::vector<struct_ipPort>* vec);

//
BOOL check_ipV4(CString ip);

//port range is 0 ~ 65535
BOOL check_port(CString port);
BOOL check_port(int port);