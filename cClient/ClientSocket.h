#pragma once

using namespace System;
using namespace System::Net;
using namespace System::Net::NetworkInformation;
using namespace System::Net::Sockets;
using namespace System::Windows::Forms;
using namespace System::Runtime::Serialization;
using namespace System::Collections::Generic; //List
using namespace System::Text; //Encoding

static String^ GetLocalIpv4(NetworkInterfaceType type);

ref class ClientSocket
{
private:
	String^ serverIPAddr;
	int serverPort;

public:
	Socket^ socket;

	ClientSocket();
	ClientSocket(String^ _ServerIPAddr, int _ServerPort);
	int createSocket();
	int connectToServer();
	int connectToServer(String^ _ServerIPAddr, int _ServerPort);
	int sendMessage(array<Byte>^ buff);
	int sendMessage(array<Byte>^ buff, int size);
};

