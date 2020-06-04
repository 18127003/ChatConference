#pragma once
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Windows::Forms;
using namespace System::Runtime::Serialization;
using namespace System::Collections::Generic; //List
using namespace System::Text; //Encoding

ref class MsgStruct abstract
{
public:
	enum class MessageType {
		PublicMessage, PrivateMessage, PrivateFile, UserStatus, RequestSendFile, ResponseSendFile,
		Login, ResponseLogin, Signup, ResponseSignup, LoginNotification, LogoutNotification
	};
	MessageType messageType;

	virtual array<Byte>^ pack() = 0; //Pack msg with type to send
	virtual MsgStruct^ unpack(array<Byte>^ buff) = 0;  //get msg content and type
	//virtual void encrypt() = 0; for encrypting msg content
};