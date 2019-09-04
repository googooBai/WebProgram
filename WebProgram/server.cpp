#include "server.h"

const USHORT DEFAULT_PORT=12000;
const unsigned BUFFER_LEN = 16;

int Server() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET listenSock=INVALID_SOCKET;
	SOCKET clientSock = INVALID_SOCKET;

	/*addrinfo s;
	ZeroMemory(&s, sizeof(s));
	s.ai_family = AF_INET;
	s.ai_socktype = SOCK_STREAM;
	s.ai_protocol = IPPROTO_TCP;
	s.ai_flags = AI_PASSIVE;

	addrinfo* result;
	if (getaddrinfo(NULL, DEFAULT_PORT, &s, &result) != 0) {
		cout << "getaddrinfo failed with error\n";
		WSACleanup();
		return 1;
	}*/
	
	//����һ��Socket
	listenSock= socket(AF_INET, SOCK_STREAM, 6);
	if (listenSock == INVALID_SOCKET) {
		cout << "����Socketʧ��...\n";
		WSACleanup();
		return 1;
	}
	else {
		cout << "����Socket�ɹ�...\n";
	}

	sockaddr_in s;
	s.sin_family = AF_INET;
	s.sin_port = DEFAULT_PORT;
	s.sin_addr.S_un.S_addr = INADDR_ANY;

	//�󶨶˿�
	if (SOCKET_ERROR == bind(listenSock, (sockaddr*)&s,(int)sizeof(s))) {
		cout << "�󶨶˿�ʧ��...\n";
		//freeaddrinfo(result);
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "�󶨶˿ڳɹ�...\n";
	}

	//freeaddrinfo(result);

	//��������˿�
	if (SOCKET_ERROR == listen(listenSock, SOMAXCONN)) {
		cout << "��������˿�ʧ��...\n";
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "��������˿ڳɹ�...\n";
	}

	//�ȴ��ͻ�������
	//addrinfo c;
	//ZeroMemory(&c, sizeof(c));
	sockaddr_in c = {};
	int c_len=sizeof(c);

	clientSock = accept(listenSock,(sockaddr*)&c,&c_len);
	if(clientSock==INVALID_SOCKET){
		cout << "�ͻ�������ʧ��...\n";
		//WSACleanup();
		//return 1;
	}
	else {
		cout << "�ͻ������ӳɹ�...\n";
	}

	char recvbuf[BUFFER_LEN];
	int iResult;

	do {
		//���տͻ�������
		iResult = recv(clientSock, recvbuf, BUFFER_LEN, 0);

		if (iResult > 0) {
			cout << "���յ�" << iResult << "�ֽ�		���ݣ� " << recvbuf << std::endl;
		}
		else if (iResult == 0) {
			cout << "�ͻ������ӹر�...\n";
			break;
		}
		else {
			cout << "���տͻ�������ʧ��...\n";
			continue;
			//closesocket(clientSock);
			//WSACleanup();
			//return 1;
		}
	} while (true);

	//�Ͽ�������
	if (shutdown(clientSock, SD_SEND) == SOCKET_ERROR) {
		cout << "�������ر�����ʧ��...\n";
		closesocket(clientSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "�������ر����ӳɹ�...\n";
	}

	closesocket(clientSock);
	closesocket(listenSock);

	WSACleanup();
	return 0;
}