#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WinSock2.h>

#include <list>

#pragma comment(lib,"ws2_32.lib")

constexpr auto SERVER_PORT = 7000;
constexpr auto SERVER_IP = "192.168.2.239";

class TCP_SOCKET_CLIENT
{
public:
	WSADATA wsa;
	SOCKET listener;
	sockaddr_in address;

	// Ãß°¡
	fd_set reads, temps;
	struct timeval timeout;
	std::list<SOCKET> m_list;

	bool Initialize(bool checkRecv);	// Recv => 1, Send => 0
};

