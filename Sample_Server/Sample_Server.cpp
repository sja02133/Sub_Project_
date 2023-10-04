#include <WinSock2.h>

#include <iostream>

#pragma comment (lib , "ws2_32.lib")



#define WINSOCK_DEPERECATED_NO_WARNINGS



void showError(const char* msg)

{

	std::cout << "���� : " << msg << std::endl;

	exit(-1);

}



int main()

{

	WSADATA data;

	::WSAStartup(MAKEWORD(2, 2), &data);



	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (server == INVALID_SOCKET)
		showError("���� ���� ����");



	sockaddr_in addr = { 0 };



	addr.sin_family = AF_INET;

	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	addr.sin_port = htons(7000);



	if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		showError("���ε� ����");



	if (listen(server, SOMAXCONN) == SOCKET_ERROR)
		showError("��� ����");



	std::cout << "Ŭ���̾�Ʈ�� ��ٸ��ϴ�.." << std::endl;
	


	SOCKET client = accept(server, NULL, NULL);

	std::cout << "Ŭ���̾�Ʈ accept!" << std::endl;

	char message[30];

	int strLen = recv(client, message, sizeof(message) - 1, 0);

	int result = send(client, message, strlen(message) + 1, 0);

	closesocket(client);



	closesocket(server);

	::WSACleanup();

	return 0;

}