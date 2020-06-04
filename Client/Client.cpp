#include "Client.h"
#define NULL 0
using namespace System::IO;

Client::Client() //Don't allow to create object
{
	cli_socket = gcnew ClientSocket();
}

Client::~Client()
{
	delete cli_socket;
}

Client^ Client::getObject()
{
	if (_instance == nullptr)
	{
		_instance = gcnew Client();
	}
	return _instance;
}

System::Void Client::ListenMessage()
{
	while (true)
	{
		array<Byte>^ buff = gcnew array<Byte>(1024);
		Socket^ clientSocket = Client::getObject()->cli_socket->socket;
		int revc;
		try
		{
			revc = clientSocket->Receive(buff);
		}
		catch (Exception^ e)
		{
			MessageBox::Show("Server has just disconnected!");
			Client::getObject()->cli_socket->socket = nullptr;
			Application::Exit();
		}

		MsgStruct^ msgReceived = MsgControl::unpack(buff);
		if (msgReceived == nullptr) continue;
		switch (msgReceived->messageType)
		{
		//case MsgStruct::MessageType::Login:
			//MessageBox::Show("Login Message?");
			//break;
		case MsgStruct::MessageType::ResponseLogin:
		{
			ResLoginMsg^ Resmsg = (ResLoginMsg^)msgReceived;
			if (Resmsg->IsSuccess)
			{
				Client::getObject()->loginScreen->Hide();
				//Create new form
				Client::getObject()->mainScreen = gcnew MyClient::ClientInterface;
				Application::Run(Client::getObject()->mainScreen);
			}
			else
			{
				MessageBox::Show(Resmsg->errorMsg);
			}

			break;
		}
		case MsgStruct::MessageType::PublicMessage:
		{
			PublicMsg^ msg = (PublicMsg^)msgReceived;
			Client::getObject()->mainScreen->AddTextToContent(msg->strMessage);

			break;
		}
		case MsgStruct::MessageType::PrivateMessage:
		{
			PrivateMsg^ msg = (PrivateMsg^)msgReceived;
			Client::getObject()->setPrivateMessage(msg->strToUsername, msg->strMessage);
			break;
		}
		case MsgStruct::MessageType::ResponseSignup:
		{
			ResSignupMsg^ resmsg = (ResSignupMsg^)msgReceived;
			if (resmsg->IsSuccess)
			{
				MessageBox::Show("Signed up!");
			}
			else
			{
				MessageBox::Show(resmsg->errorMsg);
			}
			break;
		}
		//case MsgStruct::MessageType::Signup:
		//{
			//MessageBox::Show("Sign up received?");
			//break;
		//}
		case MsgStruct::MessageType::UserStatus:
		{
			UsrStatusLstMsg^ statlstmsg = (UsrStatusLstMsg^)msgReceived;
			Client::getObject()->mainScreen->SetOnlineUsers(statlstmsg->lstOnlineUsers);
			break;
		}
		case MsgStruct::MessageType::LoginNotification:
		{
			LoginNotifiMsg^ NotiMsg = (LoginNotifiMsg^)msgReceived;
			Client::getObject()->mainScreen->AddTextToContent(NotiMsg->strUsername + " has just online!");
			Client::getObject()->mainScreen->AddAnOnlineUser(NotiMsg->strUsername);
			break;
		}
		case MsgStruct::MessageType::LogoutNotification:
		{
			LogoutNotiMsg^ logoutmsg = (LogoutNotiMsg^)msgReceived;
			Client::getObject()->mainScreen->AddTextToContent(logoutmsg->strUsername + " left the conference!");
			Client::getObject()->mainScreen->DeleteAnOnlineUser(logoutmsg->strUsername);
			break;
		}
		case MsgStruct::MessageType::RequestSendFile:
		{
			RequestFileMsg^ reqmsg = (RequestFileMsg^)msgReceived;
			if (MessageBox::Show(reqmsg->strUsername + " want to send you a file " +
				reqmsg->strFileName + " (" + Convert::ToString(reqmsg->iFileSize) +
				" bytes).\nDo you want to receive?", "Receive a file", MessageBoxButtons::YesNo) == DialogResult::Yes)
			{
				Client::getObject()->responseSendFile(reqmsg->strUsername, true);
			}
			else
			{
				Client::getObject()->responseSendFile(reqmsg->strUsername, false);
			}
			break;

		}
		case MsgStruct::MessageType::ResponseSendFile:
		{
			ResponseFileMsg^ resfmsg = (ResponseFileMsg^)msgReceived;
			MyClient::PrivateChannel^ prvChat = getPrivateChatFormByFriendUsername(resfmsg->strUsername);
			if (resfmsg->IsAccept)
			{
				setPrivateMessage(resfmsg->strUsername, resfmsg->strUsername + " accept a file "
					+ prvChat->fileNameToSend + " (" + Convert::ToString(prvChat->fileSizeToSend) + " bytes) from you!");

				sendPrivateFile(prvChat->strFriendUsername, prvChat->fileNameToSend, prvChat->filePathToSend);
			}
			else
			{

				setPrivateMessage(resfmsg->strUsername, resfmsg->strUsername + " don't accept to receive file "
					+ prvChat->fileNameToSend + " (" + Convert::ToString(prvChat->fileSizeToSend) + " bytes) from you!");
			}

			//Change listen Message to non-static method.
			//Send file thread


			break;
		}
		case MsgStruct::MessageType::PrivateFile:
		{
			PrivateFileMsg^ fmsg = (PrivateFileMsg^)msgReceived;
			MyClient::PrivateChannel^ prvChat = getPrivateChatFormByFriendUsername(fmsg->strUsername);
			if (fmsg->iPackageNumber == 1)
				prvChat->writerStream = gcnew FileStream(prvChat->pathFileToReceiver + "/" + fmsg->strFilename, FileMode::Create, FileAccess::Write);

			prvChat->writerStream->Write(fmsg->bData, 0, fmsg->bData->Length);
			setPrivateMessage(fmsg->strUsername, "Received: " + fmsg->iPackageNumber + "/" + fmsg->iTotalPackage);

			break;
		}

		default:
			break;
		}
	}
}

int Client::createThreadListenMessageFromServer()
{
	Thread^ newThread = gcnew Thread(gcnew ThreadStart(this, &Client::ListenMessage));
	newThread->SetApartmentState(ApartmentState::STA);
	newThread->Start();

	return 0;
}

int Client::login(String^ _Username, String^ _Password)
{
	LoginMsg^ loginpck = gcnew LoginMsg();
	loginpck->strUsername = _Username;
	loginpck->strPassword = _Password;

	array<Byte>^ byteData = loginpck->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::logout()
{
	LogoutNotiMsg^ logOut = gcnew LogoutNotiMsg;

	array<Byte>^ byteData = logOut->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::signup(String^ _Username, String^ _Password)
{
	SignupMsg^ msg= gcnew SignupMsg();
	msg->strUsername = _Username;
	msg->strPassword = _Password;

	array<Byte>^ byteData = msg->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::sendPublicMessage(String^ _Message)
{
	PublicMsg^ msg = gcnew PublicMsg;
	msg->strMessage = _Message;

	array<Byte>^ byteData = msg->pack();

	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::sendPrivateMessage(String^ _ToUsername, String^ _Message)
{
	PrivateMsg^ msg = gcnew PrivateMsg;
	msg->strToUsername = _ToUsername;
	msg->strMessage = _Message;

	array<Byte>^ byteData = msg->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::requestListOnlineUsers()
{
	UsrStatusLstMsg^ statlst = gcnew UsrStatusLstMsg;
	array<Byte>^ byteData = statlst->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize)
{
	RequestFileMsg^ reqmsg = gcnew RequestFileMsg;
	reqmsg->strFileName = _FileName;
	reqmsg->strUsername = _ToUsername;
	reqmsg->iFileSize = _iFileSize;
	array<Byte>^ byteData = reqmsg->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::responseSendFile(String^ _ToUsername, bool _IsAccept)
{
	ResponseFileMsg^ resfmsg = gcnew ResponseFileMsg;
	resfmsg->strUsername = _ToUsername;
	resfmsg->IsAccept = _IsAccept;
	array<Byte>^ byteData = resfmsg->pack();
	cli_socket->sendMessage(byteData);

	return 0;
}

int Client::sendPrivateFile(String^ _ToUsername, String^ _FileName, String^ _FilePath)
{
	PrivateFileMsg^ fmsg = gcnew PrivateFileMsg;
	fmsg->strFilename = _FileName;
	fmsg->strUsername = _ToUsername;
	//Spit to smaller packages to send to server

	array<Byte>^ buffer;

	FileStream^ fileStream = nullptr;
	try
	{
		fileStream = gcnew FileStream(_FilePath, FileMode::Open, FileAccess::Read);

		int length = (int)fileStream->Length;  // get file length
		buffer = gcnew array<Byte>(length);            // create buffer

		int count;                            // actual number of bytes read
		int sum = 0;                          // total number of bytes read

		// read until Read method returns 0 (end of the stream has been reached)
		while ((count = fileStream->Read(buffer, sum, length - sum)) > 0)
		{
			sum += count;  // sum is a buffer offset for next reading
		}

		int BUFF_SIZE = 512;
		int counter = 0;
		int curPackageNumber = 1;
		int iTotalPackage = sum / (BUFF_SIZE + 1) + 1;

		for (; curPackageNumber <= iTotalPackage; ++curPackageNumber)
		{
			int copyLength = counter + BUFF_SIZE < sum ? BUFF_SIZE : (sum % BUFF_SIZE);
			array<Byte>^ bData = gcnew array<Byte>(copyLength);
			Console::WriteLine(copyLength);
			System::Array::Copy(buffer, counter, bData, 0, copyLength);
			counter += BUFF_SIZE;

			fmsg->iPackageNumber = iTotalPackage;
			fmsg->iTotalPackage = iTotalPackage;
			fmsg->bData = bData;
			array<Byte>^ byteData = fmsg->pack();
			cli_socket->sendMessage(byteData);
			setPrivateMessage(_ToUsername, "Sent: " + curPackageNumber + "/" + iTotalPackage);
		}

	}
	catch (Exception^ e)
	{
		Console::WriteLine(e->Message);
	}
	finally
	{
		if (fileStream != nullptr)
			fileStream->Close();
	}

	return 0;
}

int Client::setPrivateMessage(String^ _ToUsername, String^ _Message)
{
	MyClient::PrivateChannel^ prvChatForm = getPrivateChatFormByFriendUsername(_ToUsername);
	if (prvChatForm != nullptr)
	{
		prvChatForm->AddTextToDisplayChatbox(_Message);
	}
	else
	{
		MyClient::PrivateChannel^ pChat = gcnew MyClient::PrivateChannel(strUsername, _ToUsername);
		lstPrivateChatForm->Add(pChat);
		setPrivateMessage(_ToUsername, _Message); //Re set private message

		Application::Run(pChat);  //include create  new thread
	}

	return 0;
}

MyClient::PrivateChannel^ Client::getPrivateChatFormByFriendUsername(String^ _Username)
{
	for each (MyClient::PrivateChannel^ prvChat in lstPrivateChatForm)
	{
		if (prvChat->strFriendUsername == _Username)
			return prvChat;
	}

	return nullptr; //not found
}
