#pragma once
#include "ClientSocket.h"
#include "DataStruct.h"
#include "Login.h"
#include "ClientInterface.h"
#include "PrivateChannel.h"

ref class Client;

//Singleton pattern

using namespace System::Threading;

ref class Client
{
private:
	static Client^ _instance = nullptr;
	Client(); //Don't allow to create object
public:
	static Client^ getObject();
	static MyClient::PrivateChannel^ tempPrvChat;
	~Client();
	ClientSocket^ cli_socket;
	MyClient::Login^ loginScreen = nullptr;
	MyClient::ClientInterface^ mainScreen = nullptr;
	List<MyClient::PrivateChannel^>^ lstPrivateChatForm = gcnew List<MyClient::PrivateChannel^>();
	String^ strUsername;

	System::Void ListenMessage();
	int createThreadListenMessageFromServer();

	int login(String^ _Username, String^ _Password);
	int logout();
	int signup(String^ _Username, String^ _Password);
	int sendPublicMessage(String^ _Message);
	int sendPrivateMessage(String^ _ToUsername, String^ _Message);
	int requestListOnlineUsers();
	int requestSendFile(String^ _ToUsername, String^ _FileName, int _iFileSize);
	int responseSendFile(String^ _ToUsername, bool _IsAccept);
	int sendPrivateFile(String^ _ToUsername, String^ _FileName, String^ _FilePath);


	// Set private message to suit form
	int setPrivateMessage(String^ _ToUsername, String^ _Message);

	//Others
	MyClient::PrivateChannel^ getPrivateChatFormByFriendUsername(String^ _Username);
	Thread^ threadListenClient;
};
