#pragma once

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Windows::Forms;
using namespace System::Runtime::Serialization;
using namespace System::Collections::Generic; //List
using namespace System::Text; //Encoding

enum class MessageType {
	PublicMessage, PrivateMessage, PrivateFile, UserStatus, RequestSendFile, ResponseSendFile,
	Login, ResponseLogin, Signup, ResponseSignup, LoginNotification, LogoutNotification, PublicFile
};

ref class MsgStruct
{
private:
	List<Byte>^ content;
public:
	void pack(MessageType msgtype, String^ tousr, String^ usrname);
	array<Byte>^ getContent() {
		return content->ToArray();
	};
	MsgStruct() {
		content = gcnew List<Byte>();
	};
	void pullMsg(String^ msg);
	void pullInt(int i);
	void pullBool(bool b);
	void pullData(array<Byte>^ data);
};


ref class RecMsgStruct
{
public:
	MessageType msgType;
	String^ usrname;
	String^ password;
	int IsSuccess = false;
	String^ errorMsg;
	String^ strMessage;
	String^ ToUsername;
	String^ Filename;
	int FileSize;
	int iPackageNumber;
	int iTotalPackage;

    array<Byte>^ bData;
	array<String^>^ lstOnlineUsers;
	void unpack(array<Byte>^ buff);
};