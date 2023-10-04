#include "TCP_SOCKET_CLIENT.h"

bool TCP_SOCKET_CLIENT::Initialize(bool checkRecv)
{
	int check = 0;

	if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0)
		return false;

	this->listener = socket(PF_INET, SOCK_STREAM, 0);
	if (this->listener == INVALID_SOCKET)
		return false;

	u_long on = 1;
	if (!checkRecv) {
		// client
		if (::ioctlsocket(listener, FIONBIO, &on) == INVALID_SOCKET) {
			return false;
		}
	}

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
		address.sin_addr.s_addr = inet_addr(SERVER_IP);
	}

	if (checkRecv) {
		check = listen(listener, SOMAXCONN);
		if (check != 0)
			return false;
	}
	int fd = 0;

	while (true && !checkRecv) {
		// non-block client socket connect
		if (connect(this->listener, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR) {
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			if (::WSAGetLastError() == WSAEISCONN)
				break;

			// error
			break;
		}
	}
	
	// non-block socket to block socket
	on = 0;
	if (!checkRecv) {
		// client
		if (::ioctlsocket(listener, FIONBIO, &on) == INVALID_SOCKET) {
			return false;
		}
	}
	

	while (true) {
		//Sleep(1000);

		FD_ZERO(&reads);
		FD_SET(listener, &reads);

		temps = this->reads;

		timeout.tv_sec = 0;
		timeout.tv_usec = 50000;

		int s = select(listener + 1, &temps, 0, 0, &timeout);

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
					}
				}
				else {
					// read
					int kkk = 0;
				}
			}
		}

	}

}