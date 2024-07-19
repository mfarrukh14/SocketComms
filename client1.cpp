#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")


bool initialize()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main()
{
	if (!initialize())
	{
		cout << "Initialization failed..." << endl;
	}

	int port = 12345;
	string serverAddress = "127.0.0.1";

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
	{
		cout << "Invalid socket..." << endl;
	}

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serverAddress.c_str(), (&serveraddr.sin_addr));

	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "Unable to connect to server..." << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}


	cout << "Client 1 Online";

	string message = "Hello from client1";
	int bytesSent = send(s, message.c_str(), message.length(), 0);

	if (bytesSent == SOCKET_ERROR)
	{
		cout << "Send Failed!";
	}

	closesocket(s);


	WSACleanup();
	return 0;
}