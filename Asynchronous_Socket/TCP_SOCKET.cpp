#include "TCP_SOCKET.h"

bool TCP_SOCKET::Initialize(bool checkRecv)
{
	int check = 0;

	if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0)
		return false;

	this->listener = socket(PF_INET, SOCK_STREAM, 0);
	if (this->listener == INVALID_SOCKET)
		return false;

	if (checkRecv) {
		// Server
		memset(&address, 0, sizeof(sockaddr_in));
		address.sin_family = AF_INET;
		address.sin_port = htons(SERVER_PORT);
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		check = bind(this->listener, (sockaddr*)&address, sizeof(sockaddr_in));

		if (check != 0)
			return false;

	}
	else {
		// Client
		memset(&address, 0, sizeof(sockaddr_in));
		address.sin_family = AF_INET;
		address.sin_port = htons(SERVER_PORT);
		address.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	}
	
	if (checkRecv) {
		check = listen(listener, SOMAXCONN);
		if (check != 0)
			return false;
	}
	int fd = 0;


	while (true) {
		Sleep(1000);
		
		FD_ZERO(&reads);
		FD_SET(listener, &reads);

		temps = this->reads;

		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		int s = select(listener+1, &temps, 0, 0, &timeout);

		if (s < 0)
			break;
		else if (s == 0)
			continue;

		for (int i = 0; i < reads.fd_count; i++) {
			if (FD_ISSET(reads.fd_array[i], &temps)) {
				if (reads.fd_array[i] == this->listener && checkRecv) {
					// 연결 요청
					SOCKET hCltSock = accept(this->listener, nullptr, nullptr);
					if (hCltSock == INVALID_SOCKET) {
						closesocket(listener);
						WSACleanup();
						return false;
					}
					else {
						m_list.push_back(hCltSock);
						char* a = (char*)malloc(1);
						char b[1] = { 'A' };
						//memcpy(a, b, 1);
						//send(hCltSock, a, 1,0);
					}
				}
				else {
					// read
				}
			}
		}
		
	}

}