#include "ServerInterface.h"

using namespace MyServer;

System::Void ServerInterface::MainScreen_Load(System::Object^  sender, System::EventArgs^  e)
{
	Server^ server = Server::getObject();

	//Set default serverIPAddr, serverPort
	txtIPServer->Text = server->serverIPAddr;
	txtPortServer->Text = Convert::ToString(server->serverPort);

	UpdateClientList(server->getRegisteredClientList());
}

void ServerInterface::ListenClientMessage(Object^ obj)
{
	Socket^ socket = (Socket^)obj;

	while (true)
	{

		array<Byte>^ buff = gcnew array<Byte>(1024);
		int recv = socket->Receive(buff);
		RecMsgStruct^ msgReceived = gcnew RecMsgStruct;
		msgReceived->unpack(buff);
		//if (msgReceived == nullptr) continue;   ???????

		switch (msgReceived->msgType)
		{

		case MessageType::Login:
		{
			//RecLSMsg^ loginmsg = (RecLSMsg^)msgReceived;
			Server::getObject()->login(msgReceived->usrname, msgReceived->password, socket);
			break;
		}

		case MessageType::PublicMessage:
		{
			//RecPublicMsg^ pubMsgStr = (RecPublicMsg^)msgReceived;

			Server::getObject()->sendPublicMsgToClients(msgReceived->strMessage, socket);
			break;
		}
		case MessageType::PrivateMessage:
		{
			Server::getObject()->sendPrivateMessage(msgReceived->ToUsername, msgReceived->strMessage, socket);
			break;
		}
		case MessageType::Signup:
		{
			//RecLSMsg^ signupmsg = (RecLSMsg^)msgReceived;
			Server::getObject()->signup(msgReceived->usrname, msgReceived->password, socket);
			break;
		}
		case MessageType::UserStatus:
		{
			Server::getObject()->userStatusResponse(socket);
			break;
		}
		case MessageType::LogoutNotification:
		{
			Server::getObject()->sendLogoutNotification(socket);
			break;
		}
		case MessageType::RequestSendFile:
		{
			Server::getObject()->requestSendFile(msgReceived->ToUsername, msgReceived->Filename, msgReceived->FileSize, socket);  //Change
			break;
		}
		case MessageType::ResponseSendFile:
		{
			Server::getObject()->responseSendFile(msgReceived->usrname, msgReceived->IsSuccess, socket);
			break;
		}
		case MessageType::PrivateFile:
		{
			Server::getObject()->sendPrivateFilePackage(msgReceived->usrname, msgReceived->Filename, msgReceived->iPackageNumber, msgReceived->iTotalPackage, msgReceived->bData, socket);
			break;
		}
		case MessageType::PublicFile: //serverInterface.cpp
		{
			Server::getObject()->sendPublicFilePackage(msgReceived->Filename, msgReceived->iPackageNumber, msgReceived->iTotalPackage, msgReceived->bData);
			break;
		}
		default:
			break;
		}
	}

}

System::Void ServerInterface::backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	Socket^ serverSocket = Server::getObject()->serverSocket;

	while (true)
	{
		Socket^ connector = serverSocket->Accept(); //Accept Connection From Client
		threadListenClient = gcnew Thread(gcnew ParameterizedThreadStart(ServerInterface::ListenClientMessage));
		threadListenClient->IsBackground = true;
		threadListenClient->Start(connector); //Listen messages from client
	}

}

void ServerInterface::AddTextToContent(String^ text)
{
	txtClientChatBox->AppendText(text);
	txtClientChatBox->AppendText(Environment::NewLine);
}

void ServerInterface::UpdateConnectedClient(List<String^>^ lstClient) //
{
	txtConnected->Text = nullptr;
	for each (String^ user in lstClient)
	{
		txtConnected->AppendText(user);
		txtConnected->AppendText(Environment::NewLine);
	}
}

void ServerInterface::UpdateClientList(List<String^>^ lstClient)
{
	txtConnected->Text = nullptr;
	for each (String^ user in lstClient)
	{
		txtClientList->AppendText(user);
		txtClientList->AppendText(Environment::NewLine);
	}
}

System::Void ServerInterface::btListen_Click(System::Object^  sender, System::EventArgs^  e)
{
	Server^ server = Server::getObject();

	//Listen 
	IPAddress^ serverIPAddress = IPAddress::Parse(server->serverIPAddr);
	int serverPort = server->serverPort;
	IPEndPoint^ serverIPEndPoint = gcnew IPEndPoint(serverIPAddress, serverPort);

	server->serverSocket->Bind(serverIPEndPoint);
	server->serverSocket->Listen(10);

	backgroundWorker1->WorkerSupportsCancellation = true;
	backgroundWorker1->RunWorkerAsync();

	//Disable this button
	btListen->Enabled = false;
	txtIPServer->ReadOnly = true;
	txtPortServer->ReadOnly = true;
}
