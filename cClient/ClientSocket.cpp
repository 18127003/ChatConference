#include "ClientSocket.h"

String^ GetLocalIpv4(NetworkInterfaceType type) {
	for each(NetworkInterface^ item in NetworkInterface::GetAllNetworkInterfaces())
	{
		if (item->NetworkInterfaceType == type && item->OperationalStatus == OperationalStatus::Up)
		{
			for each(UnicastIPAddressInformation^ ip in item->GetIPProperties()->UnicastAddresses)
			{
				if (ip->Address->AddressFamily == AddressFamily::InterNetwork)
				{
					return ip->Address->ToString();
				}
			}
		}
	}
	return "127.0.0.1";
}

ClientSocket::ClientSocket()
{
	//serverIPAddr = "192.168.159.1"; //Localhost IP
	serverIPAddr = GetLocalIpv4(NetworkInterfaceType::Wireless80211);
	serverPort = 1234; //Default server Port

	createSocket(); //Create a socket
}

ClientSocket::ClientSocket(String^ _ServerIPAddr, int _ServerPort)
{
	serverIPAddr = _ServerIPAddr;
	serverPort = _ServerPort;
}

int ClientSocket::createSocket()
{
	socket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	return 0;
}

int ClientSocket::connectToServer(String^ _ServerIPAddr, int _ServerPort)
{
	serverIPAddr = _ServerIPAddr;
	serverPort = _ServerPort;
	return connectToServer();
}

int ClientSocket::connectToServer()
{
	IPAddress^ ipAddress = IPAddress::Parse(serverIPAddr);
	IPEndPoint^ ServerIPEndPoint = gcnew IPEndPoint(ipAddress, serverPort);

	try
	{
		socket->Connect(ServerIPEndPoint);
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message, "Error Connection", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return 1; //Error
	}

	return 0;
}

int ClientSocket::sendMessage(array<Byte>^ buff)
{
	if (socket != nullptr)
		return socket->Send(buff, buff->Length, SocketFlags::None); //Sent bytes

	return 0;
}

int ClientSocket::sendMessage(array<Byte>^ buff, int size)
{
	return socket->Send(buff, size, SocketFlags::None); //Sent bytes
}

