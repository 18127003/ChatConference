#include "Server.h"
#include "DataStruct.h"

Server::Server()
{
	serverIPAddr = "192.168.1.5";
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

		mainScreen->AddTextToContent(_UserName + " hast just online!");
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
	ResSignupMsg^ resmsg = gcnew ResSignupMsg;
	resmsg->IsSuccess = _IsSucc;
	resmsg->errorMsg = errorMsg;
	array<Byte>^ buff = resmsg->pack();

	_ClientSocket->Send(buff); //Send the result to client.
}

void Server::userStatusResponse(Socket^ _ClientSocket)
{
	UsrStatusLstMsg^ statmsg = gcnew UsrStatusLstMsg;
	statmsg->lstOnlineUsers = getListClient()->ToArray();

	array<Byte>^ buff = statmsg->pack();
	_ClientSocket->Send(buff);
}

void Server::sendLoginNotification(String^ _Username, Socket^ _ClientSocket)
{
	LoginNotifiMsg^ loginNoti = gcnew LoginNotifiMsg;
	loginNoti->strUsername = _Username;
	array<Byte>^ buff = loginNoti->pack();

	for each (Client^ cli in cli_list)
	{
		if (cli->clientSocket != _ClientSocket) //Send the others login notication
		{
			cli->clientSocket->Send(buff);
		}
	}
}

void Server::sendLogoutNotification(Socket^ _ClientSocket)
{
	String^ clientUsername = getUsernameBySocket(_ClientSocket);
	LogoutNotiMsg^ logoutNoti = gcnew LogoutNotiMsg;
	logoutNoti->strUsername = clientUsername;
	array<Byte>^ buff = logoutNoti->pack();

	mainScreen->AddTextToContent(clientUsername + " hast just offline!");
	removeClientByUsername(clientUsername);
	mainScreen->UpdateConnectedClient(getListClient()); //Update list connected

	for each (Client^ cli in cli_list)
	{
		if (cli->clientSocket != _ClientSocket) //Send the others logout notication
		{
			cli->clientSocket->Send(buff);
		}
	}
}
/*
void Server::requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize, Socket^ _ClientSocket)
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	RequestFileMsg^ freq = gcnew RequestFileMsg;
	freq->strUsername = sender;
	freq->strFileName = _FileName;
	freq->iFileSize = _iFileSize;

	Socket^ receiver = getSocketByUsername(_ToUsername);
	array<Byte>^ byteData = freq->pack();
	receiver->Send(byteData);
}

void Server::responseSendFile(String^ _ToUsername, bool _IsAccept, Socket^ _ClientSocket)
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	ResponseFileMsg^ fres = gcnew ResponseFileMsg;
	fres->strUsername = sender;
	fres->IsAccept = _IsAccept;

	Socket^ receiver = getSocketByUsername(_ToUsername);
	array<Byte>^ byteData = fres->pack();
	receiver->Send(byteData);
}

void Server::sendPrivateFilePackage(String^ _ToUsername, String^ _Filename, int _iPackageNumber, int _TotalPackage, array<Byte>^ _bData, Socket^ _ClientSocket)
{
	String^ sender = getUsernameBySocket(_ClientSocket);
	PrivateFileMsg^ prvfmsg = gcnew PrivateFileMsg;
	prvfmsg->strUsername = sender;
	prvfmsg->strFilename = _Filename;
	prvfmsg->bData = _bData;
	prvfmsg->iPackageNumber = _iPackageNumber;
	prvfmsg->iTotalPackage = _TotalPackage;

	array<Byte>^ byteData = prvfmsg->pack();
	Socket^ receiver = getSocketByUsername(_ToUsername);
	receiver->Send(byteData);
}
*/
void Server::loginResponse(bool _IsSucc, String^ _errorMsg, Socket^ _ClientSocket)
{
	ResLoginMsg^ resmsg = gcnew ResLoginMsg;
	resmsg->IsSuccess = _IsSucc;
	resmsg->errorMsg = _errorMsg;
	array<Byte>^ buff = resmsg->pack();

	_ClientSocket->Send(buff); //Send the result to client.
}

int Server::sendPublicMsgToClients(String^ _strMessage, Socket^ _ClientSocket)
{
	String^ username = getUsernameBySocket(_ClientSocket);
	String^ cli_msg = username + ": " + _strMessage;
	PublicMsg^ Msgpack = gcnew PublicMsg;
	Msgpack->strMessage = cli_msg;                    //Set msg content
	array<Byte>^ buff = Msgpack->pack();			//Pack msg to send

	for each (Client^ cli in cli_list)
	{
		cli->clientSocket->Send(buff);
	}

	return 0;
}
/*
int Server::sendPrivateMessage(String^ _ToUsername, String^ _Message, Socket^ _SenderSocket)
{
	String^ sender = getUsernameBySocket(_SenderSocket);
	Socket^ receiverSocket = getSocketByUsername(_ToUsername);
	//If receiver is offline (nullptr)

	PrivateMessageMsg^ prvmsg = gcnew PrivateMessageMsg;
	if (receiverSocket == nullptr)
	{
		//Send error message back to sender
		privateMessageMsg->strMessage = "Error: " + _ToUsername + " is offline!";
		privateMessageMsg->strToUsername = _ToUsername;
		array<Byte>^ byteData = privateMessageMsg->pack();
		_SenderSocket->Send(byteData);
	}
	else
	{
		privateMessageMsg->strMessage = sender + ": " + _Message;
		privateMessageMsg->strToUsername = sender;
		array<Byte>^ byteData = privateMessageMsg->pack();
		receiverSocket->Send(byteData);
	}

	return 0;
}
*/


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


