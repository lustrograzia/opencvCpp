
#include "opencv2/opencv.hpp"	// opencv를 사용하기 위한 헤더파일
#include <string>
#include <iostream>

//#include <WinSock2.h>	// 소켓을 사용하기 위한 헤더파일

#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "SerialComm.h"	// serial communication header

#pragma comment(lib, "ws2_32")	// 위에서 선언한 헤더파일들을 가져다 쓰기위한 링크

#define PORT 4578
#define PACKET_SIZE 1024
/*
int main()
{
	// SOCKET
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	// PF_INET : IPv4, SOCK_STREAM : 연결지향형 소켓, IPPROTO_TCP : TCP
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	while(1) {
		
	}

	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);
	printf("Recv Msg : %s\n", cBuffer);

	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);

	closesocket(hClient);
	closesocket(hListen);


	// Serial Communication
	
	char buffer;
	CSerialComm serialComm; 

	if (!serialComm.connect("COM6"))
	{
		std::cout << "connect filed" << "\n";
		return -1;
	}
	else
		std::cout << "connect successed" << "\n";
	
	
	WSACleanup();
	return 0;
}
*/