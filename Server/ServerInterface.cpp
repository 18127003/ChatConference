#include "ServerInterface.h"
#include "DataStruct.h"
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
			MsgStruct^ msgReceived = MsgControl::unpack(buff);
			//if (msgReceived == nullptr) continue;   ???????

			switch (msgReceived->messageType)
			{
			
			case MsgStruct::MessageType::Login:
			{
				LoginMsg^ loginmsg = (LoginMsg^)msgReceived;
				Server::getObject()->login(loginmsg->strUsername, loginmsg->strPassword, socket);
				break;
			}
		
			case MsgStruct::MessageType::PublicMessage:
			{
				PublicMsg^ pubMsgStr = (PublicMsg^)msgReceived;

				Server::getObject()->sendPublicMsgToClients(pubMsgStr->strMessage, socket);
				break;
			}
			/*
			case MsgStruct::MessageType::PrivateMessage:
			{
				PrivateMsg^msg = (PrivateMsg^)msgReceived;
				Server::getObject()->sendPrivateMessage(msg->strToUsername, msg->strMessage, socket);
				break;
			}
			*/
			case MsgStruct::MessageType::Signup:
			{
				SignupMsg^ signupmsg = (SignupMsg^)msgReceived;
				Server::getObject()->signup(signupmsg->strUsername, signupmsg->strPassword, socket);
				break;
			}
			case MsgStruct::MessageType::UserStatus:
			{
				Server::getObject()->userStatusResponse(socket);
				break;
			}
			case MsgStruct::MessageType::LogoutNotification:
			{
				Server::getObject()->sendLogoutNotification(socket);
				break;
			}
			/*
			case MsgStruct::MessageType::RequestSendFile:
			{
				RequestFileMsg^ freq = (RequestFileMsg^)msgReceived;
				Server::getObject()->freq(freq->strUsername, freq->strFileName, freq->iFileSize, socket);
				break;
			}
			case MsgStruct::MessageType::ResponseSendFile:
			{
				ResponseFileMsg^ fres = (ResponseFileMsg^)msgReceived;
				Server::getObject()->fres(fres->strUsername, fres->IsAccept, socket);
				break;
			}
			case MsgStruct::MessageType::PrivateFile:
			{
				PrivateFileMsg^ fmsg = (PrivateFileMsg^)msgReceived;
				Server::getObject()->fmsg(fmsg->strUsername, fmsg->strFilename, fmsg->iPackageNumber, fmsg->iTotalPackage, fmsg->bData, socket);
				break;
			}
			*/
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
	txtClientChatBox->AppendText("\n");
}

void ServerInterface::UpdateConnectedClient(List<String^>^ lstClient) //
{
	txtConnected->Text = nullptr;
	for each (String^ user in lstClient)
	{
		txtConnected->AppendText(user);
		txtConnected->AppendText("\n");
	}
}

void ServerInterface::UpdateClientList(List<String^>^ lstClient)
{
	txtConnected->Text = nullptr;
	for each (String^ user in lstClient)
	{
		txtClientList->AppendText(user);
		txtClientList->AppendText("\n");
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
