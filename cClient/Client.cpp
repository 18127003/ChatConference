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

		RecMsgStruct^ msgReceived = gcnew RecMsgStruct;
		msgReceived->unpack(buff);
		switch (msgReceived->msgType)
		{
		case MessageType::ResponseLogin:
		{
			if (msgReceived->IsSuccess)
			{
				Client::getObject()->loginScreen->Hide();
				//Create new form
				Client::getObject()->mainScreen = gcnew MyClient::ClientInterface;
				Application::Run(Client::getObject()->mainScreen);
			}
			else
			{
				MessageBox::Show(msgReceived->errorMsg);
			}

			break;
		}
		case MessageType::PublicMessage:
		{
			Client::getObject()->mainScreen->AddTextToContent(msgReceived->strMessage);
			break;
		}
		case MessageType::PrivateMessage:
		{
			Client::getObject()->setPrivateMessage(msgReceived->ToUsername, msgReceived->strMessage);
			break;
		}
		case MessageType::ResponseSignup:
		{
			if (msgReceived->IsSuccess)
			{
				MessageBox::Show("Signed up!");
			}
			else
			{
				MessageBox::Show(msgReceived->errorMsg);
			}
			break;
		}
		case MessageType::UserStatus:
		{
			Client::getObject()->mainScreen->SetOnlineUsers(msgReceived->lstOnlineUsers);
			break;
		}
		case MessageType::LoginNotification:
		{
			Client::getObject()->mainScreen->AddTextToContent(msgReceived->usrname + " has just online!");
			Client::getObject()->mainScreen->AddAnOnlineUser(msgReceived->usrname);
			break;
		}
		case MessageType::LogoutNotification:
		{
			Client::getObject()->mainScreen->AddTextToContent(msgReceived->usrname + " left the conference!");
			Client::getObject()->mainScreen->DeleteAnOnlineUser(msgReceived->usrname);
			break;
		}
		case MessageType::RequestSendFile:
		{
			if (MessageBox::Show(msgReceived->usrname + " want to send you a file " +
				msgReceived->Filename + " (" + Convert::ToString(msgReceived->FileSize) +
				" bytes).\nDo you want to receive?", "Receive a file", MessageBoxButtons::YesNo) == DialogResult::Yes)
			{
				Client::getObject()->responseSendFile(msgReceived->usrname, true);
			}
			else
			{
				Client::getObject()->responseSendFile(msgReceived->usrname, false);
			}
			break;

		}
		case MessageType::ResponseSendFile:
		{
			MyClient::PrivateChannel^ prvChat = getPrivateChatFormByFriendUsername(msgReceived->usrname);
			if (msgReceived->IsSuccess)
			{
				setPrivateMessage(msgReceived->usrname, msgReceived->usrname + " accept a file "
					+ prvChat->fileNameToSend + " (" + Convert::ToString(prvChat->fileSizeToSend) + " bytes) from you!");

				sendPrivateFile(prvChat->strFriendUsername, prvChat->fileNameToSend, prvChat->filePathToSend);
			}
			else
			{

				setPrivateMessage(msgReceived->usrname, msgReceived->usrname + " don't accept to receive file "
					+ prvChat->fileNameToSend + " (" + Convert::ToString(prvChat->fileSizeToSend) + " bytes) from you!");
			}

			//Change listen Message to non-static method.
			//Send file thread


			break;
		}
		case MessageType::PrivateFile:
		{
			MyClient::PrivateChannel^ prvChat = getPrivateChatFormByFriendUsername(msgReceived->usrname);
			if (msgReceived->iPackageNumber == 1)
				prvChat->writerStream = gcnew FileStream(prvChat->pathFileToReceiver + "/" + msgReceived->Filename, FileMode::Create, FileAccess::Write);
			if (prvChat->writerStream->CanWrite) {
				prvChat->writerStream->Write(msgReceived->bData, 0, msgReceived->bData->Length);
				prvChat->writerStream->Flush();
			}

			setPrivateMessage(msgReceived->usrname, "Received: " + msgReceived->iPackageNumber + "/" + msgReceived->iTotalPackage);

			break;
		}
		case MessageType::PublicFile:  //Client.cpp
		{
			if (msgReceived->iPackageNumber == 1)
				internalStream = gcnew FileStream(filePath+"/"+msgReceived->Filename, FileMode::Create, FileAccess::Write); 
			internalStream->Write(msgReceived->bData, 0, msgReceived->bData->Length);
			internalStream->Flush();
			if (msgReceived->iPackageNumber == msgReceived->iTotalPackage) internalStream->Close();
			Client::getObject()->mainScreen->AddTextToContent(" Received: " + msgReceived->iPackageNumber + "/" + msgReceived->iTotalPackage);
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
	MsgStruct^ loginpck = gcnew MsgStruct;
	loginpck->pack(MessageType::Login, nullptr, _Username);
	loginpck->pullMsg(_Password);
	cli_socket->sendMessage(loginpck->getContent());

	return 0;
}

int Client::logout()
{
	MsgStruct^ logoutpck = gcnew MsgStruct;
	logoutpck->pack(MessageType::LogoutNotification, nullptr, nullptr);
	cli_socket->sendMessage(logoutpck->getContent());

	return 0;
}

int Client::signup(String^ _Username, String^ _Password)
{
	MsgStruct^ msg = gcnew MsgStruct;
	msg->pack(MessageType::Signup, nullptr, _Username);
	msg->pullMsg(_Password);
	cli_socket->sendMessage(msg->getContent());

	return 0;
}

int Client::sendPublicMessage(String^ _Message)
{
	MsgStruct^ msg = gcnew MsgStruct();
	msg->pack(MessageType::PublicMessage, nullptr, nullptr);
	msg->pullMsg(_Message);
	cli_socket->sendMessage(msg->getContent());

	return 0;
}

int Client::sendPrivateMessage(String^ _ToUsername, String^ _Message)
{
	MsgStruct^ msg = gcnew MsgStruct();
	msg->pack(MessageType::PrivateMessage, _ToUsername, nullptr);
	msg->pullMsg(_Message);
	cli_socket->sendMessage(msg->getContent());

	return 0;
}

int Client::requestListOnlineUsers()
{
	MsgStruct^ statlst = gcnew MsgStruct;
	statlst->pack(MessageType::UserStatus, nullptr, nullptr);
	cli_socket->sendMessage(statlst->getContent());

	return 0;
}

int Client::requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize)
{
	MsgStruct^ reqmsg = gcnew MsgStruct;
	//reqmsg->strFileName = _FileName;
	//reqmsg->strUsername = _ToUsername;
	//reqmsg->iFileSize = _iFileSize;
	reqmsg->pack(MessageType::RequestSendFile, _ToUsername, nullptr);  //change
	reqmsg->pullMsg(_FileName);
	reqmsg->pullInt(_iFileSize);
	cli_socket->sendMessage(reqmsg->getContent());

	return 0;
}

int Client::responseSendFile(String^ _ToUsername, bool _IsAccept)
{
	MsgStruct^ resfmsg = gcnew MsgStruct;
	//resfmsg->strUsername = _ToUsername;
	//resfmsg->IsAccept = _IsAccept;
	resfmsg->pack(MessageType::ResponseSendFile,nullptr,_ToUsername);
	resfmsg->pullBool(_IsAccept);
	cli_socket->sendMessage(resfmsg->getContent());

	return 0;
}

int Client::sendPrivateFile(String^ _ToUsername, String^ _FileName, String^ _FilePath)
{
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
			MsgStruct^ fmsg = gcnew MsgStruct;
			int copyLength = counter + BUFF_SIZE < sum ? BUFF_SIZE : (sum % BUFF_SIZE);
			array<Byte>^ bData = gcnew array<Byte>(copyLength);
			Console::WriteLine(copyLength);
			System::Array::Copy(buffer, counter, bData, 0, copyLength);
			counter += BUFF_SIZE;

			fmsg->pack(MessageType::PrivateFile,nullptr, _ToUsername);
			fmsg->pullMsg(_FileName);
			fmsg->pullInt(curPackageNumber);
			fmsg->pullInt(iTotalPackage);
			fmsg->pullData(bData);
			cli_socket->sendMessage(fmsg->getContent());
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

int Client::sendPublicFile(String^ _FileName, String^ _FilePath) {
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
			MsgStruct^ fmsg = gcnew MsgStruct;
			int copyLength = counter + BUFF_SIZE < sum ? BUFF_SIZE : (sum % BUFF_SIZE);
			array<Byte>^ bData = gcnew array<Byte>(copyLength);
			Console::WriteLine(copyLength);
			System::Array::Copy(buffer, counter, bData, 0, copyLength);
			counter += BUFF_SIZE;

			fmsg->pack(MessageType::PublicFile, nullptr, nullptr);
			fmsg->pullMsg(_FileName);
			fmsg->pullInt(curPackageNumber);
			fmsg->pullInt(iTotalPackage);
			fmsg->pullData(bData);
			cli_socket->sendMessage(fmsg->getContent());
			Client::getObject()->mainScreen->AddTextToContent(" Sent: "+curPackageNumber + "/"+ iTotalPackage);
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
