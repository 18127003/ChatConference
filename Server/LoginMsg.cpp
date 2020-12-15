#include "LoginMsg.h"
LSMsg::LSMsg(MessageType msgtype)
{
	IsSuccess = 0;
	//strUsername = nullptr;
	//strPassword = nullptr;
	content->AddRange(BitConverter::GetBytes(int(msgtype)));
}

RecLSMsg::RecLSMsg(MessageType msgtype) {
	msgType = msgtype;
	usrname = nullptr;
	password = nullptr;
	switch (msgtype) {
	case MessageType::Login:
		//errorMsg = gcnew String("");
		errorMsg = "Login error";
		break;
	case MessageType::LoginNotification:
		errorMsg = "Login notify error";
		break;
	case MessageType::LogoutNotification:
		errorMsg = "Logout notify error";
		break;
	case MessageType::ResponseLogin:
		errorMsg = "Login respond error";
		break;
	case MessageType::ResponseSignup:
		errorMsg = "Signed up respond error";
		break;
	case MessageType::Signup:
		errorMsg = "Signed up error";
		break;
	}
}

void LSMsg::pack() {
	content->AddRange(BitConverter::GetBytes(IsSuccess));
}

RecMsgStruct^ RecLSMsg::unpack(array<Byte>^ buff) {
	int offset = 4;
	int msglen;
	switch (msgType) {
	case MessageType::Login:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0) usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen; //Update offset
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		if (msglen > 0) password = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::LoginNotification:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::LogoutNotification:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::Signup:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0) usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen; //Update offset
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		if (msglen > 0) password = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::ResponseLogin:
		IsSuccess = BitConverter::ToBoolean(buff, offset);
		offset += 1;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		if (msglen > 0)
			errorMsg = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::ResponseSignup:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);

		offset += msglen; //Update offset

		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		if (msglen > 0)
			password = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	}
	return this;
}

/*
void LSMsg::pack()
{
	//List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::Login)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));


	//add Password Info
	if (strPassword != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strPassword))); //Length of password
		byteData->AddRange(Encoding::UTF8->GetBytes(strPassword)); //Password string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	return byteData->ToArray();
}

MsgStruct^ LoginMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength, passwordLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	offset += usernameLength; //Update offset

	passwordLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update offset
	if (passwordLength > 0)
		strPassword = Encoding::UTF8->GetString(buff, offset, passwordLength);

	return this;
}

LoginNotifiMsg::LoginNotifiMsg()
{
	strUsername = nullptr;
}

array<Byte>^ LoginNotifiMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::LoginNotification)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	return byteData->ToArray();
}

MsgStruct^ LoginNotifiMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	return this;
}

ResLoginMsg::ResLoginMsg()
{
	IsSuccess = false; //Not success
	errorMsg = nullptr;
}

array<Byte>^ ResLoginMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::ResponseLogin)));
	byteData->AddRange(BitConverter::GetBytes(IsSuccess));

	if (errorMsg != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(errorMsg))); //errorMsg length
		byteData->AddRange(Encoding::UTF8->GetBytes(errorMsg)); //errorMsg string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	return byteData->ToArray();
}

MsgStruct^ ResLoginMsg::unpack(array<Byte>^ buff)
{
	int offset = 4;
	IsSuccess = BitConverter::ToBoolean(buff, offset);
	offset += 1;
	int errorMsgLength = BitConverter::ToInt32(buff, offset);
	offset += 4;
	if (errorMsgLength > 0)
		errorMsg = Encoding::UTF8->GetString(buff, offset, errorMsgLength);

	return this;
}

LogoutNotiMsg::LogoutNotiMsg()
{
	strUsername = nullptr;
}

array<Byte>^ LogoutNotiMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::LogoutNotification)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	return byteData->ToArray();
}

MsgStruct^ LogoutNotiMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	return this;
}
*/