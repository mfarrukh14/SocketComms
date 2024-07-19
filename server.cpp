#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

// This function initializes the server
bool initialize()
{
	WSADATA data;

	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}


void InteractWithClient(SOCKET clientSocket)
{

	// recieving messages from client
	cout << "Client Connected" << endl;
	char buffer[4096];

	while (1)
	{
		int bytesRec = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (bytesRec <= 0)
		{
			cout << "Client Disconnected" << endl;
			break;
		}

		string message(buffer, bytesRec);

		cout << "Message recieved from client: " << message << endl;
	}

	closesocket(clientSocket);
}

int main()
{

	if (!initialize())
	{
		cout << "Server initialization failed";
		return 1;
	}


	//initializing the listening socket

	// AF_INET => corresponds to ipv4 addressing
	// SOCK_STREAM => corresponds to TCP method
	// third parameter just tells us the prototcol which we leave at 0 to be handled by the service provider

	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);


	// Always a good practice to check the return value of socket
	if (listeningSocket == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		return 1;
	}


	// creating addressing structure
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	//converting the ipaddress (0.0.0.0) to sin_family in binary form
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1)
	{
		cout << "Setting address structure failed" << endl;
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}

	// Binding the socket to server address
	if (bind(listeningSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "Socket binding failed" << endl;
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}

	// Listening on the specified port
	if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listening failed" << endl;
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server has started listening on port: " << port << endl;


	while (1)
	{
		// Accepting client on server socket
		SOCKET clientSocket = accept(listeningSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET)
		{
			cout << "Invalid Client socket" << endl;
		}

		thread t1(InteractWithClient, clientSocket);
	}


	closesocket(listeningSocket);

	// Always perform a cleanup at the end
	WSACleanup();
	return 0;
}