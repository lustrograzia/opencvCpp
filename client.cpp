#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#define PORT 4578
#define PACKET_SIZE 1024
#define SERVER_IP "10.10.23.10"
/*
int main()
{
	WSADATA wsaData;
	int temp = WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	char cMsg[] = "Client Send";
	send(hSocket, cMsg, strlen(cMsg), 0);

	char cBuffer[PACKET_SIZE] = {};
	recv(hSocket, cBuffer, PACKET_SIZE, 0);

	printf("Recv Msg : %s\n", cBuffer);

	closesocket(hSocket);

	WSACleanup();
	return 0;
}*/
