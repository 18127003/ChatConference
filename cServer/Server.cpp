#include "Server.h"


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

Server::Server()
{
	//serverIPAddr = "192.168.159.1";
	serverIPAddr = GetLocalIpv4(NetworkInterfaceType::Wireless80211);
	serverPort = 1234;

	createSocket();
}

Server^ Server::getObject()
{
	if (_instance == nullptr)
	{
		_instance = gcnew Server();
	}

	return _instance;
}

Server::~Server()
{
	delete serverSocket;
}

int Server::createSocket()
{
	serverSocket = gcnew Socket(AddressFamily::InterNetwork,
		SocketType::Stream, ProtocolType::Tcp);

	return 0;
}

//Helper


bool Server::checkLogin(String^ _Username, String^ _Password, String^& errorMsg)
{
	//Check for blank!
	if (String::IsNullOrEmpty(_Username) || String::IsNullOrEmpty(_Password))
	{
		errorMsg = "Username or Password can't be blank!";
		return false;
	}

	//Check for special characters
	//String^ lstSpecialCharacters = "|"
	if (_Username->Contains("|") || _Username->Contains("|"))
	{
		errorMsg = "Username or password can't contain special characters!";
		return false;
	}

	//Check account in database
	if (!checkAccount(_Username, _Password))
	{
		errorMsg = "Username or Password is not correct!";
		return false;
	}

	//Check account in current online
	for each (String^ user in getListClient())
	{
		if (_Username == user)
		{
			errorMsg = user + " is current login on another client!";
			return false;
		}
	}

	return true;
}

bool Server::checkAccount(String^ _Username, String^ _Password)
{
	array<String^>^ lines = System::IO::File::ReadAllLines(accountPath);
	for each (String^ line in lines)
	{
		if (line == _Username + "|" + _Password)
		{
			return true;
		}
	}

	return false;
}

bool Server::checkSignup(String^ _Username, String^ _Password, String^& errorMsg)
{
	//Check for blank!
	if (String::IsNullOrEmpty(_Username) || String::IsNullOrEmpty(_Password))
	{
		errorMsg = "Username or Password can't be blank!";
		return false;
	}

	//Check for special characters
	//String^ lstSpecialCharacters = "|"
	if (_Username->Contains("|") || _Password->Contains("|"))
	{
		errorMsg = "Username or password can't contain special characters!";
		return false;
	}

	//Check exists username in database
	if (checkAccountExists(_Username))
	{
		errorMsg = "This username has been registered!";
		return false;
	}

	if (!addAnAccountToDatabase(_Username, _Password))
	{
		errorMsg = "Can't register!";
		return false;
	}

	return true;
}

bool Server::checkAccountExists(String^ _Username)
{
	array<String^>^ lines = System::IO::File::ReadAllLines(accountPath);
	for each (String^ line in lines)
	{
		if (line->Contains(_Username + "|"))
		{
			return true;
		}
	}

	return false;
}

bool Server::addAnAccountToDatabase(String^ _Username, String^ _Password)
{
	try
	{
		System::IO::File::AppendAllText(accountPath, _Username + "|" + _Password + "\n");
	}
	catch (Exception^ e)
	{
		return false;
	}
	return true;
}

bool Server::login(String^ _UserName, String^ _Password, Socket^ _ClientSocket)
{
	String^ errorMsg = "";
	if (checkLogin(_UserName, _Password, errorMsg))
	{
		cli_list->Add(gcnew Client(_ClientSocket, _UserName));
		mainScreen->UpdateConnectedClient(getListClient()); //Update list connected

		mainScreen->AddTextToContent(_UserName + " hast just online!\n");
		loginResponse(true, errorMsg, _ClientSocket);
		mainScreen->UpdateConnectedClient(getListClient());
		sendLoginNotification(_UserName, _ClientSocket);

		return true; //Login successs
	}

	loginResponse(false, errorMsg, _ClientSocket);

	return false; //Error login
}

bool Server::signup(String^ _UserName, String^ _Password, Socket^ _ClientSocket)
{
	String^ errorMsg = "";
	if (checkSignup(_UserName, _Password, errorMsg))
	{
		signupResponse(true, errorMsg, _ClientSocket);
		mainScreen->UpdateClientList(getRegisteredClientList());

		return true; //Signup successs
	}
	signupResponse(false, errorMsg, _ClientSocket);

	return false; //Error sign up
}

void Server::signupResponse(bool _IsSucc, String^ errorMsg, Socket^ _ClientSocket)
{
	MsgStruct^ resmsg = gcnew MsgStruct();

	resmsg->pack(MessageType::ResponseSignup, nullptr, nullptr);
	resmsg->pullBool(_IsSucc);
	resmsg->pullMsg(errorMsg);
	_ClientSocket->Send(resmsg->getContent()); //Send the result to client.
}

void Server::userStatusResponse(Socket^ _ClientSocket)
{
	MsgStruct^ statmsg = gcnew MsgStruct;
	array<String^>^ lstOnlineUsers = nullptr;
	lstOnlineUsers = getListClient()->ToArray();
	String^ strListOnlineUsers = "";
	if (lstOnlineUsers != nullptr)
	{
		for (int i = 0; i < lstOnlineUsers->Length - 1; ++i)
			strListOnlineUsers += lstOnlineUsers[i] + "|";
		if (lstOnlineUsers->Length > 0)
			strListOnlineUsers += lstOnlineUsers[lstOnlineUsers->Length - 1];
		//End of merging!!
	}
	statmsg->pack(MessageType::UserStatus, nullptr, nullptr);
	statmsg->pullMsg(strListOnlineUsers);
	_ClientSocket->Send(statmsg->getContent());
}

void Server::sendLoginNotification(String^ _Username, Socket^ _ClientSocket)
{
	MsgStruct^ loginNoti = gcnew MsgStruct();
	//loginNoti->strUsername = _Username;
	//array<Byte>^ buff = loginNoti->pack();
	loginNoti->pack(MessageType::LoginNotification, nullptr, _Username);

	for each (Client^ cli in cli_list)
	{
		if (cli->clientSocket != _ClientSocket) //Send the others login notication
		{
			cli->clientSocket->Send(loginNoti->getContent());
		}
	}
}

void Server::sendLogoutNotification(Socket^ _ClientSocket)
{
	String^ clientUsername = getUsernameBySocket(_ClientSocket);
	MsgStruct^ logoutNoti = gcnew MsgStruct();
	logoutNoti->pack(MessageType::LogoutNotification, nullptr, clientUsername);

	mainScreen->AddTextToContent(clientUsername + " hast just offline!");
	removeClientByUsername(clientUsername);
	mainScreen->UpdateConnectedClient(getListClient()); //Update list connected

	for each (Client^ cli in cli_list)
	{
		if (cli->clientSocket != _ClientSocket) //Send the others logout notication
		{
			cli->clientSocket->Send(logoutNoti->getContent());
		}
	}
}

void Server::requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize, Socket^ _ClientSocket)  //change
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	MsgStruct^ freq = gcnew MsgStruct;
	freq->pack(MessageType::RequestSendFile, nullptr, sender);
	freq->pullMsg(_FileName);
	freq->pullInt(_iFileSize);
	Socket^ receiver = getSocketByUsername(_ToUsername);
	receiver->Send(freq->getContent());
}

void Server::responseSendFile(String^ _ToUsername, bool _IsAccept, Socket^ _ClientSocket)
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	MsgStruct^ fres = gcnew MsgStruct;
	//fres->strUsername = sender;
	//fres->IsAccept = _IsAccept;

	Socket^ receiver = getSocketByUsername(_ToUsername);
	fres->pack(MessageType::ResponseSendFile, nullptr, sender);
	fres->pullBool(_IsAccept);
	receiver->Send(fres->getContent());
}

void Server::sendPrivateFilePackage(String^ _ToUsername, String^ _Filename, int _iPackageNumber, int _TotalPackage, array<Byte>^ _bData, Socket^ _ClientSocket)
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	MsgStruct^ prvfmsg = gcnew MsgStruct;

	prvfmsg->pack(MessageType::PrivateFile, nullptr, sender);
	prvfmsg->pullMsg(_Filename);
	prvfmsg->pullInt(_iPackageNumber);
	prvfmsg->pullInt(_TotalPackage);
	prvfmsg->pullData(_bData);
	Socket^ receiver = getSocketByUsername(_ToUsername);
	receiver->Send(prvfmsg->getContent());
}

void Server::loginResponse(bool _IsSucc, String^ _errorMsg, Socket^ _ClientSocket)
{
	MsgStruct^ resmsg = gcnew MsgStruct;
	//resmsg->IsSuccess = _IsSucc;
	//resmsg->errorMsg = _errorMsg;
	//array<Byte>^ buff = resmsg->pack();
	resmsg->pack(MessageType::ResponseLogin, nullptr, nullptr);
	resmsg->pullBool(_IsSucc);
	resmsg->pullMsg(_errorMsg);
	//if (_errorMsg != nullptr) resmsg->pullMsg(_errorMsg);

	_ClientSocket->Send(resmsg->getContent()); //Send the result to client.
}

int Server::sendPublicMsgToClients(String^ _strMessage, Socket^ _ClientSocket)
{
	String^ username = getUsernameBySocket(_ClientSocket);
	String^ cli_msg = username + ": " + _strMessage;
	MsgStruct^ Msgpack = gcnew MsgStruct;
	//Msgpack->strMessage = cli_msg;                    //Set msg content
	//array<Byte>^ buff = Msgpack->pack();			//Pack msg to send
	Msgpack->pack(MessageType::PublicMessage, nullptr, nullptr);
	Msgpack->pullMsg(cli_msg);

	for each (Client^ cli in cli_list)
	{
		cli->clientSocket->Send(Msgpack->getContent());
	}

	return 0;
}

void Server::sendPublicFilePackage(String^ _Filename, int _iPackageNumber, int _TotalPackage, array<Byte>^ _bData) {
	MsgStruct^ Msgpack = gcnew MsgStruct;
	Msgpack->pack(MessageType::PublicFile, nullptr, nullptr);
	Msgpack->pullMsg(_Filename);
	Msgpack->pullInt(_iPackageNumber);
	Msgpack->pullInt(_TotalPackage);
	Msgpack->pullData(_bData);
	for each (Client^ cli in cli_list)
	{
		cli->clientSocket->Send(Msgpack->getContent());
	}
}

int Server::sendPrivateMessage(String^ _ToUsername, String^ _Message, Socket^ _SenderSocket)
{
	String^ sender = getUsernameBySocket(_SenderSocket);
	Socket^ receiverSocket = getSocketByUsername(_ToUsername);
	//If receiver is offline (nullptr)

	MsgStruct^ prvmsg = gcnew MsgStruct;
	if (receiverSocket == nullptr)
	{
		//Send error message back to sender
		prvmsg->pack(MessageType::PrivateMessage, _ToUsername, nullptr);
		prvmsg->pullMsg("Error: " + _ToUsername + " is offline!");
		_SenderSocket->Send(prvmsg->getContent());
	}
	else
	{
		prvmsg->pack(MessageType::PrivateMessage, sender, nullptr);
		prvmsg->pullMsg(sender + ": " + _Message);
		receiverSocket->Send(prvmsg->getContent());
	}

	return 0;
}



String^ Server::getUsernameBySocket(Socket^ _socket)
{
	for each (Client^ cli in cli_list)
		if (cli->clientSocket == _socket)
			return cli->strUsermame;

	return nullptr;
}

Socket^ Server::getSocketByUsername(String^ _username)
{
	for each (Client^ cli in cli_list)
		if (cli->strUsermame == _username)
			return cli->clientSocket;

	return nullptr;
}

void Server::removeClientByUsername(String^ _username)
{
	for each (Client^ cli in cli_list)
		if (cli->strUsermame == _username)
		{
			cli_list->Remove(cli);
			break;
		}
}

List<String^>^ Server::getListClient()
{
	List<String^>^ listClient = gcnew List<String^>;
	for each (Client^ cli in cli_list)
	{
		listClient->Add(cli->strUsermame);
	}

	return listClient;
}

List<String^>^ Server::getRegisteredClientList()
{
	array<String^>^ lines = System::IO::File::ReadAllLines(accountPath);
	List<String^>^ listLines = gcnew List<String^>();
	for each (String^ line in lines)
	{

		listLines->Add(line->Substring(0, line->IndexOf("|")));
	}

	return listLines;
}


