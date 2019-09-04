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
	
	//创建一个Socket
	listenSock= socket(AF_INET, SOCK_STREAM, 6);
	if (listenSock == INVALID_SOCKET) {
		cout << "创建Socket失败...\n";
		WSACleanup();
		return 1;
	}
	else {
		cout << "创建Socket成功...\n";
	}

	sockaddr_in s;
	s.sin_family = AF_INET;
	s.sin_port = DEFAULT_PORT;
	s.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定端口
	if (SOCKET_ERROR == bind(listenSock, (sockaddr*)&s,(int)sizeof(s))) {
		cout << "绑定端口失败...\n";
		//freeaddrinfo(result);
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "绑定端口成功...\n";
	}

	//freeaddrinfo(result);

	//监听网络端口
	if (SOCKET_ERROR == listen(listenSock, SOMAXCONN)) {
		cout << "监听网络端口失败...\n";
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "监听网络端口成功...\n";
	}

	//等待客户端连接
	//addrinfo c;
	//ZeroMemory(&c, sizeof(c));
	sockaddr_in c = {};
	int c_len=sizeof(c);

	clientSock = accept(listenSock,(sockaddr*)&c,&c_len);
	if(clientSock==INVALID_SOCKET){
		cout << "客户端连接失败...\n";
		//WSACleanup();
		//return 1;
	}
	else {
		cout << "客户端连接成功...\n";
	}

	char recvbuf[BUFFER_LEN];
	int iResult;

	do {
		//接收客户端数据
		iResult = recv(clientSock, recvbuf, BUFFER_LEN, 0);

		if (iResult > 0) {
			cout << "接收到" << iResult << "字节		内容： " << recvbuf << std::endl;
		}
		else if (iResult == 0) {
			cout << "客户端连接关闭...\n";
			break;
		}
		else {
			cout << "接收客户端数据失败...\n";
			continue;
			//closesocket(clientSock);
			//WSACleanup();
			//return 1;
		}
	} while (true);

	//断开服务器
	if (shutdown(clientSock, SD_SEND) == SOCKET_ERROR) {
		cout << "服务器关闭连接失败...\n";
		closesocket(clientSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "服务器关闭连接成功...\n";
	}

	closesocket(clientSock);
	closesocket(listenSock);

	WSACleanup();
	return 0;
}