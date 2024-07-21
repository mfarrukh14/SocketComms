#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>

using namespace std;

#pragma comment(lib,"ws2_32.lib")


bool initialize()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMsg(SOCKET s)
{
	cout << "Enter your chat name: " << endl;
	string name;
	getline(cin, name);

	string message;

	while (1)
	{
		getline(cin, message);

		string final_message = name + " : " + message;
		int bytes_sent = send(s, final_message.c_str(), final_message.length(), 0);

		if (bytes_sent == SOCKET_ERROR)
		{
			cout << "Error sending message!" << endl;
			break;
		}

		if (message == "quit")
		{
			cout << "Quitting application" << endl;
			break;
		}
	}

	closesocket(s);
	WSACleanup();

}

void RecieveMsg(SOCKET s)
{
	char buffer[4096];
	int recv_len;
	string msg = "";

	while (1)
	{
		recv_len = recv(s, buffer, sizeof(buffer), 0);
		if (recv_len <= 0)
		{
			cout << "Disconnected from server!" << endl;
			break;
		}
		else
		{
			msg = string(buffer, recv_len);
			cout << "\n" << msg << endl;
		}
	}

	closesocket(s);
	WSACleanup();

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


	cout << "Client 2 Online" << endl;

	thread senderThread(SendMsg, s);
	thread recieverThread(RecieveMsg, s);

	senderThread.join();
	recieverThread.join();

	return 0;
}