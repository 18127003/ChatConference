#pragma once
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections::Generic; //List
using namespace System::Threading; //Thread

ref class Client
{
public:
	Socket^ clientSocket = nullptr;
	String^ strUsermame = nullptr;

	Client() {};
	Client(Socket^ _clientSocket, String^ _strUsermame) : clientSocket(_clientSocket), strUsermame(_strUsermame) {};
};

