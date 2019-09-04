#include "EasyTCPClient.h"

const USHORT DEFAULT_PORT = 12000;
const unsigned BUFFER_LEN = 16;
const char* sendbuf = "BOOKBOOKBOOKBOOKBOOKBOOK";

int Client() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	/*addrinfo c;
	ZeroMemory(&c, sizeof(c));
	c.ai_family = AF_INET;
	c.ai_protocol = IPPROTO_TCP;
	c.ai_socktype = SOCK_STREAM;
	
	addrinfo* result;
	if (getaddrinfo("127.0.0.1", DEFAULT_PORT, &c, &result) != 0) {
		cout << "getaddrinfo failed with error...\n";
		WSACleanup();
		return 1;
	}*/

	//创建Socket
	SOCKET connectSock = INVALID_SOCKET;
	connectSock = socket(AF_INET, SOCK_STREAM,6);
	if (connectSock == INVALID_SOCKET) {
		cout << "创建Socket失败...\n";
		//freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else {
		cout << "创建Socket成功...\n";
	}

	sockaddr_in c;
	c.sin_family = AF_INET;
	c.sin_port = DEFAULT_PORT;
	inet_pton(AF_INET,"127.0.0.1",(void*)&c.sin_addr.S_un);
	if (SOCKET_ERROR == connect(connectSock, (sockaddr*)&c, (int)sizeof(c))) {
		cout << "连接服务器失败...\n";
		closesocket(connectSock);
		//freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else {
		cout << "连接服务器成功...\n";
	}

	//freeaddrinfo(result);

	if (SOCKET_ERROR == send(connectSock, sendbuf, strlen(sendbuf)+1,0)) {
		cout << "客户端发送失败...\n";
		closesocket(connectSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "客户端发送成功...\n";
	}

	if (SOCKET_ERROR == shutdown(connectSock, SD_SEND)) {
		cout << "客户端关闭连接失败...\n";
		closesocket(connectSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "客户端关闭连接成功...\n";
	}

	int iResult;
	char recvbuf[BUFFER_LEN];
	do {
		iResult = recv(connectSock, recvbuf, BUFFER_LEN, 0);
		if (iResult > 0) {
			cout << "接收到" << iResult << "字节		内容： " << recvbuf << std::endl;
		}
		else if (iResult == 0) {
			cout << "服务器连接关闭...\n";
		}
		else {
			cout << "接收服务器数据失败...\n";
			closesocket(connectSock);
			WSACleanup();
			return 1;
		}
	} while (iResult>0);

	closesocket(connectSock);
	WSACleanup();
	return 0;
}