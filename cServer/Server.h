#pragma once
//Singleton Pattern

using namespace System;
using namespace System::Net;
using namespace System::Net::NetworkInformation;
using namespace System::Net::Sockets;
using namespace System::Collections::Generic; //List
using namespace System::Threading; //Thread

static String^ GetLocalIpv4(NetworkInterfaceType type);

ref class Server; //Prototype
#include "ServerInterface.h"
#include "Client.h"
#include "MsgStruct.h"

ref class Server
{
private:
	static Server^ _instance = nullptr;
	Server();
	int createSocket();
public:
	static Server^ getObject();
	~Server();

	//Database
	String^ accountPath = "../Server/Database/accounts.txt";

	//Form
	MyServer::ServerInterface^ mainScreen;

	//Socket
	String^ serverIPAddr;
	int serverPort;
	Socket^ serverSocket;
	List<Client^>^ cli_list = gcnew List<Client^>();



	//Helper
	bool checkLogin(String^ _Username, String^ _Password, String^& errorMsg);
	bool checkAccount(String^ _Username, String^ _Password);
	bool checkSignup(String^ _Username, String^ _Password, String^& errorMsg);
	bool checkAccountExists(String^ _Username);
	bool addAnAccountToDatabase(String^ _Username, String^ _Password);

	//Main calling
	bool login(String^ _UserName, String^ _Password, Socket^ _ClientSocket);
	void loginResponse(bool _IsSucc, String^ errorMsg, Socket^ _ClientSocket);
	bool signup(String^ _Username, String^ _Password, Socket^ _ClientSocket);
	void signupResponse(bool _IsSucc, String^ errorMsg, Socket^ _ClientSocket);
	void userStatusResponse(Socket^ _ClientSocket);
	void sendLoginNotification(String^ _Username, Socket^ _ClientSocket);
	void sendLogoutNotification(Socket^ _ClientSocket);
	void requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize, Socket^ _ClientSocket);
	void responseSendFile(String^ _ToUsername, bool _IsAccept, Socket^ _ClientSocket);
	void sendPrivateFilePackage(String^ _ToUsername, String^ _Filename, int _iPackageNumber, int _TotalPackage, array<Byte>^ _bData, Socket^ _ClientSocket);
	void sendPublicFilePackage(String^ _Filename, int _iPackageNumber, int _TotalPackage, array<Byte>^ _bData);

	int sendPublicMsgToClients(String^ _strMessage, Socket^ _ClientSocket);

	//Server send message to specify username client socket
	int sendPrivateMessage(String^ _ToUsername, String^ _Message, Socket^ _ClientSocket);

	//Others
	List<String^>^ getListClient();
	List<String^>^ getRegisteredClientList();
	String^ getUsernameBySocket(Socket^ _socket);
	Socket^ getSocketByUsername(String^ _username);
	void removeClientByUsername(String^ _username);
};


